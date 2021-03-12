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

#include "runtime_call.h"

namespace priscas
{

	void Shell::execute_runtime_directive(std::vector<std::string>& args_list)
	{
		if(args_list.size() < 1)
		{
			throw priscas::mt_insuff_arg();
		}
		// Check first argument, and pass the rest as args for that call
		
		if(this->directives.count(args_list[0]) > 0)
		{
			directives.at(args_list[0])(args_list, *this);
		}
		
		else
		{
			if(args_list.size() > 0)
			{
				std::string put1 = (std::string("The specified runtime directive could not be ran. It may not exist.\n"));
				std::string put2 = (std::string("\tCould not run: ") + args_list[0] + std::string("\n"));
				WriteToOutput(put1); WriteToOutput(put2);
			}
		}
	}

	void breakpoint(const Arg_Vec & args, Shell& inst)
	{
		inst.WriteToOutput(("[Breakpoint]\n"));
		inst.WriteToOutput("Not yet implemented.\n");
	}

	void exit(const Arg_Vec & args, Shell& inst)
	{
		inst.modeset_Shutdown();
	}

	void help(const Arg_Vec & args, Shell& inst)
	{
		inst.WriteToOutput(("[Help]\n"));

		if(args.size() <= 1)
		{
			inst.WriteToOutput(HELP.c_str());
			return;
		}

		else
		{
			std::string msg = "Unknown help topic. The command may not exist, or may be undocumented.\n";

			if(args[1] == ".breakpoint")
			{
				msg = HELP_BREAKPOINT;
			}

			else if(args[1] == ".exit")
			{
				msg = HELP_EXIT;
			}
			
			else if(args[1] == ".help")
			{
				msg = HELP_HELP;
			}

			inst.WriteToOutput(msg.c_str());
		}
	}

	void help_loader(const Arg_Vec & args, Shell& inst)
	{
		inst.WriteToOutput(("[Help]\n"));

		if(args.size() <= 1)
		{
			inst.WriteToOutput(HELP.c_str());
			return;
		}

		else
		{
			std::string msg = "Unknown help topic. The command may not exist, or may be undocumented.\n";

			if(args[1] == ".breakpoint")
			{
				msg = HELP_BREAKPOINT;
			}

			else if(args[1] == ".exit")
			{
				msg = HELP_EXIT;
			}
			
			else if(args[1] == ".help")
			{
				msg = HELP_HELP;
			}
			else if(args[1] == ".rst")
			{
				msg = HELP_RST;
			}

			inst.WriteToOutput(msg.c_str());
		}
	}

	void reset(const Arg_Vec & args, Shell& inst)
	{
	}
}
