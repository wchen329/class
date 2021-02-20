#include "env.h"

namespace priscas
{
	void Env::characterize_Env(const Arg_Vec& args)
	{
		size_t argc = args.size();

		for(size_t argind = 0; argind < argc; argind++)
		{
			if(args[argind] == "-l")
			{
				this->has_Option_AsmMode = true;
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

			if(args[argind] == "-o")
			{
				// Set to machine mode
				this->update_Mode(MACHINE);

				// Declare -o was received
				this->has_Option_AsmOutput = true;
				
				// Set file name
				if((argind + 1) < argc)
				{
					this->asmOutput = args[argind+1];
					this->has_AsmOutput_Value = true;
				}
			}
		}
	}
}
