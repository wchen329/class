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
#ifndef __PROGRAM_H__
#define __PROGRAM_H__
#include <cstddef>
#include <cstdint>
#include "mt_exception.h"

#define KB 1024
#define MB KB*1024
#define MAX_PROGRAM 64*MB

namespace priscas
{

	/* Program
	 * A program is simple a range of bytes that correspond to
	 * instruction code.
	 */

	class Program
	{
		public:
			uint8_t read(ptrdiff_t addr);
			void write_next(uint8_t data);

			const uint8_t* get_PC_zero() { return prog_range; }

			uint64_t get_EOP() { return this->eop; }

			// Empty program
			Program() : eop(0), eor(MAX_PROGRAM){}

		private:
			// Memory Range
			uint8_t prog_range[MAX_PROGRAM];

			// End of program pointer (non inclusive)
			uint64_t eop;

			// End of range pointer (non-inclusive)
			uint64_t eor;

	};
}

#endif
