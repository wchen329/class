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
#ifndef __MEM_H__
#define __MEM_H__
#include <cstddef>
#include "primitives.h"
#include "priscas_global.h"

namespace priscas
{

	/* bit_storage
	 * That represents anything that any level of the memory hierarchy
	 *
	 * wchen329
	 */
	class bit_storage
	{
		public:
			/* byte_8b& operator[](ptrdiff_t ind)
			 * IMPLEMENTATION: get a byte from this bit storage.
			 */
			virtual byte_8b& operator[](ptrdiff_t ind) = 0;

			/* const byte_8b& operator[](int ind) const
			 * IMPLEMENTATION: get a (read only) byte ref. from this bit storage
			 */
			virtual const byte_8b& operator[](ptrdiff_t ind) const = 0;

			/* ~bit_storage()
			 * IMPLEMENTATION: clean up
			 */
			virtual ~bit_storage() {};
	};

}
#endif
