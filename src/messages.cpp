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
#include "messages.h"

/* Various command line messages
 *
 * CURRENTLY: placeholders, but will be replaced by file I/O based on localization
 */
namespace priscas
{
	const std::string HELP =	std::string(".breakpoint - set breakpoints which will bring up the interactive prompt, that can be used for debugging\n") +
								std::string(".cpuopts - show special CPU options or execute one\n") +
								std::string(".cycle - advance the processor a cycle given the current PC\n") +
								std::string(".exit - exit\n") +
								std::string(".help - show this help message or get help with commands\n") +
								std::string(".rst - reset CPU state and mem state\n") +
								std::string(".sr - save and restore main memory\n") +
								std::string(".state - echo register state\n") +
								std::string(".mem - show main memory size, or take a peek at memory\n") +
								std::string(".run - continue running the processor (as in unattended mode)\n") +
								std::string(".time - show simulation timing information\n") +
								std::string("[assembly instruction] - execute that instruction, write it to the PC location and cycle\n") +
								std::string("For more help on specific commands use \"help [command]\" (for example .help .rst)\n");
	const char * BAD_COMMAND = "Unknown command. For more information on acceptable commands type \".help\".\n" ;  // bad command message
	const char * BAD_FORMAT = "Bad or unsupported register format.\n";
	const char * NON_DIAG_CPU = "Warning: The CPU used does not have diagnostic capabilities, such CPUs are unable to receive fetches from interactive mode...\n";
	const std::string HELP_BREAKPOINT =	std::string(".breakpoint\n") +
										std::string("Insert a breakpoint into the program corresponding to a line number or a cycle\n") +
										std::string("Execution will be stopped before the corresponding cycle has passed in execution.\n") +
										std::string("Options:\n") +
										std::string("-l [line_number]: insert a breakpoint at a line number. This can only used with an input file specified\n") +
										std::string("-c [cycle number]: break when a certain cycle count is reached (but not yet executed)\n");
	const std::string HELP_CPUOPTS =	std::string(".cpuopts\n") +
									std::string("If no arguments are specified then a list of cpu options will appear.\n") +
									std::string("To execute a cpu option, give the option flag corresponding to a cpu\n") +
									std::string("option as an argument to .cpuopts\n");
	const std::string HELP_CYCLE =	std::string(".cycle\n") +
									std::string("Step the processor a cycle. Effectively, this will turn on the CPU for a cycle and then\n") +
									std::string("break the processor after the cycle returning to the interactive prompt.\n");

	const std::string HELP_EXIT =	std::string(".exit\n") +
									std::string("Use this to exit program...\n");

	const std::string HELP_HELP =	std::string(".help\n") +
									std::string("It's... well, how one gets help in this application.\n") +
									std::string("For more information, just use .help without specifying any arguments.\n");

	const std::string HELP_MEM =	std::string(".mem\n") +
									std::string("With no args, .mem will only print out the size of main memory\n") +
									std::string("However a single memory address can be printed with a single unsigned integer argument:\n") +
									std::string(".mem 0 # print mem[0]\n") +
									std::string("Or one or more ranges can be specified using the syntax begin:end. Both ends are inclusive:\n") +
									std::string(".mem 0:2 # will print mem[0], mem[1], mem[2]\n") +
									std::string("You can also use the syntax begin:end:step:\n") +
									std::string(".mem 0:4:2 # will print states of mem[0], mem[2], mem[4]\n");
	const std::string HELP_RST =	std::string(".rst\n") +
									std::string("Use this to reset certain components of the machine\n") +
									std::string("Options:\n") +
									std::string("[no args] - reset both the CPU state and mem state. This is the same as -a (see below).\n") +
									std::string("-c: reset the CPU state\n") +
									std::string("-a: reset both the CPU state and mem state\n");
	const std::string HELP_RUN =	std::string(".run\n") +
									std::string("Using .run will cycle the CPU continuously without\n") +
									std::string("bringing up the interactive prompt. It's akin to\n") +
									std::string("running the CPU for real, or in unattended mode.\n") +
									std::string("In the CLI shell, control can be returned to the\n") +
									std::string("interactive shell through CTRL+C. In general, setting\n") +
									std::string("a breakpoint will also return control to the\n") +
									std::string("interactive shell.\n");
	const std::string HELP_SR =		std::string(".sr\n") +
									std::string("Save or restore main memory.\n") +
									std::string("To use, specify either save or restore mode and then\n") +
									std::string("a filename (see below for options). A memory image will be saved to\n") +
									std::string("or loaded from the given image.\n") +
									std::string("Options:\n") +
									std::string("-s (specify save mode)\n") +
									std::string("-r (specify restore mode)\n") +
									std::string("-f [FILENAME] (specify an input file)\n") +
									std::string("Example: .sr -s -f file.img # save main memory to file.img\n");
	const std::string HELP_STATE =	std::string(".state\n") +
									std::string("Print register state information.\n") +
									std::string("By default the state of every single register will be printed.\n") +
									std::string("The state of an individual register can be printed by specifying its register number as an argument:\n") +
									std::string(".state 0 # print register 0 (in MIPS $0)\n") +
									std::string("Or one or more ranges can be specified using the syntax begin:end. Both ends are inclusive:\n") +
									std::string(".state 0:2 # will print states of registers 0, 1, 2\n") +
									std::string("You can also use the syntax begin:end:step:\n") +
									std::string(".state 0:4:2 # will print states of registers 0, 2, 4\n");
	const std::string HELP_TIME =	std::string(".time\n") +
									std::string("Print out information about timing information\n");

}
