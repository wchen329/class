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
#ifndef __SHELL_H__
#define __SHELL_H__

#include <set>
#include <queue>
#include <map>
#include <string>
#include <vector>
#include "env.h"
#include "mb.h"
#include "primitives.h"
#include "priscas_global.h"
#include "priscas_osi.h"
#include "syms_table.h"
#include "streams.h"

namespace priscas
{
	/* Divides a string based on whitespace, tabs, commas and newlines
	 * Acknowledges escaping \ and quotes
	 */
	UPString_Vec chop_string(const UPString & input);

	/* Searches a vector of strings
	 * for value assignment pairs
	 * These consist of string with EXACT format:
	 * NAME=VALUE
	 */
	std::vector<priscas::NameValueStringPair> scan_for_values(const std::vector<std::string>& input);

	/* A single instance of a Shell
	 * -
	 * The shell allows easy and direct access to utilizing a processor.
	 */
	class Shell
	{

		public:
			LINK_DE void Run();
			LINK_DE void SetArgs(std::vector<std::string> & args) { this->args = args; }
			LINK_DE mb& GetMotherboard() { return *this->motherboard; } // Call this **after** Run
			LINK_DE void SetQuiet() { isQuiet = true; }
			LINK_DE Env::Mode_t modeget() { return shEnv.get_Mode(); }
			LINK_DE void modeset_Machine() { shEnv.update_Mode(Env::MACHINE); }
			LINK_DE void modeset_Interactive() { shEnv.update_Mode(Env::INTERACTIVE); }
			LINK_DE void modeset_Shutdown() { shEnv.update_Mode(Env::SHUTDOWN); }

			LINK_DE void WriteToOutput(std::string& o);
			LINK_DE void WriteToOutput(const char* e);
			LINK_DE void WriteToError(std::string& e);
			LINK_DE void WriteToError(const char* e);
			LINK_DE const UPString& ReadFromInput();

			LINK_DE void add_program_breakpoint(unsigned long line); // sets the program breakpoint directly, CANNOT be used be external sources (yet!)
			LINK_DE void declare_program_breakpoint(unsigned long line); // queues the breakpoint to be added, but doesn't set it
			LINK_DE void add_microarch_breakpoint(unsigned long cycle) { this->microarch_breakpoints.insert(std::pair<unsigned long, bool>(cycle, true)); }
			LINK_DE void setOutputTextStream(priscas_io::text_stream & ts) { this->tw_output = &ts; }
			LINK_DE void setErrorTextStream(priscas_io::text_stream & ts) { this->tw_error = &ts; }
			LINK_DE void setInputTextStream(priscas_io::text_stream & ts) { this->tw_input = &ts; }
			LINK_DE void setNoConsoleOutput(bool torf) { this->NoConsoleOutput = torf; }

			LINK_DE std::string getLineAtPC(unsigned long pc) {return this->PC_to_line_string.count(pc) > 0 ? this->PC_to_line_string[pc] : "???";}
			LINK_DE ~Shell() { delete motherboard; if(this->inst_file != nullptr) fclose(inst_file); }
			LINK_DE Shell();

		private:
			bool NoConsoleOutput;
			bool hasAsmInput;
			FILE* inst_file;
			priscas_io::text_stream * tw_error;
			priscas_io::text_stream * tw_output;
			priscas_io::text_stream * tw_input;
			std::string rd_buffer;
			Shell& operator=(const Shell&);
			Shell(const Shell&);
			std::vector<std::string> args;
			priscas::mb * motherboard;
			bool isQuiet;

			// Assembling facilities
			inline bool AsmFlash(const UPString& ains, mb& target, const BW& asm_pc); // macroop for assemble and flash; return true if success, false if not

			// The environment which the shell wraps around
			Env shEnv;

			// Runtime Directives, run through the shell
			std::map<unsigned long, unsigned long> program_breakpoints;
			std::map<unsigned long, unsigned long> PC_to_line_number;
			std::map<unsigned long, unsigned long> line_number_to_PC;
			std::map<unsigned long, std::string> PC_to_line_string;
			std::map<unsigned long, bool> microarch_breakpoints;
			std::queue<unsigned long> queued_prog_breakpoints;
			std::map<std::string, void(*)(const Arg_Vec&, Shell& shell)> directives;
			priscas::syms_table jump_syms;
			priscas::mono_syms_table directive_syms;
			void execute_runtime_directive(std::vector<std::string>& args_list);
			bool has_ma_break_at(unsigned long line){ return this->microarch_breakpoints.count(line) > 0; }
			bool has_prog_break_at(unsigned long line){ return this->program_breakpoints.count(line) > 0; }
	};
}

#endif
