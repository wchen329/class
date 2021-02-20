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
#ifndef __RANGE_H__
#define __RANGE_H__
#include <list>
#include <cstdio>
#include <string>
#include <vector>
#include "mt_exception.h"
#include "priscas_global.h"

namespace priscas
{
	LINK_DE typedef std::list<size_t>::iterator range_iterator;

	/* A generic, iterable "range"
	 * Really just an ordered collection of numbers which can be iterated over
	 */
	class range
	{
		std::list<size_t> numbers;

		// Constructs a range using a string specified
		// Ranges are constructed using MATLAB syntax, that is:
		// begin (inclusive):end (inclusive):step
		// i.e. 0:4:2 would look like 0, 2 (4 is out of range)
		// the step can be omitted which then would look like 0:4 (0, 1, 2, 3)
		public:
			LINK_DE range(const UPString&);
			LINK_DE range_iterator begin() { return this->numbers.begin(); }
			LINK_DE range_iterator end() { return this->numbers.end(); }
	};
}

#endif
