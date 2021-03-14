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
#ifndef __SHELL_H__
#define __SHELL_H__

#include <set>
#include <queue>
#include <map>
#include <string>
#include <vector>
#include "branding.h"
#include "env.h"
#include "ISA.h"
#include "mips.h"
#include "mmem.h"
#include "mtsstream.h"
#include "primitives.h"
#include "priscas_global.h"
#include "priscas_osi.h"
#include "program.h"
#include "syms_table.h"
#include "streams.h"

namespace priscas
{
	class Shell;
}
#include "runtime_call.h"

namespace priscas
{

	/* A mapping from a string into a directive function pointer (must be a member of Shell)
	 */
	typedef std::pair<std::string, void(*)(const Arg_Vec &, Shell&)> directive_pair;

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
			virtual void Run();
			void SetArgs(std::vector<std::string> & args) { this->args = args; }
			void SetQuiet() { isQuiet = true; }
			Env::Mode_t modeget() { return shEnv.get_Mode(); }
			void modeset_Machine() { shEnv.update_Mode(Env::MACHINE); }
			void modeset_Interactive() { shEnv.update_Mode(Env::INTERACTIVE); }
			void modeset_Shutdown() { shEnv.update_Mode(Env::SHUTDOWN); }

			void WriteToOutput(std::string& o);
			void WriteToOutput(const char* e);
			void WriteToError(std::string& e);
			void WriteToError(const char* e);
			const UPString& ReadFromInput();

			void add_program_breakpoint(unsigned long line); // sets the program breakpoint directly, CANNOT be used be external sources (yet!)
			void declare_program_breakpoint(unsigned long line); // queues the breakpoint to be added, but doesn't set it
			void add_microarch_breakpoint(unsigned long cycle) { this->microarch_breakpoints.insert(std::pair<unsigned long, bool>(cycle, true)); }
			void setOutputTextStream(priscas_io::text_stream & ts) { this->tw_output = &ts; }
			void setErrorTextStream(priscas_io::text_stream & ts) { this->tw_error = &ts; }
			void setInputTextStream(priscas_io::text_stream & ts) { this->tw_input = &ts; }
			void setNoConsoleOutput(bool torf) { this->NoConsoleOutput = torf; }

			mmem& Mem() { return m_zone; }
			std::string getLineAtPC(unsigned long pc) {return this->PC_to_line_string.count(pc) > 0 ? this->PC_to_line_string[pc] : "???";}
			~Shell() { if(this->inst_file != nullptr) fclose(inst_file); }
			Shell();
		protected:
			void execute_runtime_directive(std::vector<std::string>& args_list);
			std::map<std::string, void(*)(const Arg_Vec&, Shell& shell)> directives;
			std::vector<std::string> args;


			// The environment which the shell wraps around
			Env shEnv;

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
			bool isQuiet;

			// Assembling facilities
			inline bool AsmFlash(const UPString& ains, const BW& asm_pc, ISA& isa, uint64_t& bytecount); // macroop for assemble and flash; return true if success, false if not
			void trim_label(UPString& strin);


			// Runtime Directives, run through the shell
			std::map<unsigned long, unsigned long> program_breakpoints;
			std::map<unsigned long, unsigned long> PC_to_line_number;
			std::map<unsigned long, unsigned long> line_number_to_PC;
			std::map<unsigned long, std::string> PC_to_line_string;
			std::map<unsigned long, bool> microarch_breakpoints;
			std::queue<unsigned long> queued_prog_breakpoints;
			priscas::syms_table jump_syms;
			priscas::mono_syms_table directive_syms;
			bool has_ma_break_at(unsigned long line){ return this->microarch_breakpoints.count(line) > 0; }
			bool has_prog_break_at(unsigned long line){ return this->program_breakpoints.count(line) > 0; }

			Program prog;

			// Loader stuff
			mmem m_zone;
	};
}

#endif
