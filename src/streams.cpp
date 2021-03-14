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
#include <strstream>
#include "streams.h"

namespace priscas_io
{
	null_text_stream null_tstream;
	const priscas::UPString newLine = "\n";

	template<class InputC> std::string GeneralConvToStr(InputC in)
	{
		std::string ret;
		
		std::strstream conv;
		conv << in;

		while(!conv.eof())
		{
			ret += conv.get();
		}

		if(ret.length() > 0)
		{
			ret.pop_back();
		}

		return ret;
	}

	std::string StrTypes::IntToStr(int in)
	{
		return GeneralConvToStr<int>(in);	
	}

	std::string StrTypes::Int32ToStr(long in)
	{
		return GeneralConvToStr<long>(in);
	}
	std::string StrTypes::UIntToStr(unsigned in)
	{
		return GeneralConvToStr<unsigned>(in);
	}

	std::string StrTypes::UInt32ToStr(unsigned long in)
	{
		return GeneralConvToStr<unsigned long>(in);
	}

	std::string StrTypes::UInt64ToStr(unsigned long long in)
	{
		return GeneralConvToStr<unsigned long long>(in);
	}

	std::string StrTypes::Int64ToStr(long long in)
	{
		return GeneralConvToStr<long long>(in);
	}

	std::string StrTypes::SizeToStr(size_t in)
	{
		return GeneralConvToStr<size_t>(in);
	}

	std::string StrTypes::FloatToStr(float in)
	{
		return GeneralConvToStr<float>(in);
	}

}
