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
#ifndef __USTROP_H__
#define __USTROP_H__

/* Micro String Operations
 * wchen329
 */
#include "mt_exception.h"
#include "priscas_global.h"
#include "primitives.h"
#include <algorithm>
#include <cstdint>
#include <cstdlib>

namespace priscas
{
	class StrOp
	{
		public:

		static bool is_whitespace(char c)
		{
			return c == '\n' || c == '\r' || c == ' ' || c == '\t';
		}

		/* whitespace return
		 * Remove all leading whitespaces 
		 */
		static UPString ws_return(const UPString&);

		/* strip comment
		 * Remove commented code
		 */
		static UPString strip_comment(const UPString&);

		/* tokas
		 * Tokenize a string value assignment pair.
		 */
		static NameValueStringPair tokas(const UPString&);

		/* has_prefix
		 * Check for a specific prefix (sprfx).
		 * If non-matching prefix, return null string
		 * else, return string without the prefix.
		 */
		static UPString has_prefix(const UPString& strin, const UPString& sprfx);



		/* numeric_interpet<>
		 * Interpret string as specific type
		 */
		template<class Tin>
		static Tin numeric_interpret(const UPString& in)
		{

			const UPString& key = in;
			
			Tin ret = 0;
			int base = 10;

			UPString unprefd;

			// First we need to check for prefixes relating to various bases.
			unprefd = StrOp::has_prefix(key, "0x");		
			if(unprefd != "")
			{
				base = 16;
			}
			else
			{
				unprefd = StrOp::has_prefix(key, "o");		
				if(unprefd != "")
				{
					base = 8;
				}
				else
				{
					// If there are no prefixes, just execute assuming decimal
					unprefd = key;
				}
			}

			// Check for "-". If it precedes everything, make the number
			// negative
			if(unprefd.size() == 0)
			{
				throw priscas::mt_bad_imm();
			}

			bool neg = unprefd[0] == '-';
			if(neg) unprefd = unprefd.substr(1, unprefd.size());

			// For convenience reverse to calc
			std::reverse<UPString::iterator>(unprefd.begin(), unprefd.end());

			// Use the definition of a n-radix number to build ret (little endian)
			for(size_t itr = 0; itr < unprefd.size(); ++itr)
			{
				Tin multiplier = 1;

				for(size_t m_itr = 0; m_itr < itr; ++m_itr)
				{
					multiplier *= base;
				}

				ret += get_digit_value(unprefd[itr]) * multiplier;
			}

			if(neg) ret = -ret;

			return ret;
		}

		static byte_8b get_digit_value(char ascii);

		static uint64_t StrToUInt64(const UPString&);
		static int64_t StrToInt64(const UPString&);
		static uint32_t StrToUInt32(const UPString&);
		static int32_t StrToInt32(const UPString&);

	};

}

#endif
