#include "env.h"

namespace priscas
{
	void Env::characterize_Env(const Arg_Vec& args)
	{
		size_t argc = args.size();

		for(size_t argind = 0; argind < argc; argind++)
		{
			if(args[argind] == "-h")
			{
				this->has_Option_Help = true;
				return;
			}

			if(args[argind] == "-i")
			{
				// Set to machine mode
				this->update_Mode(MACHINE);

				// Declare -i was received
				this->has_Option_AsmInput = true;
				
				// Set file name
				if((argind + 1) < argc)
				{
					this->asmInputs.push_back(args[argind+1]); // todo store this value
					this->has_AsmInput_Value = true;
				}
			}

			if(args[argind] == "-m")
			{
				this->has_Option_MemWidth = true;
				if((argind + 1) < argc)
				{	
					this->mem_bitwidth = atoi(args[argind + 1].c_str());
				}
			}

			if(args[argind] == "-c")
			{
				this->has_Option_CpuSelect = true;
				if((argind + 1) < argc)
				{
					this->cpuStrings.push_back(args[argind + 1]);
				}
			}
		}
	}
}