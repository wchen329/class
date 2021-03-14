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
#include "shell_cload.h"

namespace priscas
{

	void Shell_Cload::Run()
	{
		// Set shell enviornment
		Shell::shEnv.characterize_Env(Shell::args);

		UPString app_brand = branding::APPLICATION_NAME + " " +
				branding::APPLICATION_VERSION + " " +
				branding::APPLICATION_DBG;

		UPString header = app_brand + priscas_io::newLine + UPString("[loader]") +
			priscas_io::newLine;

		FILE* fscript = nullptr;

		Shell::WriteToOutput(header);

		// Check for input script.
		if(Shell::shEnv.get_Option_AsmInput())
		{
			if(!Shell::shEnv.get_Option_AsmInputSpecified())
			{
				fprintf(stdout, "Error: Option -i requires an argument.\n");
				return;
			}
			else
			{
				fscript = fopen(shEnv.get_asmFilenames()[0].c_str(), "r");

				if(fscript == nullptr)
				{
					fprintf(stdout, "Error: instruction file (through -i) could not be opened for reading.\n");
					return;
					
				}
			}
		}

		// Set memory mode.
		WriteToOutput("Allocating memory resources...\n");
		uint64_t mem_size = 64*MB;
		Mem().resize(mem_size);
		UPString mem_msg = UPString("Memory Size: ") +
		priscas_io::StrTypes::UInt64ToStr(mem_size) + (" bytes\n");
		WriteToOutput(mem_msg);

		// Invoke input script if possible.
		if(fscript != nullptr)
		{
			const size_t MAX_SIZE = 256;
			char input_f_stream[MAX_SIZE];
			memset(input_f_stream, 0, sizeof(input_f_stream));
			while(fgets(input_f_stream, MAX_SIZE - 1, fscript) != nullptr)
			{
				UPString val = UPString(input_f_stream);

				if(val.size() == 0)
				{
					continue;
				}

				else if(val[0] == '.')
				{
					try
					{
						UPString_Vec chopped = chop_string(val);
						Shell::execute_runtime_directive(chopped);
					}
			
					catch(priscas::mt_exception & e)
					{
						WriteToError(e.get_err());
					}
				}
				else
				{
					UPString_Vec args;
					args.push_back(".help");
					Shell::execute_runtime_directive(args);
				}
			}

			fclose(fscript);

			if(Shell::modeget() != Env::SHUTDOWN)
			{
				Shell::modeset_Interactive();
			}
		}

		// Get ready for interactive mode

		while(Shell::modeget() == Env::INTERACTIVE || Shell::modeget() == Env::MACHINE)
		{

			if(Shell::modeget() != Env::MACHINE)
			{
				Shell::WriteToOutput(">> ");

				const UPString& val = this->ReadFromInput();
			

				if(val.size() == 0)
				{
					continue;
				}

				else if(val[0] == '.')
				{
					try
					{
						UPString_Vec chopped = chop_string(val);
						Shell::execute_runtime_directive(chopped);
					}
			
					catch(priscas::mt_exception & e)
					{
						WriteToError(e.get_err());
					}
				}
				else
				{
					UPString_Vec args;
					args.push_back(".help");
					Shell::execute_runtime_directive(args);
				}

			}
		}

	}


	// Set up list of runtime directives
	Shell_Cload::Shell_Cload() :
		Shell()
	{
		// Set up jump table for runtime directives
		Shell::directives.insert(directive_pair(".help", priscas::help_loader));
		Shell::directives.insert(directive_pair(".rst", priscas::reset));
		Shell::directives.insert(directive_pair(".sr", priscas::sr));
		Shell::directives.insert(directive_pair(".mem", priscas::mem));
		Shell::directives.insert(directive_pair(".resize", priscas::resize));
		Shell::directives.insert(directive_pair(".wait", priscas::wait));
	}
}
