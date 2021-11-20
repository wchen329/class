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
#include "ustrop.h"

namespace priscas
{
	// These can be simplified by using
	// C++ std::string member functions 
	// TODO: do that
	UPString StrOp::ws_return(const UPString& ups)
	{
		UPString ret;
		bool notws = false;
		for(size_t ind = 0; ind < ups.size(); ++ind)
		{
			char c = ups[ind];

			if(!is_whitespace(c))
			{
				notws = true;
			}

			if(notws)
			{
				ret += c;
			}
		}

		return ret;
	}

	UPString StrOp::strip_comment(const UPString& ups)
	{
		UPString ret;

		for(size_t ind = 0; ind < ups.size(); ++ind)
		{
			char c = ups[ind];
			if(c == ';') break;
			if(c == '\n') break;
			ret += c;
		}

		return ret;
	}

	NameValueStringPair StrOp::tokas(const UPString& ups)
	{
		UPString name, value;
		size_t ind = ups.find_first_of("=");

		if(ind == -1)  return NameValueStringPair("", "");

		for(size_t wh = 0; wh < ind; ++wh)
		{
			name += ups[wh];
		}
		for(size_t wh = ind + 1; wh < ups.size(); ++wh)
		{
			value += ups[wh];
		}
		
		return NameValueStringPair(name, value);
	}


	UPString StrOp::has_prefix(const UPString& strin, const UPString& sprfx)
	{
		UPString ret;

		if(sprfx.size() >= strin.size())
		{
			return ret;
		}

		for(size_t ind = 0; ind < sprfx.size(); ++ind)
		{
			if(strin[ind] != sprfx[ind])
			{
				return ret;
			}
		}

		return strin.substr(sprfx.size(), strin.size());

	}

	// Convert ascii to binary, supports up to hex
	byte_8b StrOp::get_digit_value(char ascii)
	{
		// For numerical, easy; just subtract the ASCII base
		if(ascii >= '0' && ascii <= '9')
		{
			return ascii - 48; // '0' is 48
		}

		// For hex chars
		switch(ascii)
		{

			// More flexible ways to do this but... this is okay too
			case 'a':
			case 'A':
				return 10;
			case 'b':
			case 'B':
				return 11;
			case 'c':
			case 'C':
				return 12;
			case 'd':
			case 'D':
				return 13;
			case 'e':
			case 'E':
				return 14;
			case 'f':
			case 'F':
				return 15;
			default:
				throw mt_bad_imm();
		}
	}

	uint64_t StrOp::StrToUInt64(const UPString& in)
	{
		//return numeric_interpret<unsigned long long, &strtoull>(in);
		return numeric_interpret<uint64_t>(in);
	}

	int64_t StrOp::StrToInt64(const UPString& in)
	{
		//return numeric_interpret<long long, &strtoll>(in);
		return numeric_interpret<int64_t>(in);
	}

	uint32_t StrOp::StrToUInt32(const UPString& in)
	{
		return numeric_interpret<uint32_t>(in);
	}

	int32_t StrOp::StrToInt32(const UPString& in)
	{
		return numeric_interpret<int32_t>(in);
	}
}
