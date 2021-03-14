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
#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <memory>
#include <string>
#include "priscas_global.h"

#define BW_MASK_8 0x0FF
#define BW_MASK_16 0x0FFFF
#define BW_MASK_32 0x0FFFFFFFF

/* Various byte long
 * primitives such as a 32-bitlong word
 * or a 64-bitlong one.
 *
 * wchen329
 */

namespace priscas
{
	typedef unsigned char byte_8b;
	

	template<class ConvType, int bitlength> UPString genericHexBuilder(ConvType c)
	{
		UPString ret = "0x";
		int tbl = bitlength;
		
		UPString interm = "";

		while(tbl > 0)
		{
			char val = c & 0x0F;

			switch(val)
			{
				case 0:
					interm = "0" + interm;
					break;
				case 1:
					interm = "1" + interm;
					break;
				case 2:
					interm = "2" + interm;
					break;
				case 3:
					interm = "3" + interm;
					break;
				case 4:
					interm = "4" + interm;
					break;
				case 5:
					interm = "5" + interm;
					break;
				case 6:
					interm = "6" + interm;
					break;
				case 7:
					interm = "7" + interm;
					break;
				case 8:
					interm = "8" + interm;
					break;
				case 9:
					interm = "9" + interm;
					break;
				case 10:
					interm = "a" + interm;
					break;
				case 11:
					interm = "b" + interm;
					break;
				case 12:
					interm = "c" + interm;
					break;
				case 13:
					interm = "d" + interm;
					break;
				case 14:
					interm = "e" + interm;
					break;
				case 15:
					interm = "f" + interm;
					break;
			};


			c = (c >> 4);
			tbl -= 4;
		}

		return ret + interm;
	}

	/* BW
	 * interface for generic nth bit bit-word
	 */
	class BW
	{
		public:
			virtual UPString toHexString() const = 0;

			// Interpret as unsigned 8-bit unsigned integer
			virtual const uint8_t AsUInt8() const = 0;

			// Interpret as unsigned 16-bit integer
			virtual const uint16_t AsUInt16() const = 0;

			// Interpret as unsigned 32-bit unsigned integer
			virtual const uint32_t AsUInt32() const = 0;

			// Interpret as unsigned 64-bit unsigned integer
			virtual const uint64_t AsUInt64() const = 0;

			// Equality and non-equality operations
			virtual bool operator==(const BW& bw2) const = 0;
			virtual bool operator!=(const BW& bw2) const = 0;

			// Bit count
			virtual uint8_t get_bitcount() = 0;

			// Get byte at n
			virtual uint8_t get_nth_byte(uint8_t offset) = 0;

			// Set byte at n
			virtual void set_nth_byte(uint8_t offset, uint8_t data) = 0;

			// Reverse endian
			// On x86/AMD64 machines, the default is little
			virtual void reverse_endian() = 0;
	};

	/* BW_generic
	 * A bitword of different backing types.
	 */
	template<class BW_back, int bitcount> class BW_generic : public BW
	{
		public:
			virtual UPString toHexString() const { return genericHexBuilder<BW_back, bitcount>(this->backing_item); }

			// Interpret as unsigned 8-bit unsigned integer
			virtual const uint8_t AsUInt8() const { return (backing_item) & (BW_MASK_8); }

			// Interpret as unsigned 16-bit integer
			virtual const uint16_t AsUInt16() const  { return (backing_item) & (BW_MASK_16); }

			// Interpret as unsigned 32-bit unsigned integer
			virtual const uint32_t AsUInt32() const  { return (backing_item) & (BW_MASK_32); }

			// Interpret as unsigned 64-bit unsigned integer
			virtual const uint64_t AsUInt64() const  { return (backing_item); }

			// Equality and non-equality operations
			virtual bool operator==(const BW& bw2) const { return bw2.AsUInt64() == this->AsUInt64(); }
			virtual bool operator!=(const BW& bw2) const { return !this->operator==(bw2);}

			// Constructor
			BW_generic(BW_back init) : backing_item(init) {}
			BW_generic() : backing_item(0) {}

			// Assignment
			BW_generic<BW_back, bitcount> operator=(BW_back in) { backing_item = in; }

			// Bit count
			virtual uint8_t get_bitcount() { return bitcount; }

			// Get byte at n
			virtual uint8_t get_nth_byte(uint8_t offset) { return *nth_byte_addr(offset); }

			// Set byte at n
			virtual void set_nth_byte(uint8_t offset, uint8_t data) { *nth_byte_addr(offset) = data; }

			// Reverse endian
			virtual void reverse_endian()
			{
				unsigned bytes = bitcount / 8;
				byte_8b* begin = reinterpret_cast<byte_8b*>(&backing_item);
				byte_8b* end = reinterpret_cast<byte_8b*>((&backing_item)) + bytes;

				std::reverse<byte_8b*>(begin, end);
			}

		private:
			BW_back backing_item;

			inline byte_8b* nth_byte_addr(uint8_t offset)
			{
					return reinterpret_cast<byte_8b*>(&backing_item) + offset;
			}
	};

	typedef BW_generic<uint8_t, 8> BW_8;
	typedef BW_generic<uint16_t, 16> BW_16;
	typedef BW_generic<uint32_t, 32> BW_32;
	typedef BW_generic<uint64_t, 64> BW_64;

	/* Just a collection of two strings
	 * Name - the name of this object
	 * Description - description of this object
	 */
	class NameDescPair
	{
		public:
			std::string& getName() { return this->name; }
			std::string& getDescription() { return this->desc; }
			const std::string& getName() const { return this->name; }
			const std::string& getDescription() const { return this->desc; }
			NameDescPair(std::string name_of, std::string desc_of) : name(name_of), desc(desc_of) {}
		private:
			std::string name;
			std::string desc;
	};

	/* Just a collection of two strings
	 * Name - the name of this object
	 * Value - the string value of this object
	 */
	class NameValueStringPair
	{
		public:
			std::string& getName() { return this->name; }
			std::string& getValue() { return this->value; }
			NameValueStringPair(std::string name_of, std::string value_of) : name(name_of), value(value_of) {}
		private:
			std::string name;
			std::string value;
	};

	/* Bit-Word (Managed)
	 * (mBW)
	 */
	typedef std::shared_ptr<BW> mBW;

}

#endif
