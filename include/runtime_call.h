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
#ifndef __RUNTIME_CALL_H__
#define __RUNTIME_CALL_H__

#include <cstdio>
#include <map>
#include <vector>
#include "messages.h"
#include "priscas_osi.h"
#include "priscas_global.h"
#include "range.h"
#include "shell_cload.h"
#include "streams.h"
#include "ustrop.h"

namespace priscas
{
	// For the Shell_Cload
	void breakpoint(const Arg_Vec & args, Shell_Cload& inst);
	void exit(const Arg_Vec & args, Shell_Cload& inst);
	void help(const Arg_Vec & args, Shell_Cload& inst);

	// For the Loader
	void help_loader(const Arg_Vec& args, Shell_Cload& inst);
	void reset(const Arg_Vec& args, Shell_Cload& inst);
	void sr(const Arg_Vec& args, Shell_Cload& inst);
	void mem(const Arg_Vec& args, Shell_Cload& inst);
	void wait(const Arg_Vec& args, Shell_Cload& inst);
	void resize(const Arg_Vec& args, Shell_Cload& inst);
	void go(const Arg_Vec& args, Shell_Cload& inst);
}

#endif
