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
		bool HAS_SPECIFIER = false;
		for(size_t a = 0; a < args.size(); a++)
		{
			if(args[a] == "-l")
			{
				HAS_SPECIFIER = true;
				if(args.size() > (a + 1))
				{
					errno = 0;
					long n = strtol(args[a + 1].c_str(), nullptr, 10);
					if(errno != 0)
					{
						throw priscas::mt_exception();
					}
					
					inst.add_program_breakpoint(n);
				}

				else throw priscas::mt_exception();
			}

			if(args[a] == "-c")
			{
				HAS_SPECIFIER = true;
				if(args.size() > (a + 1))
				{
					errno = 0;
					long n = strtol(args[a + 1].c_str(), nullptr, 10);
					if(errno != 0)
					{
						throw priscas::mt_exception();
					}

					inst.add_microarch_breakpoint(n);
					std::string o = (std::string("Breakpoint set at cycle ") + priscas_io::StrTypes::Int32ToStr(n) + std::string("\n"));
					inst.WriteToOutput(o);
				}

				else throw priscas::mt_exception();
			}
		}

		if(!HAS_SPECIFIER)
		{
			inst.WriteToOutput(("Usage: .breakpoint -c [cycle_number] or .breakpoint -l [line_number]\n"));
		}
	}

	void cycle(const Arg_Vec & args, Shell& inst)
	{
		inst.GetMotherboard().step();
	}

	void cpuopts(const Arg_Vec & args, Shell& inst)
	{
		priscas::cpu& c = inst.GetMotherboard().get_cpu();
		priscas::diag_cpu & dcpu = dynamic_cast<priscas::diag_cpu&>(c);
		
		CPU_ControlPanel & cp = dcpu.get_CPU_options();

		if(args.size() <= 1)
		{
			inst.WriteToOutput(("[CPU Specific Options]\n"));
			if(cp.empty())
			{
				inst.WriteToOutput(("No CPU options found.\n"));
			}

			else
			{
				const CPU_Option_List cpuopts_list = cp.list_Controls();

				for(CPU_Option_List_CIter s = cpuopts_list.cbegin(); s != cpuopts_list.cend(); ++s)
				{
					inst.WriteToOutput("\n");
					std::string o = s->getName() + "\n\t" + s->getDescription() + priscas_io::newLine;
					inst.WriteToOutput(o);
					
					const Name_Vec& posVal_vector = s->get_PossibleValues();

					if(posVal_vector.size() > 0)
					{
						std::string posVals = "\tPossible Values: ";

						for(size_t vind = 0; vind < posVal_vector.size(); vind++)
						{
							posVals += (posVal_vector[vind] + " ");
						}

						posVals += priscas_io::newLine;
						
						inst.WriteToOutput(posVals);
					}

					std::string curVal = "Current Value: ";
					curVal += s->getValue();
					curVal += priscas_io::newLine;
					inst.WriteToOutput(curVal);
				}

				inst.WriteToOutput("\n");
				inst.WriteToOutput(("To execute an option just enter\n"));
				inst.WriteToOutput(".cpuopts [option 1]=[value 1] ... into the shell\n");
				inst.WriteToOutput("If no possible values are specified for an option, omit \'=\':\n");
				inst.WriteToOutput(".cpuopts [option 1]\n");
			}
		}

		else
		{
			try
			{
				std::vector<priscas::NameValueStringPair> nvsp = scan_for_values(args);

				for(std::vector<NameValueStringPair>::iterator nvspit = nvsp.begin() + 1; nvspit != nvsp.end(); ++nvspit)
				{
					cp.set_ControlValue(nvspit->getName(), nvspit->getValue());
				}
			}

			catch(priscas::mt_exception& mte)
			{
				inst.WriteToOutput("An error has occurred while processing CPU options\n");
				inst.WriteToOutput(mte.get_err());
			}
		}
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

			else if(args[1] == ".cpuopts")
			{
				msg = HELP_CPUOPTS;
			}

			else if(args[1] == ".cycle")
			{
				msg = HELP_CYCLE;
			}

			else if(args[1] == ".exit")
			{
				msg = HELP_EXIT;
			}
			
			else if(args[1] == ".help")
			{
				msg = HELP_HELP;
			}

			else if(args[1] == ".mem")
			{
				msg = HELP_MEM;
			}

			else if(args[1] == ".rst")
			{
				msg = HELP_RST;
			}

			else if(args[1] == ".run")
			{
				msg = HELP_RUN;
			}

			else if(args[1] == ".sr")
			{
				msg = HELP_SR;
			}

			else if(args[1] == ".state")
			{
				msg = HELP_STATE;
			}

			else if(args[1] == ".time")
			{
				msg = HELP_TIME;
			}

			inst.WriteToOutput(msg.c_str());
		}
	}

	void pci(const Arg_Vec & args, Shell& inst)
	{
		inst.WriteToOutput(("[PCI Bus Emulation]\n"));
		inst.WriteToOutput(("Not yet implemented\n"));
	}

	void rst(const Arg_Vec & args, Shell& inst)
	{
		if(args.size() <= 1)
		{
			inst.GetMotherboard().reset();
		}

		else
		{
			bool RESET_STATE = false;
			bool RESET_MEM = false;
			bool RESET_MACHINE = false;

			for(size_t itr = 1; itr < args.size(); itr++)
			{
				if (args[itr] == "-s") RESET_STATE = true;
				else if(args[itr] == "-m") RESET_MEM = true;
				else if(args[itr] == "-M") RESET_MACHINE = true;
				else continue;
			}

			if(RESET_STATE)
			{
				inst.GetMotherboard().get_cpu().rst();
			}

			if(RESET_MEM)
			{
				// still to be implemented
			}

			if(RESET_MACHINE)
			{
				inst.GetMotherboard().reset();
			}
		}
	}

	void mem(const Arg_Vec & args, Shell& inst)
	{
		inst.WriteToOutput(("[Memory Information]\n"));

		priscas::mb& cmp = inst.GetMotherboard();
		

		// No args specified just print the memory size (in bytes)
		if(args.size() <= 1)
		{
			std::string o = (std::string("Main memory Size: " + priscas_io::StrTypes::SizeToStr(cmp.get_mmem_size()) + std::string(" bytes\n")));
			inst.WriteToOutput(o);
		}

		// Otherwise print memory specific to indicies
		for(size_t itr = 1; itr < args.size(); itr++)
		{
			priscas::range r = priscas::range(args[itr]);

			for(priscas::range_iterator itr_2 = r.begin(); itr_2 != r.end(); itr_2++)
			{
				if(*itr_2 >= inst.GetMotherboard().get_mmem_size() || *itr_2 < 0)
				{
					throw priscas::mem_oob_exception();
				}

				std::string o = (std::string("Mem[") + priscas_io::StrTypes::SizeToStr(*itr_2) + std::string("]: ") + 
					priscas_io::StrTypes::IntToStr(inst.GetMotherboard().DMA_read(*itr_2)) + priscas_io::newLine);
				inst.WriteToOutput(o);
			}
		}
	}

	void power(const Arg_Vec & args, Shell& inst)
	{
		inst.WriteToOutput(("[Power Usage Statistics]\n"));
		inst.WriteToOutput(("Not yet implemented\n"));
	}

	void run(const Arg_Vec & args, Shell& inst)
	{
		inst.modeset_Machine();
	}

	void sound(const Arg_Vec & args, Shell& inst)
	{
		inst.WriteToOutput(("[Soundcard Emulation]\n"));
		inst.WriteToOutput(("Not yet implemented\n"));
	}

	void sr(const Arg_Vec & args, Shell& inst)
	{
		inst.WriteToOutput("[Save / Restore]\n");
		
		if(args.size() <= 1)
		{
			inst.WriteToOutput("No options were given, see \'.help .sr\' for usage information.\n");
			return;
		}

		bool save_mode = false;
		bool restore_mode = false;
		UPString filename;

		for(size_t viiter = 0; viiter < args.size(); viiter++)
		{
			size_t viiterpo = viiter + 1;

			if(args[viiter] == "-s")
			{
				save_mode = true;
			}

			else if(args[viiter] == "-r")
			{
				restore_mode = true;
			}

			else if(args[viiter] == "-f")
			{
				if(viiterpo < args.size())
				{
					filename = args[viiterpo];
				}
			}
		}

		// Check for errors
		if(save_mode && restore_mode)
		{
			inst.WriteToError("Error: Save mode and Restore Mode may not both be specified at once\n");
			return;
		}

		if(!save_mode && !restore_mode)
		{
			inst.WriteToError("Error: Either save mode or restore mode must be specified.\n");
			return;
		}

		if(filename == "")
		{
			inst.WriteToError("Error: Filename not specified, or invalid.\n");
			return;
		}

		// All checks have passed, now perform the operation
		
		mmem& mp = inst.GetMotherboard().get_mmem();

		if(save_mode)
		{
			sr_handler::image_save(mp, filename);
		}

		else if(restore_mode)
		{
			sr_handler::image_restore(mp, filename);
		}

		inst.WriteToOutput("Operation completed successfully.\n");
	}

	void state(const Arg_Vec & args, Shell& inst)
	{
		
		inst.WriteToOutput("[Register State Information]\n");

		priscas::mb& cmp = inst.GetMotherboard();
		priscas::diag_cpu& dcpu = dynamic_cast<priscas::diag_cpu&>(cmp.get_cpu());
		int reg_count = dcpu.get_ISA().get_reg_count();
		priscas::BW_32 pc_val = dcpu.get_PC();
		
		priscas::ISA& isa = dcpu.get_ISA();
		
		// No args specified print out every register
		if(args.size() <= 1)
		{
			std::string o = (std::string("PC:\t") + priscas_io::StrTypes::Int32ToStr(pc_val.AsInt32()) + "\t"
								+ priscas_io::StrTypes::UInt32ToStr(pc_val.AsUInt32()) + "\t"
								+ pc_val.toHexString()
								+ std::string("\n"));
			inst.WriteToOutput(o);

			for(int r = 0; r < reg_count; r++)
			{
				std::string o =
					isa.get_reg_name(r)
					+ std::string(":\t") 
					+ priscas_io::StrTypes::Int32ToStr(dcpu.get_reg_data(r).AsInt32()) + "\t"
					+ priscas_io::StrTypes::UInt32ToStr(dcpu.get_reg_data(r).AsUInt32()) + "\t"
					+ dcpu.get_reg_data(r).toHexString()
					+ priscas_io::newLine;
				inst.WriteToOutput(o);
			}
		}

		else
		{
			for(size_t itr = 1; itr < args.size(); itr++)
			{
				priscas::range r(args[itr]);

				for(priscas::range_iterator ritr = r.begin(); ritr != r.end(); ritr++)
				{

					if(*ritr < 0 || *ritr >= dcpu.get_ISA().get_reg_count())
					{
						throw priscas::reg_oob_exception();
					}

					std::string o = std::string(isa.get_reg_name(*ritr) + std::string(":\t") +
						priscas_io::StrTypes::Int32ToStr(dcpu.get_reg_data(*ritr).AsInt32()) + priscas_io::newLine);
					inst.WriteToOutput(o);
				}
			}
		}
	}

	void time(const Arg_Vec & args, Shell& inst)
	{
		inst.WriteToOutput("[Processor Timing Information]\n");
		unsigned long n = inst.GetMotherboard().get_cycles();
		std::string cc = "Cycle Count: " + priscas_io::StrTypes::UInt32ToStr(n) + " cycles\n";
		inst.WriteToOutput(cc);

		
		float instCount = static_cast<float>(dynamic_cast<diag_cpu&>(inst.GetMotherboard().get_cpu()).get_InstCommitCount());
		instCount = instCount == 0 ? 1 : instCount;
		UPString strCPI = priscas_io::StrTypes::FloatToStr(static_cast<float>(n) / instCount);
		UPString cpict = UPString("CPI: ") + strCPI + priscas_io::newLine;
		inst.WriteToOutput(cpict);

		// Print it out in terms of smallest units
		inst.WriteToOutput("Time Elapsed: ");
		unsigned long long total_time = inst.GetMotherboard().get_time().getPS();
		unsigned long long total_days = inst.GetMotherboard().get_time().getDays();
		
		if(total_time == 0)
		{
			inst.WriteToOutput("0s\t");
		}
		
		unsigned short picoseconds = (total_time) % 1000;
		total_time = total_time / 1000;

		unsigned short nanoseconds = (total_time) % 1000;
		total_time = total_time / 1000;

		unsigned short microseconds = (total_time) % 1000;
		total_time = total_time / 1000;

		unsigned short milliseconds = (total_time) % 1000;
		total_time = total_time / 1000;

		unsigned short seconds = (total_time) % 60;
		total_time = total_time / 60;

		unsigned short minutes = (total_time) % 60;
		total_time = total_time / 60;

		unsigned short hours = static_cast<unsigned short>(total_time);

		if(total_days > 0)
		{
			std::string t = priscas_io::StrTypes::UInt64ToStr(total_days) + " days\t";
			inst.WriteToOutput(t);
		}

		if(hours > 0)
		{
			std::string t = priscas_io::StrTypes::UIntToStr(hours) + " hours\t";
			inst.WriteToOutput(t);
		}

		if(minutes > 0)
		{
			std::string t = priscas_io::StrTypes::UIntToStr(minutes) + " minutes\t";
			inst.WriteToOutput(t);
		}

		if(seconds > 0)
		{
			std::string t = priscas_io::StrTypes::UIntToStr(seconds) + " seconds\t";
			inst.WriteToOutput(t);
		}

		if(milliseconds > 0)
		{
			std::string t = priscas_io::StrTypes::UIntToStr(milliseconds) + " ms\t";
			inst.WriteToOutput(t);
		}

		if(microseconds > 0)
		{
			std::string t = priscas_io::StrTypes::UIntToStr(microseconds) + " micros\t";
			inst.WriteToOutput(t);
		}

		if(nanoseconds > 0)
		{
			std::string t = priscas_io::StrTypes::UIntToStr(nanoseconds) + " ns\t";
			inst.WriteToOutput(t);
		}

		if(picoseconds > 0)
		{
			std::string t = priscas_io::StrTypes::UIntToStr(picoseconds) + " ps";
			inst.WriteToOutput(t);
		}

		UPString ups = "" + priscas_io::newLine;
		inst.WriteToOutput(ups);
	}

	void trace(const Arg_Vec & args, Shell& inst)
	{
		inst.WriteToOutput("[Special Tracing Options]\n");
		inst.WriteToOutput("Not yet implemented\n");
	}

	void vga(const Arg_Vec & args, Shell& inst)
	{
		inst.WriteToOutput("[VGA Emulation]\n");
		inst.WriteToOutput("Not yet implemented\n");
	}
}
