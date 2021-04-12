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

	void asm_ostream::append(uint8_t* ui, size_t count)
	{
		if(st == BIN)
		{
			fwrite(ui, 1, count, this->f);
		}
		else
		{
			UPString ninst = dynamicHexBuilder(ui, count);

			if(st == HEX)
			{
				fprintf(this->f, ninst.c_str());
				fprintf(this->f, "\n");
			}
			else if(st == MIF)
			{
				int bc_c = count * 8;
				max_inst_size = bc_c < max_inst_size ? max_inst_size : bc_c;
				mif_insts.push_back(ninst);
			}
		}
	}

	asm_ostream::asm_ostream(const char * filename, STREAM_TYPE stin) : max_inst_size(0)
	{
		this->f = fopen(filename, "w");
		if(f == NULL)
		{
			// Throw an exception here
			throw mt_illegal_file_error();
		}

		st = stin;
	}

	void asm_ostream::finalize()
	{
		if(st == MIF && mif_insts.size() > 0)
		{
			// If we are writing a mif file, serialize at this point
			fprintf(this->f, "-- CLASS generated .mif file\n");
			fprintf(this->f, "WIDTH=%d;\n", max_inst_size);
			fprintf(this->f, "DEPTH=%d;\n", mif_insts.size());
			fprintf(this->f, "\n\n");

			// Write Radix Information
			fprintf(this->f, "ADDRESS_RADIX=UNS;\nDATA_RADIX=HEX;\n\n\n");
			fprintf(this->f, "CONTENT BEGIN\n");

			for(uint64_t addr = 0; addr < mif_insts.size(); ++addr)
			{
				fprintf(this->f, "\t%lld: %s;\n", addr, mif_insts[addr].c_str());
			}

			fprintf(this->f, "END;\n");
		}
	}

	asm_ostream::~asm_ostream()
	{
		fclose(this->f);
	}

}
