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
#ifndef __RUNTIME_CALL_H__
#define __RUNTIME_CALL_H__

#include <cstdio>
#include <map>
#include <vector>
#include "diag_cpu.h"
#include "mem.h"
#include "mb.h"
#include "messages.h"
#include "priscas_global.h"
#include "range.h"
#include "shell.h"

namespace priscas
{
	void breakpoint(const Arg_Vec & args, Shell& inst);
	void cycle(const Arg_Vec & args, Shell& inst);
	void cpuopts(const Arg_Vec & args, Shell& inst);
	void exit(const Arg_Vec & args, Shell& inst);
	void help(const Arg_Vec & args, Shell& inst);
	void mem(const Arg_Vec & args, Shell& inst);
	void pci(const Arg_Vec & args, Shell& inst);
	void power(const Arg_Vec & args, Shell& inst);
	void rst(const Arg_Vec & args, Shell& inst);
	void run(const Arg_Vec & args, Shell& inst);
	void sound(const Arg_Vec & args, Shell& inst);
	void state(const Arg_Vec & args, Shell& inst);
	void sr(const Arg_Vec & args, Shell& inst);
	void time(const Arg_Vec & args, Shell& inst);
	void trace(const Arg_Vec & args, Shell& inst);
	void vga(const Arg_Vec & args, Shell& inst);
}

#endif
