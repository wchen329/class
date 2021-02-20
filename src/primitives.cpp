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
#include "primitives.h"

namespace priscas
{
	BW_32::BW_32(char b_0, char b_1, char b_2, char b_3)
	{
		char * w_ptr = (char*)&(w.i32);
		*w_ptr = b_0;
		*(w_ptr + 1) = b_1;
		*(w_ptr + 2) = b_2;
		*(w_ptr + 3) = b_3;
	}

	BW_16::BW_16(char b_0, char b_1)
	{
		char * w_ptr = (char*)&(w.i16);
		*w_ptr = b_0;
		*(w_ptr + 1) = b_1;
	}
}
