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
#ifndef __CONTEXT_H__
#define __CONTEXT_H__
#include <cstdio>
#include "mt_exception.h"
#include "priscas_global.h"

namespace priscas
{

	/* srpackable - Save / Restore Packable
	 * Can be packed in a save restore image.
	 */
	class srpackable
	{
		public:
		
			/* void save(FILE*)
			 * IMPLEMENTATION: save this as part of the context.
			 */
			virtual void save(FILE*) = 0;

			/* void restore(FILE*)
			 * IMPLEMENTATION: restore a saved context portion
			 */
			virtual void restore(FILE*) = 0;
	};

	/* Save / Restore Handler
	 * save and restore controllers for srpackables
	 * Handles file opening and closing automatically and raises errors.
	 */
	class sr_handler
	{
		public:
			static void image_save(srpackable&, const UPString&);
			static void image_restore(srpackable&, const UPString&);
	};

}

#endif