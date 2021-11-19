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
#ifndef __RANGE_H__
#define __RANGE_H__
#include <list>
#include <cstdio>
#include <string>
#include <vector>
#include "mt_exception.h"
#include "priscas_global.h"
#include "ustrop.h"

namespace priscas
{

	/* A generic, iterable "range"
	 * Really just an ordered collection of numbers which can be iterated over
	 */
	class range
	{
		// Constructs a range using a string specified
		// Ranges are constructed using MATLAB syntax, that is:
		// begin (inclusive):end (inclusive):step
		// i.e. 0:4:2 would look like 0, 2 (4 is out of range)
		// the step can be omitted which then would look like 0:4 (0, 1, 2, 3)
		public:
			range(const UPString&);
			
			// Reset the range so it iterates from the beginning.
			void reset() { this->curr = first; }

			size_t next() { size_t ret = curr; curr += step; ++next_count; return ret; }

			bool atEnd()
			{
				return next_count >= bound;
			}

		private:
			int64_t first; // Initial Value in Range
			int64_t bound; // How many times next can be called without exhausting the bound
			int64_t step; // The iterator step
			int64_t curr; // Current value
			int64_t next_count; // How many times next was called
	};
}

#endif
