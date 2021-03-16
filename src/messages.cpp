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
#include "messages.h"

/* Various command line messages
 *
 * CURRENTLY: placeholders, but will be replaced by file I/O based on localization
 */
namespace priscas
{
	const std::string HELP =
								std::string(".exit - exit\n") +
								std::string(".help - show this help message or get help with commands\n") +
								std::string(".rst - reset CPU state and mem state\n") +
								std::string(".sr - pack files into main memory, and create files from main memory\n") +
								std::string(".mem - show main memory size, or take a peek at memory\n") +
								std::string(".wait - wait on a condition before performing the next option\n") +
								std::string(".resize - resize the available host memory\n") +
								std::string(".run - enable memory controller\n") +
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
									std::string("[no args] - reset both the CPU state and mem state.\n") +
									std::string("-afu: reset the CPU state\n") +
									std::string("-host: reset the host memory to all zeroes\n");
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

	const UPString HELP_RUN = UPString(".run\n") + UPString("When the AFU first boots up, the memory controller is blocked from\n") +
		UPString("performing any operations, to prevent it from corrupting host memory.\n") + UPString("When you are done loading your instruction code with .sr\n") +
		UPString("use .run to unblock the memory controller, allowed the CPU to fetch instructions.\n");

	const UPString HELP_SR = UPString(".sr\n") +
		UPString("[no args] - doesn't do anything\n") +
		UPString("-r pathtofile [-bige] address: flattens file, beginning at address\n\tSpecify bige to interpret as big endian.\n") +
		UPString("-s pathtofile [-bige] begin,end: saves memory bytes from begin to end into file.\n\tSpecify bige to interpret as big endian.\n");

	const UPString HELP_WAIT = UPString(".wait\n") +
		UPString("Usage: .wait spin_address value_to_look_for [timeout]\n") +
		UPString(".wait causes the program to spin on \"spin_address\" until it reads \"value_to_look_for\" from the spin address.\n") +
		UPString("It polls the spin address location approximately every millisecond, until it reads the value it is looking for, or\n") +
		UPString("\"timeout\" milliseconds is reached. The timeout argument is optional and has a default value of 1 second. A user can specify their\n") +
		UPString("own timeout in milliseconds. If the user passes a timeout of 0, \".wait\" will run until the right value is read,\n") +
		UPString("until interrupted through SIGINT, or cload is killed/aborted\n") +
		UPString("\nExample: .wait 0 100 10; wait 10 ms for the value at address 0 to become 100\n");

	const UPString HELP_RESIZE = UPString(".resize\n") +
		UPString("Usage: .resize numberofbits\n") +
		UPString("Resizes the pool of addressable memory on the host to be 2^numberofbits\n") +
		UPString("Example: .resize 20; Give me 1MB of addressable memory on the host\n");
}
