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
#include "env.h"

namespace priscas
{
	void Env::characterize_Env(const Arg_Vec& args)
	{
		size_t argc = args.size();

		for(size_t argind = 0; argind < argc; argind++)
		{
			if(args[argind] == "-s")
			{
				this->has_Option_AsmStrMode = true;

				if((argind + 1) < argc)
				{
					this->has_AsmStrMode_Value = true;

					if(args[argind + 1] == "hex") st = asm_ostream::HEX; 
					else if(args[argind + 1] == "bin") st = asm_ostream::BIN;
					else if(args[argind + 1] == "mif") st = asm_ostream::MIF;
					else
					{
						this->has_AsmStrMode_Value = false;
					}
				}
			}

			if(args[argind] == "-b")
			{
				this->has_Option_WordSize = true;

				if((argind + 1) < argc)
				{
					this->width = atoi(args[argind + 1].c_str());

					if(width <= 0)
					{
						this->has_WordSize_Value = false;
					}
					else
					{
						this->has_WordSize_Value = true;
					}
				}
			}

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
