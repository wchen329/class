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
#ifndef __MMEM_H__
#define __MMEM_H__
#include <cstddef>
#include <cstring>
#include "AFU.h"
#include "config.h"
#include "priscas_global.h"
#include "primitives.h"

namespace priscas
{
	// Main memory
	class mmem
	{
		public:
			size_t get_size(){return size;}		// returns size;
			mmem();
			mmem(size_t);
			volatile byte_8b& operator[](ptrdiff_t ind);
			const volatile byte_8b& operator[](ptrdiff_t ind) const;
			void save(ptrdiff_t begin, ptrdiff_t end, FILE*);
			void restore(ptrdiff_t begin, FILE*);
			void resize(size_t size);
			void reset();
			void setAFU(AFU* afu) { this->afu = afu; }
			~mmem();
		private:
			mmem operator=(const mmem &);		// copy assignment, disabled
			mmem(const mmem &);			// copy constructor, disabled
			volatile byte_8b * data;		// the actual data of memory
			size_t size;				// size of memory space in bytes

			virtual void alloc(size_t bytes);
			virtual void dealloc();
			AFU* afu; 
	};

}

#endif
