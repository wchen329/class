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
		ptrdiff_t segno = ((ind >> 30) & 3); // & 3 = 2'b11 zeroes
		ptrdiff_t offset = (ind & ((1 << 30) - 1));

		return data[segno][offset];
	}

	const volatile byte_8b& mmem::operator[](ptrdiff_t ind) const
	{
		ptrdiff_t segno = ((ind >> 30) & 3); // & 3 = 2'b11 zeroes
		ptrdiff_t offset = (ind & ((1 << 30) - 1));

		return data[segno][offset];
	}

	mmem::mmem() :
		size(0),
		data(4, 0x0)
	{
	}

	mmem::mmem(size_t size) : 
		size(size),
		data(4, 0x0)
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
		this->size = size;
		alloc(size);
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
			byte_8b buf = (*this)[offset];
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
			(*this)[where] = buf;
			++offset;
		}
		while(read_count);
	}

	void mmem::reset()
	{
		// Can't use memset because of volatile
		// Do this slow iterative reset...
		size_t brem = this->size;
		for(size_t sitr = 0; sitr < 4; ++sitr)
		{
			size_t nextsize = brem < GB ? brem : GB;

			for(size_t itr = 0; itr < nextsize; ++itr)
			{
				data[sitr][itr] = 0x0;
			}

			brem -= nextsize;
		}
	}

	void mmem::alloc(size_t bytes)
	{
		// Can change allocator
		// Throw std::bad_alloc, potentially
		// Could also use new...

		// First, find how segments we need
		//uint64_t seg_count = bytes / GB +
		//	((bytes % GB) != 0 ? 1: 0);

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
				bytes_left -= GB;
			}
			else
			{
				data[segno] = afu->malloc<byte_8b>(bytes_left);
				bytes_left = 0;
			}

			if(data[segno] == nullptr)
			{
				throw std::bad_alloc();
			}

			++segno;

		}

		// When allocating, we have to write the array
		// starting address out
		this->map_mem();

	}

	void mmem::dealloc()
	{
			// Declare the below memory hierarchy as invalid
			afu->write(MMIO_BASE_ADDR_S0, 0x0);
			afu->write(MMIO_BASE_ADDR_S1, 0x0);
			afu->write(MMIO_BASE_ADDR_S2, 0x0);
			afu->write(MMIO_BASE_ADDR_S3, 0x0);

			// Deallocate all pointers in the array
			for(size_t dp = 0; dp < 4; ++dp)
			{
				if(this->data[dp] != nullptr)
				{
					afu->free(this->data[dp]);
					this->data[dp] = nullptr;
				}
			}
	}

	void mmem::map_mem()
	{
		afu->write(MMIO_BASE_ADDR_S0, reinterpret_cast<uint64_t>(this->data[0]));
		afu->write(MMIO_BASE_ADDR_S1, reinterpret_cast<uint64_t>(this->data[1]));
		afu->write(MMIO_BASE_ADDR_S2, reinterpret_cast<uint64_t>(this->data[2]));
		afu->write(MMIO_BASE_ADDR_S3, reinterpret_cast<uint64_t>(this->data[3]));
	}
}
