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
#include "range.h"

namespace priscas
{
	range::range(const UPString& specifier)
	{
		// 0th value - the begin
		// 1st value - the end
		// 2nd value - the step
		std::vector<std::string> string_list;
		std::vector<long> bound_list;

		std::string imm = "";

		for(size_t itr = 0; itr < specifier.size(); itr++)
		{
			if(specifier[itr] != ':')
			{
				imm += specifier[itr];
			}

			else
			{
				string_list.push_back(imm);
				imm = "";
			}
		}
		
		if(imm != "")
		{
			string_list.push_back(imm);
		}

		// Now evaluate. We have exactly two or three fields?
		if(string_list.size() != 1 && string_list.size() != 2 && string_list.size() != 3)
		{
			throw priscas::mt_invalid_range("Must have either 1 - 3 parameters in the format index, begin:end, or begin:end:step.");
		}
		
		// Then just convert. If zero, check that it's REALLY zero
		for(size_t itr_2 = 0; itr_2 < string_list.size(); itr_2++)
		{
			errno = 0;
			long val = strtol(string_list[itr_2].c_str(), nullptr, 10);
			if(errno != 0)
			{
				throw std::exception();
			}

			bound_list.push_back(val);
		}

		long begin = bound_list[0];
		long end = bound_list.size() <= 1 ? bound_list[0] : bound_list[1];
		long step = bound_list.size() <= 2 ? 1 : bound_list[2];

		// Check for sensical stepping
		if(step == 0)
			throw priscas::mt_invalid_range("Step may not be 0.");

		if(end - begin > 0)
		{
			if(!(step > 0)) throw priscas::mt_invalid_range("A decreasing range must specify a postive step.");
		}

		if(end - begin < 0)
		{
			if(!(step < 0)) throw priscas::mt_invalid_range("A decreasing range must specify a negative step.");
		}

		// Now evaluate the range
		while((end - begin >= 0 && step > 0) || (end - begin <= 0 && step < 0))
		{
			numbers.push_back(begin);
			begin += step;
		}
	}
}
