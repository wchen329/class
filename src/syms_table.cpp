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
#include "syms_table.h"

namespace priscas
{
	void syms_table::insert(std::string key, unsigned long PC)
	{

		// That symbol better not be assigned already...
		bool already_def = this->syms_to_PC.count(key) > 0;
			
		if(already_def)
		{
			throw mt_multidef_symbol(key.c_str());
		}

		// Allows lookup from PC to symbol

		// case: not yet allocated, create a new entry
		if(!this->has(PC))
		{
			std::pair<unsigned long, std::vector<std::string>> in;
			in.first = PC;
			in.second = std::vector<std::string>();
			in.second.push_back(key);
			this->PC_to_syms.insert(in);
		}


		// other case: PC entry already there, just create a new entry
		else
		{
			// Allows lookup from PC to symbol
			this->PC_to_syms.at(PC).push_back(key);
		}

		// Allows lookup from symbol to PC
		std::pair<std::string, unsigned long> in_2;
		in_2.first = key;
		in_2.second = PC;
		this->syms_to_PC.insert(in_2);

	}

	void mono_syms_table::insert(std::string key, unsigned long PC)
	{

		// Allows lookup from PC to symbol

		// case: not yet allocated, create a new entry
		if(!this->has(PC))
		{
			std::pair<unsigned long, std::vector<std::string>> in;
			in.first = PC;
			in.second = std::vector<std::string>();
			in.second.push_back(key);
			this->PC_to_syms.insert(in);
		}


		// other case: PC entry already there, just create a new entry
		else
		{
			// Allows lookup from PC to symbol
			this->PC_to_syms.at(PC).push_back(key);
		}
	}
}
