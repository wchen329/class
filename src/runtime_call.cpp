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

#include "runtime_call.h"

namespace priscas
{

	void Shell_Cload::execute_runtime_directive(std::vector<std::string>& args_list)
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

	void breakpoint(const Arg_Vec & args, Shell_Cload& inst)
	{
		inst.WriteToOutput(("[Breakpoint]\n"));
		inst.WriteToOutput("Not yet implemented.\n");
	}

	void exit(const Arg_Vec & args, Shell_Cload& inst)
	{
		inst.modeset_Shutdown();
	}

	void help(const Arg_Vec & args, Shell_Cload& inst)
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

	void help_loader(const Arg_Vec & args, Shell_Cload& inst)
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
			else if(args[1] == ".sr")
			{
				msg = HELP_SR;
			}
			else if(args[1] == ".mem")
			{
				msg = HELP_MEM;
			}
			else if(args[1] == ".wait")
			{
				msg = HELP_WAIT;
			}
			else if(args[1] == ".resize")
			{
				msg = HELP_RESIZE;
			}

			else if(args[1] == ".run")
			{
				msg = HELP_RUN;
			}

			inst.WriteToOutput(msg.c_str());
		}
	}

	void reset(const Arg_Vec & args, Shell_Cload& inst)
	{
		bool reset_afu = false;
		bool reset_host = false;

		if(args.size() <= 1)
		{
			reset_afu = true;
			reset_host = true;
		}
		else
		{
			if(args[1] == "-afu")
			{
				reset_afu = true;
			}
			else if(args[1] == "-host")
			{
				reset_host = true;
			}
		}

		//
		if(!reset_afu && !reset_host)
		{
			inst.WriteToOutput("No reset performed.\n");
			inst.WriteToOutput("Valid arguments for .rst include -afu and -host\n");
			return;
		}

		// Perform resets.
		if(reset_afu)
		{
			//inst.WriteToOutput("AFU Reset [Not implemented].\n");
			//inst.AFU_Reset();
		}

		if(reset_host)
		{
			inst.Mem().reset();
			inst.WriteToOutput("Host Memory Reset.\n");
		}
	}

	void sr(const Arg_Vec& args, Shell_Cload& inst)
	{
		if(args.size() <= 1)
		{
			inst.WriteToError("Invalid Usage, please refer to online help on how to use this sr.\n");
			return;
		}

		if(args[1] == "-r")
		{
			if(args.size() < 4)
			{
				inst.WriteToError("-r: not enough arguments\n");
				return;
			}
				
			// Read file in
			FILE* f = fopen(args[2].c_str(), "r");

			if(!f)
			{
				inst.WriteToError("-r: source file could not be opened\n");
				return;
			}

			int iind = 3;

			uint64_t addr = 0;
			try
			{
				UPString begin_addr = args[iind];
				addr = StrOp::StrToUInt64(begin_addr);
			}
			catch(mt_exception& e)
			{
				inst.WriteToError("Error when parsing begin address, the address was likely malformed.\n");
				inst.WriteToError(e.get_err());
				fclose(f);
				return;
			}

			UPString str_addr = genericHexBuilder<uint64_t, 64>(addr);

			// Flatten the file opened
			inst.Mem().restore(addr, f);

			UPString head = "Squashed file beginning @ address: ";
			UPString msg = head + str_addr + priscas_io::newLine;
			inst.WriteToOutput(msg);
				
			fclose(f);
			return;
		}
		else if(args[1] == "-s")
		{

			if(args.size() < 5)
			{
				inst.WriteToError("-s: not enough arguments\n");
				return;
			}
				
			// Read file in
			FILE* f = fopen(args[2].c_str(), "w");

			if(!f)
			{
				inst.WriteToError("-s: error opening target file\n");
				return;
			}

			int iind = 3;

			uint64_t addr_b = 0;
			uint64_t addr_e = 0;
			try
			{
				UPString begin_addr = args[iind];
				UPString end_addr = args[iind + 1];
				addr_b = StrOp::StrToUInt64(begin_addr);
				addr_e = StrOp::StrToUInt64(end_addr);
			}
			catch(mt_exception& e)
			{
				inst.WriteToError("Error when parsing begin or end address, at least one of the addresses was likely malformed.\n");
				inst.WriteToError(e.get_err());
				fclose(f);
				return;
			}

			UPString str_addr_b = genericHexBuilder<uint64_t, 64>(addr_b);
			UPString str_addr_e = genericHexBuilder<uint64_t, 64>(addr_e);

			// Flatten the file opened
			inst.Mem().save(addr_b, addr_e, f);

			UPString head = "Wrote file with data beginning @ address: ";
			UPString tail = ", ending @ address ";
			UPString msg = head + str_addr_b + tail + str_addr_e + priscas_io::newLine;
			inst.WriteToOutput(msg);

			fclose(f);
			return;
		}

		else
		{
			inst.WriteToOutput("No operation performed. Expecting -r or -s\n");
			return;
		}
	}

	void mem(const Arg_Vec & args, Shell_Cload& inst)
	{
		inst.WriteToOutput(("[Memory Information]\n"));

		mmem& cmp = inst.Mem();

		// No args specified just print the memory size (in bytes)
		if(args.size() <= 1)
		{
			std::string o = (std::string("Main memory Size: " + priscas_io::StrTypes::SizeToStr(cmp.get_size()) + std::string(" bytes\n")));
			inst.WriteToOutput(o);
			return;
		}

		size_t first = 1;
		bool hexOutput = false;
		bool asciiOutput = false;
		size_t output_byte_size = 1;
		for(size_t aitr = 1; aitr < args.size(); ++aitr)
		{
			if(args[aitr] == "-h")
			{
				hexOutput = true;
				first += 1;
			}

			if(args[aitr] == "-a")
			{
				asciiOutput = true;
				first += 1;
			}

			if(args[aitr] == "-b")
			{
				if(args.size() <= aitr + 1)
				{
					inst.WriteToError("Error: -b requires an argument\n");
					return;
				}

				output_byte_size = atoi(args[aitr + 1].c_str());

				if(output_byte_size != 2 && output_byte_size != 4
					&& output_byte_size != 8
				)
				{
					inst.WriteToError("Error: -b can only take in the following values: (1, 2, 4, 8).\n");
					return;
				}

				first += 2;
			}
		}

		if(asciiOutput && output_byte_size != 1)
		{
			inst.WriteToError("Error: -a and -b {2, 4, 8} are incompatible with each other. Specify either but not both\n");
			return;
		}

		// Otherwise print memory specific to indicies
		for(size_t itr = first; itr < args.size(); itr++)
		{
			priscas::range r = priscas::range(args[itr]);

			while(!r.atEnd())
			{
				size_t itr_2 = r.next();
				if(itr_2 >= inst.Mem().get_size() || itr_2 < 0 ||
					(output_byte_size == 2 && itr_2 + 2 >= inst.Mem().get_size()) ||
					(output_byte_size == 4 && itr_2 + 4 >= inst.Mem().get_size()) ||
					(output_byte_size == 8 && itr_2 + 8 >= inst.Mem().get_size())
				)
				{
					throw priscas::mem_oob_exception();
				}

				UPString index_str = hexOutput || asciiOutput ? genericHexBuilder<uint32_t, 32>(itr_2) :  priscas_io::StrTypes::SizeToStr(itr_2);
				UPString val_str;
				if(asciiOutput)
				{
					val_str = UPString("\'") + (UPString() + static_cast<char>(inst.Mem()[itr_2])) + UPString("\'"); 
				}
				else
				{

#define MEM_VALUE_BUILDER(OTYPE, OBITS) \
	OTYPE word = 0; uint8_t* word_ptr = reinterpret_cast<uint8_t*>(&word); \
	for(size_t itr_M = 0; itr_M < sizeof(OTYPE); ++itr_M) \
	{ \
		word_ptr[itr_M] = inst.Mem()[itr_2 + itr_M]; \
	} \
	val_str = hexOutput ? genericHexBuilder<OTYPE, OBITS>(word) : priscas_io::StrTypes::Int64ToStr(word);

					// todo genericize
					switch(output_byte_size)
					{
						case 1:
							val_str = hexOutput ? genericHexBuilder<uint8_t, 8>(inst.Mem()[itr_2]) : priscas_io::StrTypes::IntToStr(inst.Mem()[itr_2]);
						break;

						case 2:
						{
							MEM_VALUE_BUILDER(int16_t, 16);
						}
						break;

						case 4:
						{
							MEM_VALUE_BUILDER(int32_t, 32);
						}
						break;

						case 8:
						{
							MEM_VALUE_BUILDER(int64_t, 64);
						}
						break;
					}
				}

				std::string o = (std::string("Mem[") + index_str + std::string("]: ") + 
					val_str + priscas_io::newLine);
				inst.WriteToOutput(o);
			}
		}
	}

	void wait(const Arg_Vec& args, Shell_Cload& inst)
	{
		// This sleeps until a timeout or a condition in shared memory
		// with the FPGA is met
		inst.WriteToOutput("[Wait]\nWaiting...\n");
		uint64_t numms = 0;
		uint64_t timeout = 1000;
		uint64_t address_polling = 0;
		uint8_t value_wait = 0;
		bool mmio_wait = false;
		uint64_t value_wait_mmio = 0;

		if(args.size() < 3)
		{
			inst.WriteToOutput("Insufficient arguments for wait. Stopping...\n");
			return;
		}

		if(args.size() >= 3)
		{
			// Arg 0: .wait
			// Arg 1: address polling
			// Arg 2: value to wait for
			if(args[1] == "-m")
			{
				mmio_wait = true;
			}
			else
			{
				address_polling = StrOp::StrToUInt64(args[1]);
			}
			value_wait = StrOp::StrToUInt32(args[2]);
			value_wait_mmio = StrOp::StrToUInt64(args[2]);
		}
		if(args.size() >= 4)
		{
			// Arg 3: timeout
			timeout = StrOp::StrToUInt64(args[3]);
		}

		// Normalize address
		address_polling = address_polling % inst.Mem().get_size();

		inst.modeset_Machine();

		// This is implemented through polling every millisecond, approximate
		while(inst.modeget() == Env::MACHINE && (numms < timeout || timeout == 0))
		{
			priscas_osi::sleep(1);

			if(mmio_wait)
			{
				if(inst.AFU_getCV() == value_wait)
				{
					break;
				}
			}
			else
			{
				// Check the corresponding memory address. If it is "val"
				// then break.
				if(inst.Mem()[address_polling] == value_wait)
				{
					break;
				}
			}
			
			++numms;
		}

		if(numms == timeout && timeout != 0)
		{
			inst.WriteToOutput("Wait Timed out.\n");
		}
		else if(inst.modeget() != Env::MACHINE)
		{
			inst.WriteToOutput("User aborted wait.\n");
		}
		else
		{
			inst.WriteToOutput("Wait observed a state change which allowed it to continue.\n");
		}

		inst.modeset_Interactive();
	}

	void resize(const Arg_Vec& args, Shell_Cload& inst)
	{
		inst.WriteToOutput(".resize\n");
		// Take the mem and resize it to an args[1] size address space.
		if(args.size() < 2)
		{
			inst.WriteToOutput("Usage: .resize numberofbits\n");
			return;
		}

		uint64_t bitcount = StrOp::StrToUInt32(args[1]);

		if(bitcount > 32)
		{
			inst.WriteToOutput("Too large of an address space specified.\n");
			inst.WriteToOutput("No resizing done (max size 4GB [32-bit])\n");
		}
		else if(bitcount < 16)
		{
			inst.WriteToOutput("Too small of an address space specified.\n");
			inst.WriteToOutput("No resizing done (min size 64KB [16-bit]).\n");
		}
		else
		{
			inst.WriteToOutput("Purging existing memory and resizing...\n");

			bool err = false;

			
			try
			{
				uint64_t memsize = (static_cast<uint64_t>(1) << static_cast<uint64_t>(bitcount));
				inst.Mem().resize(memsize);
			}
			catch(std::bad_alloc)
			{
				inst.WriteToOutput("Failed to allocate required memory...\n");
				inst.WriteToOutput("Allocating 16KB for recovery...\n");
				inst.Mem().resize(16384);
				inst.WriteToOutput("Resize completed with errors.\n");
				err = true;
			}

			inst.Mem().reset();
			inst.WriteToOutput("Memory space succesfully resized.\n");
		}
	}

	void go(const Arg_Vec& args, Shell_Cload& inst)
	{
		inst.enableMemCntrl();
	}
}
