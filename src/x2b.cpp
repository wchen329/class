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
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <list>

#include "primitives.h"
#include "ustrop.h"

/* Return true if valid for hex string.
 * Assumes that the hex internally has all been converted to
 * upper case.
 */
bool isHex(char c)
{
	return (c == 'a' || c == 'b' || c == 'c' || c == 'd' ||
		c == 'e' || c == 'f' || c >= '0' && c <= '9');
}

int main(int argc, char ** argv)
{
	// Test argument count
	if(argc < 3)
	{
		fprintf(stdout, "x2b\n");
		fprintf(stdout, "Converts a list of hex strings into binary format\n");
		fprintf(stdout, "Usage: hex2bin input_file_name output_file_name\n");
		return 1;
	}

	std::list<priscas::byte_8b> b_list;
	
	// Open input and output files
	FILE* ifile = fopen(argv[1], "r");
	FILE* ofile = fopen(argv[2], "w");

	if(ifile == nullptr || ofile == nullptr)
	{
		fprintf(stderr, "error: failed to open either input for reading, or output for writing.\n");
		return 5;
	}

	// Read in input file
	uint32_t buildable = 0;
	char val[5] = {'0', 'x', 0, 0, 0};
	uint32_t buf_i = 0;
	char buf = 0;
	priscas::byte_8b count = 0;
	size_t rd_size = 0;
	
	while((buf_i = fgetc(ifile)) != EOF)
	{
		buf = static_cast<char>(buf_i);

		// Ignore whitespace characters
		if(buf == '\n' || buf == '\r' || buf == ' ' || buf == '\t')
		{
			continue;
		}

		if(!isHex(buf)) 
		{
			fprintf(stderr, "error: non hex-or-whitspace detected. Unable to continue...\n");
			fclose(ifile);
			fclose(ofile);
			return -2;
		}
		val[(count++) + 2] = buf;

		if(count == 2)
		{
			// convert
			buildable = priscas::StrOp::StrToUInt32(val);

			// take the first byte of buildable and store it.
			priscas::byte_8b * storeable = reinterpret_cast<priscas::byte_8b*>(&buildable);
			b_list.push_front(*storeable);

			// Reset
			val[2] = 0;
			val[3] = 0;
			count = 0;
		}

		// If valid hex add the buffer on to the character list.
	}

	if(count == 1)
	{
		fprintf(stderr, "error: dangling hex value (it should be aligned along 8-bit bytes)\n");
		return -1;
		fclose(ifile);
		fclose(ofile);
	}

	for(std::list<priscas::byte_8b>::iterator itr = b_list.begin(); itr != b_list.end(); ++itr)
	{
		priscas::byte_8b buf = *itr;
		fwrite(&buf, sizeof(priscas::byte_8b), 1, ofile);
	}


	fclose(ifile);
	fclose(ofile);
 
	return 0;
}
