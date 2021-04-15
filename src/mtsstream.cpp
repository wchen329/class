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
				mif_insts.push_back(ninst);
			}
		}

		total_bytes += count;
	}

	asm_ostream::asm_ostream(const char * filename, STREAM_TYPE stin) :
		width(4),
		total_bytes(0)
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
			fprintf(this->f, "WIDTH=%d;\n", width * 8);
			fprintf(this->f, "DEPTH=%d;\n", (total_bytes / width) + (total_bytes % width != 0 ? 1 : 0)); // In the case of uneven alignment, round up
			fprintf(this->f, "\n\n");

			// Write Radix Information
			fprintf(this->f, "ADDRESS_RADIX=UNS;\nDATA_RADIX=HEX;\n\n\n");
			fprintf(this->f, "CONTENT BEGIN\n");

			unsigned addr = 0;
			unsigned bcount = 0;
			UPString next_line;
			for(uint64_t word_ind = 0; word_ind < mif_insts.size(); ++word_ind)
			{
				UPString& curr_inst = mif_insts[word_ind];

				while(!curr_inst.empty())
				{
					if(curr_inst.size() % 2 != 0)
					{
						// Throw an exception here
					}

					// Get the next byte
					UPString hb_lower;
					hb_lower += curr_inst.back();
					curr_inst.pop_back();
					UPString hb_upper;
					hb_upper += curr_inst.back();
					curr_inst.pop_back();

					UPString fbyte = hb_upper + hb_lower;

					next_line = fbyte + next_line;

					++bcount;
					
					if(bcount >= width)
					{
						fprintf(this->f, "\t%lld: %s;\n", addr++, next_line.c_str());
						// Flush current buffers.
						next_line.clear();
						bcount = 0;
					}
				}
			}

			// If next line is still here, that means we have an unaligned line
			// Nevertheless, we should support this by zero filling
			if(!next_line.empty())
			{
					while(bcount < width)
					{
						next_line = UPString("00") + next_line;
						++bcount;
					}

					fprintf(this->f, "\t%lld: %s;\n", addr++, next_line.c_str());
			}

			fprintf(this->f, "END;\n");
		}
	}

	asm_ostream::~asm_ostream()
	{
		fclose(this->f);
	}

}
