//////////////////////////////////////////////////////////////////////////////
//
//    PRISCAS - Computer architecture simulator
//    Copyright (C) 2019 Winor Chen
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////
#include <csignal>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <string>
#include <memory>
#include "branding.h"
#include "cpu.h"
#include "diag_cpu.h"
#include "mb.h"
#include "mt_exception.h"
#include "mtsstream.h"
#include "interpret.h"
#include "runtime_call.h"
#include "shell.h"

namespace priscas
{
	/* A mapping from a string into a directive function pointer (must be a member of Shell)
	 */
	typedef std::pair<std::string, void(*)(const Arg_Vec &, Shell&)> directive_pair;

	/* Shell for CLASS
	 *
	 * The shell is essentially an interpreter that merely assembles the program
	 *
	 *
	 * wchen329
	 */
	void Shell::Run()
	{

		UPString app_brand =
			branding::APPLICATION_NAME + " " + branding::APPLICATION_VERSION + " " + branding::APPLICATION_DBG;

		// Set quiet mode
		UPString header = app_brand + priscas_io::newLine + branding::APPLICATION_DESC + priscas_io::newLine;
		WriteToOutput(header);

		// Set shell environment
		this->shEnv.characterize_Env(args);

		// Asm input was specified
		if(shEnv.get_Option_AsmInput())
		{

			if(!shEnv.get_Option_AsmInputSpecified())
			{
				WriteToError("Error: An input file is required (specified through -i [input file] ) in order to run in batch mode.\n");
				return;
			}

			else
			{
				inst_file = fopen(shEnv.get_asmFilenames()[0].c_str(), "r");
			}

			if(inst_file == NULL)
			{
				WriteToError("Error: The file specified cannot be opened or doesn't exist.\n");
				return;
			}
		}

		else
		{
			inst_file = stdin;
			WriteToOutput("Starting in interactive mode...\n");
			WriteToOutput("Tip: system directives are preceded by a . (for example .help)\n");
		}


		// Check for valid memory configurations
		if(shEnv.get_memBitwidth() <= 1)
		{
			WriteToError("Error: An error occurred when trying to read memory width (must be larger than 1 and a natural number).");
			return;
		}

		else if(shEnv.get_memBitwidth() > 20)
		{
			
			WriteToError("Error: Memory size specified is too large (must be less than or equal to 20 bits wide)");
			return;
		}

		// Set CPU Type
		cpu_t cp = STANDARD;

		if(!shEnv.get_cpuStrings().empty())
		{
			cp = static_cast<cpu_t>(atoi(shEnv.get_cpuStrings()[0].c_str()));
		}

		WriteToOutput("CPU Type: ");
		switch(cp)
		{
			case STANDARD:
				WriteToOutput("Single Cycle\n");
				break;
			case FIVE_P:
				WriteToOutput("Five Stage Pipeline\n");
				break;
			default:
				WriteToOutput("Invalid CPU type detected. Exiting...\n");
				return;
		}

		this->motherboard = new mb(cp, shEnv.get_memBitwidth());
		motherboard->reset();
		mb* MB_IN_PTR = motherboard;

		if(!isQuiet)
		{
			UPString msg = UPString( "Main Memory size: " + priscas_io::StrTypes::SizeToStr(motherboard->get_mmem_size()) + " bytes\n");
			WriteToOutput(msg);
		}

		/* Actual Execution Portion
		 * This is a "double-pass" assembler
		 * First run through: read symbols and map them with a symbol table
		 * Second run through: do the actual assembling
		 */


		/* First, if an input file was specified
		 * (1) collect file symbols
		 * (2) map it to memory assemble that file first
		 *
		 */
		if(shEnv.get_Option_AsmInputSpecified())
		{

			UPString_Vec lines;
			uint32_t equiv_pc = 0;
			BW_32 asm_pc = 0;

			char input_f_stream[255];
			memset(input_f_stream, 0, sizeof(input_f_stream));
			unsigned long line_number = 0;
			try
			{
				while(fgets(input_f_stream, 254, inst_file) != NULL)
				{
					line_number++;
					UPString current_line = UPString(input_f_stream);
					Arg_Vec parts = chop_string(current_line);
	
					// Remove strings that are just whitespace
					if(parts.empty())
						continue;

					// Symbol assignment: add the symbol to symbol table
					
					if(parts[0][parts[0].size() - 1] == ':')
					{
						this->jump_syms.insert(parts[0].substr(0, parts[0].size() - 1), equiv_pc);
						continue;
					}

					if(parts[0][0] == '.')
					{
						this->directive_syms.insert(current_line, equiv_pc);
						continue;
					}

					this->line_number_to_PC.insert(std::pair<unsigned long, unsigned long>(line_number, equiv_pc));
					this->PC_to_line_number.insert(std::pair<unsigned long, unsigned long>(equiv_pc, line_number));
					this->PC_to_line_string.insert((std::pair<unsigned long, std::string>(equiv_pc, current_line)));
					equiv_pc = equiv_pc + 4;
					lines.push_back(current_line);		
				}
			}
			catch(mt_exception& e)
			{
				WriteToOutput("An error has occurred when writing debugging symbols and assigning directives:\n\t");
				WriteToOutput(e.get_err().c_str());
				return;
			}

			// Add pre-declared program breakpoints, if any
			while(!this->queued_prog_breakpoints.empty())
			{
				this->add_program_breakpoint(this->queued_prog_breakpoints.front());
				this->queued_prog_breakpoints.pop();
			}

			// Now assemble the rest
			for(size_t itr = 0; itr < lines.size(); itr++)
			{
				if(!this->AsmFlash(lines[itr], *motherboard, asm_pc))
				{
					return;
				}

				// Increment the PC at which to flash
				asm_pc.AsUInt32() += 4;
			}
			
		}

		// Write this to program, or remain in memory

	}

	/* Takes an input string and breaks that string into a vector of several
	 * based off of whitespace and tab delineators
	 * Also removes comments
	 * "Also acknowledges " " and ' ' and \ all used for escaping
	 */
	UPString_Vec chop_string(const UPString & input)
	{
		UPString commentless_input;
		size_t real_end = input.size();
		for(size_t cind = 0; cind < input.size(); cind++)
		{
			if(input[cind] == '#')
			{
				real_end = cind;
				break;
			}
		}

		commentless_input = input.substr(0, real_end);

		std::vector<std::string> str_vec;
		
		std::string built_string = "";

		bool has_escaped = false;
		bool in_quotes = false;

		// Use a linear search
		for(size_t ind = 0; ind < commentless_input.size(); ind++)
		{
			// If no escaping, then perform syntactical checks
			if(!has_escaped)
			{
				// First acknowledge escaping
				if(commentless_input[ind] == '\\')
				{
					has_escaped = true;
					continue;
				}

				// Detect quotations
				if(commentless_input[ind] == '\"' || commentless_input[ind] == '\'')
				{
					in_quotes = !in_quotes;
					continue;
				}

				// Now if not quoted as well, then a comma, whitespace, tab, or newline delineates that argument is done parsing
				if(!in_quotes)
				{
					if(commentless_input[ind] == ',' ||  commentless_input[ind] == ' ' || commentless_input[ind] == '\t' || commentless_input[ind] == '\n' || commentless_input[ind] == '\r')
					{
						// Check: do not add empty strings
						if(built_string != "")
						{
							str_vec.push_back(built_string);
							built_string = "";
						}

						continue;
					}
				}
			}

			built_string += commentless_input[ind];
			has_escaped = false; // no matter what, escaping only escapes one...
		}

		if(has_escaped || in_quotes)
		{
			throw priscas::mt_bad_escape();
		}

		if(built_string != "")
			str_vec.push_back(built_string);

		return str_vec;
	}

	// Set up list of runtime directives
	Shell::Shell() : motherboard(nullptr), isQuiet(false), inst_file(nullptr), tw_error(&priscas_io::null_tstream),
		tw_output(&priscas_io::null_tstream), tw_input(&priscas_io::null_tstream), NoConsoleOutput(false),
		hasAsmInput(false)
	{
		// Set up jump table for runtime directives
		this->directives.insert(directive_pair(".breakpoint", priscas::breakpoint));
		this->directives.insert(directive_pair(".cycle", priscas::cycle));
		this->directives.insert(directive_pair(".exit", priscas::exit));
		this->directives.insert(directive_pair(".help", priscas::help));
		this->directives.insert(directive_pair(".mem", priscas::mem));
		this->directives.insert(directive_pair(".pci", priscas::pci));
		this->directives.insert(directive_pair(".cpuopts", priscas::cpuopts));
		this->directives.insert(directive_pair(".power", priscas::power));
		this->directives.insert(directive_pair(".rst", priscas::rst));
		this->directives.insert(directive_pair(".run", priscas::run));
		this->directives.insert(directive_pair(".sr", priscas::sr));
		this->directives.insert(directive_pair(".sound", priscas::sound));
		this->directives.insert(directive_pair(".state", priscas::state));
		this->directives.insert(directive_pair(".trace", priscas::trace));
		this->directives.insert(directive_pair(".time", priscas::time));
		this->directives.insert(directive_pair(".vga", priscas::vga));
	}

	inline bool Shell::AsmFlash(const UPString& ains, mb& target, const BW& asm_pc)
	{
				Arg_Vec asm_args = chop_string(ains);
				diag_cpu & dcpu = dynamic_cast<diag_cpu&>(target.get_cpu());
				ISA& dcpuisa = dcpu.get_ISA();
				mBW inst;
				try
				{
					inst = dcpuisa.assemble(asm_args, asm_pc, jump_syms);
				}

				catch(priscas::mt_exception& e)
				{
					WriteToError("An error occurred while assembling the program.\n");
					UPString msg_1 = 
						UPString("Error information: ") + UPString(e.get_err());
					WriteToError(msg_1);
					WriteToError("Line of error:\n");
					UPString msg_2 = 
						UPString("\t") + ains + UPString("\n");
					WriteToError(msg_2);
					return false;
				}

				BW_32& thirty_two = dynamic_cast<BW_32&>(*inst);
				target.DMA_write(thirty_two.b_0(), asm_pc.AsUInt32());
				target.DMA_write(thirty_two.b_1(), asm_pc.AsUInt32() + 1);
				target.DMA_write(thirty_two.b_2(), asm_pc.AsUInt32() + 2);
				target.DMA_write(thirty_two.b_3(), asm_pc.AsUInt32() + 3);

				return true;
	}

	void Shell::declare_program_breakpoint(unsigned long line)
	{
		this->queued_prog_breakpoints.push(line);
	}

	void Shell::add_program_breakpoint(unsigned long line)
	{
		if(this->line_number_to_PC.count(line) > 0)
		{
			unsigned long line_pc = this->line_number_to_PC.at(line);
			this->program_breakpoints.insert(std::pair<unsigned long, unsigned long>(line_pc, line));
			std::string o = (std::string("Breakpoint set at line ") + priscas_io::StrTypes::UInt32ToStr(line) + std::string("d\n"));
			WriteToOutput(o);
		}

		else
		{
			WriteToError(("Invalid line number. The line number may not be associated with an instruction or may not be in the file.\n"));
		}
	}

	void Shell::WriteToError(std::string& e)
	{
		if(!NoConsoleOutput)
		{
			fprintf(stderr, e.c_str());
		}

		*tw_error << e;
	}

	void Shell::WriteToError(const char* e)
	{
		if(!NoConsoleOutput)
		{
			fprintf(stderr, e);
		}

		std::string err = std::string(e);

		*tw_error << err;

	}

	void Shell::WriteToOutput(std::string& o)
	{
		if(!isQuiet)
		{
			if(!NoConsoleOutput)
			{
				fprintf(stdout, o.c_str());
			}

			*tw_output << o;
		}
	}

	void Shell::WriteToOutput(const char* o)
	{
		std::string o_str(o);
		WriteToOutput(o_str);
	}

	const UPString& Shell::ReadFromInput()
	{
		this->rd_buffer.clear();

		bool QUOTES = false;
		bool ESCAPED = false;

		if(!NoConsoleOutput)
		{
			char in;
			while((in = fgetc(stdin)) != EOF)
			{
				if(in == '\\' && !QUOTES && !ESCAPED)
				{
					ESCAPED = true;
					continue;
				}

				if(in == '\"')
				{
					QUOTES = (!QUOTES);
				}

				if((in == '\n' || in == '\r' ) && !ESCAPED && !QUOTES)
				{
					this->rd_buffer += in;
					break;
				}

				this->rd_buffer += in;
				ESCAPED = false;
			}
		}


		do
		{
			*tw_input >> rd_buffer;
			priscas_osi::sleep(10);
		} while(rd_buffer == "" && shEnv.get_Mode() == Env::INTERACTIVE);

		return this->rd_buffer;
	}

	std::vector<priscas::NameValueStringPair> scan_for_values(const std::vector<std::string>& input)
	{
		std::vector<priscas::NameValueStringPair> vals;

		for(size_t sind = 0; sind < input.size(); sind++)
		{
			const std::string& indc = input[sind];
			size_t substr_where = indc.find_first_of('=');

			// Case: not found, then just use the whole string as the name, with an empty value
			if(substr_where == -1)
			{
				std::string first = input[sind];
				std::string second = "";
				priscas::NameValueStringPair nv(first, second);
				vals.push_back(nv);
			}

			// Case: empty name. This is an error
			else if(substr_where == 0)
			{
				throw priscas::mt_invalid_cpu_opt("Option specifier (lvalue of = may not be blank) requires name");
			}

			// Case: there is at least one equal sign
			else
			{
				// Use the first equal sign
				size_t last_where = indc.find_last_of('=');

				std::string first = indc.substr(0, substr_where);
				std::string second = indc.substr(substr_where + 1, indc.size());
				priscas::NameValueStringPair nv(first, second);
				vals.push_back(nv);
			}
		}

		return vals;
	}
}
