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
		for(char c : ups)
		{
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

		for(char c : ups)
		{
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

	uint64_t StrOp::StrToUInt64(const UPString& in)
	{
		return numeric_interpret<unsigned long long, &strtoull>(in);
	}

	int64_t StrOp::StrToInt64(const UPString& in)
	{
		return numeric_interpret<long long, &strtoll>(in);
	}

	uint32_t StrOp::StrToUInt32(const UPString& in)
	{
		return numeric_interpret<unsigned long, &strtoul>(in);
	}

	int32_t StrOp::StrToInt32(const UPString& in)
	{
		return numeric_interpret<signed long, &strtol>(in);
	}
}
