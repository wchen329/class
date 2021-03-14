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
/* ISA Generator
 * Companion program for CLASS
 *
 * wchen329
 */
#include <cstdio>
#include "ISA_desc.h"
#include "primitives.h"
#include "priscas_global.h"
#define MAX_LINE_SIZE 1024

int main(int argc, char** argv)
{
	using namespace priscas;

	fprintf(stdout, "CLASS ISA Generator\n");

	if(argc < 2)
	{
		fprintf(stdout, "Usage: isagen [isa description file]\n");
		return 0;
	}

	// Now we know we at least have potential file argument.
	
	FILE* isadesc = fopen(argv[1], "r");

	if(isadesc == nullptr)
	{
		fprintf(stderr, "Error: ISA file could not be opened.\n");
		return -1;
	}

	// Create ISA description object
	ISA_desc meta_ISA;

	// Now the file is open. Read line by line
	char cur_line[MAX_LINE_SIZE];
	memset(cur_line, 0, MAX_LINE_SIZE);
	while(fgets(cur_line, MAX_LINE_SIZE - 1, isadesc) != nullptr)
	{
		try
		{
			// Interpret Each Line
			meta_ISA.interpret(cur_line);
		}
		catch(mt_exception& mte)
		{
			fprintf(stderr, "An error has occurred while generating the ISA.\n");
			fprintf(stderr, mte.get_err().c_str());
			fprintf(stderr, "\n");
			fprintf(stderr, "No ISA files created.\n");
			return 1;
		}
	}
	
	fclose(isadesc);

	UPString src_file_name = meta_ISA.getName() + ".cpp";
	UPString header_file_name = meta_ISA.getName() + ".h";

	meta_ISA.dump(header_file_name, src_file_name);

	fprintf(stdout, "Operation completed successfully.\n");

	return 0;
}
