//////////////////////////////////////////////////////////////////////////////
//
//    CLASS - Cloud Loader and ASsembler System
//    Copyright (C) 2021 Winor Chen
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
#include <iostream>
#include <string>
#include <memory>
#include "shell.h"

namespace priscas
{

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
			WriteToOutput("Usage:\n");
			WriteToOutput("\t{-i i_filename.s} - assemble file of name i_filename.s\n");
			WriteToOutput("\t{-o o_filename.bin} - dump assembly output to file of name o_filename.bin \n");
			WriteToOutput("\t{-s bin OR hex OR mif} - specify output format as bin, hex, or MIF\n");
			WriteToOutput("\t{-b width - for MIF out, specify the word size of the output (default is the size of the instruction) in bytes}\n");
			return;
		}


		// Other predicates to check for
		if(shEnv.get_Option_AsmOutput())
		{
			if(!shEnv.get_Option_AsmOutputSpecified())
			{
				WriteToError("Error: -o requires a filename to be specified.\n");
				return;
			}
		}

		if(shEnv.get_Option_AsmStrMode())
		{
			if(!shEnv.get_Option_AsmStrModeSpecified())
			{
				WriteToError("Error: -s requires one of: {bin, hex, mif} as an arguments.\n");
				return;
			}
		}

		if(shEnv.get_Option_WordSize())
		{
			if(!shEnv.get_Option_WordSizeSpecified())
			{
				WriteToError("Error: -b requires an integer greater than 0 as its argument\n");
				return;
			}
		}

		/* Actual Execution Portion
		 * This is a "double-pass" assembler
		 * First run through: read symbols and map them with a symbol table
		 * Second run through: do the actual assembling
		 */

		std::vector<size_t> inst_lengths;

		/* First, if an input file was specified
		 * (1) collect file symbols
		 * (2) map it to memory assemble that file first
		 *
		 */
		if(shEnv.get_Option_AsmInputSpecified())
		{

			UPString_Vec lines;
			uint32_t equiv_pc = 0;
			BW_64 asm_pc = 0;

			char input_f_stream[256];
			memset(input_f_stream, 0, sizeof(input_f_stream));
			unsigned long line_number = 0;
			try
			{
				while(fgets(input_f_stream, 255, inst_file) != NULL)
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

						// If the label is the only declaration on this line, continue
						if(parts.size() == 1)
						{
							continue;
						}

						// Otherwise assemble this instruction too
						trim_label(current_line);
					}

					// Strip newlines at the back
					current_line.pop_back();

					this->line_number_to_PC.insert(std::pair<unsigned long, unsigned long>(line_number, equiv_pc));
					this->PC_to_line_number.insert(std::pair<unsigned long, unsigned long>(equiv_pc, line_number));
					this->PC_to_line_string.insert((std::pair<unsigned long, std::string>(equiv_pc, current_line)));
					equiv_pc = equiv_pc + 4;
					lines.push_back(current_line);		
				}
			}
			catch(mt_exception& e)
			{
				WriteToOutput("An error has occurred when writing symbols table:\n\t");
				WriteToOutput(e.get_err().c_str()); WriteToOutput(priscas_io::newLine.c_str());
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
				// Specify the ISA to use
				MIPS_32 theISA;
				uint64_t bytecount;

				if(!this->AsmFlash(lines[itr], asm_pc, theISA, bytecount))
				{
					return;
				}

				// Increment the PC at which to flash
				asm_pc = asm_pc.AsUInt64() + bytecount;

				inst_lengths.push_back(bytecount);
			}
			
		}


		try
		{
			// Write this to program, or remain in memory

			UPString default_name;
			switch(shEnv.get_AsmStrMode())
			{
				case asm_ostream::MIF:
					default_name = "a.mif";
					break;
				case asm_ostream::HEX:
					default_name = "a.hex";
					break;
				default:
					default_name = "a.bin";
			}


			UPString fname(default_name);

			if(shEnv.get_Option_AsmOutput())
			{
				fname = shEnv.get_OutputFilename();
			}

			// Dump the output
			asm_ostream prg_o(fname.c_str(), shEnv.get_AsmStrMode());
			prg_o.set_width(shEnv.get_Wordsize());

			uint64_t ind = 0;
			uint64_t addr = 0;
			while(addr < prog.get_EOP())
			{
				size_t bt = inst_lengths[ind];

				byte_8b buf[1024];

				if(bt > 1024)
				{
					fprintf(stderr, "FATAL: instruction size too large, must be 1024 bytes or less");
					abort();
				}

				for(size_t sz = 0; sz < bt; ++sz)
				{
					buf[sz] = prog.read(addr + sz); 
				}

				prg_o.append(buf, bt);

				// Increments
				++ind;
				addr += bt;
			}
			
			prg_o.finalize();

			fprintf(stdout, "Operation completed succesfully\n");
		}
		catch(mt_exception& mte)
		{
			WriteToOutput("An error has occurred:\n");
			WriteToOutput(mte.get_err().c_str()); WriteToOutput(priscas_io::newLine.c_str());
		}
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
			if(input[cind] == ';')
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

	Shell::Shell() :
		isQuiet(false),
		inst_file(nullptr), tw_error(&priscas_io::null_tstream),
		tw_output(&priscas_io::null_tstream),
		tw_input(&priscas_io::null_tstream),
		NoConsoleOutput(false),
		hasAsmInput(false)
	{
	}

	inline bool Shell::AsmFlash(const UPString& ains, const BW& asm_pc, ISA& isain, uint64_t& byte_count)
	{
				Arg_Vec asm_args = chop_string(ains);
				mBW inst;
				try
				{
					// Here, we write the instruction to memory
					inst = isain.assemble(asm_args, asm_pc, jump_syms);
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

				// Write the instruction to the program

				// If big endian, we must reverse (since we are on an intel host)
				if(isain.get_endian() == ISA_Attrib::CPU_BIG_ENDIAN)
				{
					inst->reverse_endian();
				}

				// Now, we can write
				byte_count = inst->get_bitcount() / 8;

				for(size_t bind = 0; bind < byte_count; ++bind)
				{
					prog.write_next(inst->get_nth_byte(bind));
				}

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
			std::cerr << e;
		}

		*tw_error << e;
	}

	void Shell::WriteToError(const char* e)
	{
		if(!NoConsoleOutput)
		{
			std::cerr << std::string(e);
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
				std::cout << o;
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

	void Shell::trim_label(UPString& strin)
	{
		UPString snew;

		bool foundcol = false;

		for(ptrdiff_t sc = 0; sc < strin.size(); ++sc)
		{
			if(!foundcol)
			{
				if(strin[sc] == ':')
				{
					foundcol = true;
				}

				continue;
			}

			else
			{
				snew += strin[sc];
			}
		}

		strin = snew;
	}

}
