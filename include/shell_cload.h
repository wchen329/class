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
#ifndef __SHELL_CLOAD_H__
#define __SHELL_CLOAD_H__
#include "AFU.h"
#include "config.h"
#include "mmem.h"
#include "afu_json_info.h"
#include "shell.h"

namespace priscas
{
	class Shell_Cload;
}

#include "runtime_call.h"

namespace priscas
{
	/* A mapping from a string into a directive function pointer (must be a member of Shell)
	 */
	typedef std::pair<std::string, void(*)(const Arg_Vec &, Shell_Cload&)> directive_pair;

	class Shell_Cload : public Shell
	{
		public:
			virtual void Run();
			Shell_Cload();
			mmem& Mem() { return shmem; }
			void AFU_Reset();
			void enableMemCntrl() { afu.write(MMIO_GO, 0x1); }
			void execute_runtime_directive(UPString_Vec& args_list);

		private:
			// AFU
			AFU afu;

			// Runtime Directives
			priscas::mono_syms_table directive_syms;
			std::map<std::string, void(*)(const Arg_Vec&, Shell_Cload& shell)> directives;
			mmem shmem;
	};
}

#endif
