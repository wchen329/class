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
		template<class Tin, Tin (*f)(const char*, char**, int) throw()>
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

			char * epb = nullptr;
			ret = f(unprefd.c_str(), &epb, base);

			if(*epb != '\0')
			{
				throw mt_bad_imm();
			}
		
				return ret;
			}

			static uint64_t StrToUInt64(const UPString&);
			static int64_t StrToInt64(const UPString&);
			static uint32_t StrToUInt32(const UPString&);
			static int32_t StrToInt32(const UPString&);

	};
}

#endif
