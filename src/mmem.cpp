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
#include "mmem.h"

#define GB 1073741824

namespace priscas
{
	volatile byte_8b& mmem::operator[](ptrdiff_t ind) {
		return data[ind];
	}

	const volatile byte_8b& mmem::operator[](ptrdiff_t ind) const
	{
		return data[ind];
	}

	mmem::mmem() :
		size(0),
		data(4)
	{
	}

	mmem::mmem(size_t size) : 
		size(size),
		data(4)
	{
		alloc(size);
	}

	mmem::mmem(const mmem & m)
	{
		// disabled, see interface declaration, may enable later
	}
	
	mmem mmem::operator=(const mmem & m)
	{
		// disabled, see interface declaration, may enable later
		return m;
	}

	void mmem::resize(size_t size)
	{
		dealloc();
		alloc(size);
		this->size = size;
	}

	mmem::~mmem()
	{
		dealloc();
	}

	void mmem::save(ptrdiff_t begin, ptrdiff_t end, FILE* f)
	{
		// Dump the rest of the memory array

		uint64_t count = 0;
		uint64_t offset = 0;

		while(count < (end-begin))
		{
			offset = (begin + count) % size;
			byte_8b buf = data[offset];
			fwrite(&buf, sizeof(byte_8b), 1, f);
			++count;
		}
	}

	void mmem::restore(ptrdiff_t begin, FILE* f)
	{
		// Load the rest of the memory array
		uint64_t read_count = 0;
		uint64_t offset = 0;
		do
		{
			byte_8b buf = 0;
			uint64_t where = (begin + offset) % size;
			read_count = fread(&buf, sizeof(byte_8b), 1, f);
			data[where] = buf;
			++offset;
		}
		while(read_count);
	}

	void mmem::reset()
	{
		// Can't use memset because of volatile
		// Do this slow iterative reset...
		for(size_t itr = 0; itr < this->size; ++itr)
		{
			data[itr] = 0x0;
		}
	}

	void mmem::alloc(size_t bytes)
	{
		// Can change allocator
		// Throw std::bad_alloc, potentially
		// Could also use new...

		// First, find how segments we need
		uint64_t seg_count = bytes / GB +
			((bytes % GB) != 0 ? 1: 0);

		// Allocate that many segments, at most 1GB each
		uint64_t bytes_left = bytes;
		int segno = 0;
		while(bytes_left != 0)
		{
			// Critical error if more than 4 segments
			if(segno > 4) abort();

			if(bytes_left >= GB)
			{
				data[segno] = afu->malloc<byte_8b>(GB);
			}
			else
			{
				data[segno] = afu->malloc<byte_8b>(bytes_left);
			}

			++segno;

		}

		this->data = (afu->malloc<byte_8b>(bytes));
		if(data == nullptr)
		{
			throw std::bad_alloc();
		}

		// When allocating, we have to write the array
		// starting address out
		afu->write(MMIO_BASE_ADDR_S0, reinterpret_cast<uint64_t>(this->data[0]));
		afu->write(MMIO_BASE_ADDR_S1, reinterpret_cast<uint64_t>(this->data[1]));
		afu->write(MMIO_BASE_ADDR_S2, reinterpret_cast<uint64_t>(this->data[2]));
		afu->write(MMIO_BASE_ADDR_S3, reinterpret_cast<uint64_t>(this->data[3]));
		afu->write(MMIO_SIZE, 1);

	}

	void mmem::dealloc()
	{
		if(this->data != nullptr)
		{
			// Declare the below memory hierarchy as invalid
			afu->write(MMIO_BASE_ADDR_S0, 0x0);
			afu->write(MMIO_BASE_ADDR_S1, 0x0);
			afu->write(MMIO_BASE_ADDR_S2, 0x0);
			afu->write(MMIO_BASE_ADDR_S3, 0x0);

			// Deallocate array
			afu->free(this->data);
		}
	}
}
