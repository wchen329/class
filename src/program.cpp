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
#include "program.h"

namespace priscas
{
	uint8_t Program::read(ptrdiff_t addr)
	{
		if(addr >= eop)
		{
			throw mem_oob_exception();
		}

		return prog_range[addr];
	}

	void Program::write_next(uint8_t data)
	{
		if(eop >= eor)
		{
			throw mt_program_size_violation();
		}

		prog_range[eop] = data;
		++eop;
	}
}
