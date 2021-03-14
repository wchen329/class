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
#include "mtsstream.h"

namespace priscas
{

	void asm_ostream::append(uint8_t ui)
	{
		fwrite(&ui, sizeof(ui), 1, this->f);
	}

	asm_ostream::asm_ostream(const char * filename)
	{
		this->f = fopen(filename, "w");
		if(f == NULL)
		{
			// Throw an exception here
			throw  mt_illegal_file_error();
		}
	}

	asm_ostream::~asm_ostream()
	{
		fclose(this->f);
	}
}
