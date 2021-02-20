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
#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__
#include <cctype>
#include <cstdint>
#include <memory>
#include <string>
#include "priscas_global.h"
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
	 * generic nth bit bit-word
	 */
	class LINK_DE BW
	{
		public:
			virtual UPString toHexString() const = 0;

			// Interpret as signed 16-bit integer
			virtual int16_t& AsInt16() = 0;
			virtual const int16_t& AsInt16() const = 0;

			// Interpret as unsigned 16-bit integer
			virtual uint16_t& AsUInt16() = 0;
			virtual const uint16_t& AsUInt16() const = 0;

			// Interpret as signed 32-bit integer
			virtual int32_t& AsInt32() = 0;
			virtual const int32_t& AsInt32() const = 0;
			
			// Interpret as signed 32-bit unsigned integer
			virtual uint32_t& AsUInt32() = 0;
			virtual const uint32_t& AsUInt32() const = 0;

			// Interpret as single precision floating point number 
			virtual float& AsSPFloat() = 0;
			virtual const float& AsSPFloat() const = 0;

			// Equality and non-equality operations
			virtual bool operator==(const BW& bw2) const = 0;
			virtual bool operator!=(const BW& bw2) const = 0;

	};

	class LINK_DE BW_16 : public BW
	{
		public:
			char b_0() { return *(w_addr());}
			char b_1() { return *(w_addr() + 1);}
			BW_16() { w.i16 = 0; }
			BW_16(int16_t data){ w.i16 = data; }
			BW_16(uint16_t data) { w.ui16 = data; }
			BW_16(char b_0, char b_1);

			UPString toHexString() const { return genericHexBuilder<int16_t, 16>(this->w.i16); }
			
			int16_t& AsInt16() { return w.i16; }
			const int16_t& AsInt16() const { return w.i16; }

			uint16_t& AsUInt16() { return w.ui16; }
			const uint16_t& AsUInt16() const { return w.ui16; }

			// 32-bit operations; since this is operating on a 16-bit number, it is currently not well defined
			// THE OPERATION OF THE FOLLOWING IS SUBJECT TO CHANGE (potentially undefined)...
			int32_t& AsInt32() { return w.i32; }
			const int32_t& AsInt32() const { return w.i32; }

			uint32_t& AsUInt32() { return w.ui32; }
			const uint32_t& AsUInt32() const { return w.ui32; }

			float& AsSPFloat() { return w.fp32; }
			const float& AsSPFloat() const { return w.fp32; }
			////////////////// (END undefined cases)

			bool operator==(const BW& bw2) const { return (this->AsInt16() == bw2.AsInt16()); }
			bool operator!=(const BW& bw2) const { return (this->AsInt16() != bw2.AsInt16()); }


		private:
			char * w_addr() { return (char*)&w.i16; }

			union BW_16_internal
			{
				int16_t i16;
				uint16_t ui16;
				int32_t i32;
				uint32_t ui32;
				float fp32;
			};

			
			BW_16_internal w;

	};

	class LINK_DE BW_32 : public BW
	{
		public:
			char b_0() { return *(w_addr());}
			char b_1() { return *(w_addr() + 1);}
			char b_2() { return *(w_addr() + 2);}
			char b_3() { return *(w_addr() + 3);}

			BW_32() { w.i32 = 0; }
			BW_32(int32_t data){ w.i32 = data; }
			BW_32(uint32_t data) { w.ui32 = data; }
			BW_32(float data) { w.fp32 = data; }
			BW_32(char b_0, char b_1, char b_2, char b_3);


			UPString toHexString() const { return genericHexBuilder<int32_t, 32>(this->w.i32); }
			
			const int16_t& AsInt16() const { return w.i16; }
			int16_t& AsInt16() { return w.i16; }
			
			const uint16_t& AsUInt16() const { return w.ui16; }
			uint16_t& AsUInt16() { return w.ui16; }
			
			const int32_t& AsInt32() const { return w.i32; }
			int32_t& AsInt32() { return w.i32; }
			
			const uint32_t& AsUInt32() const { return w.ui32; }
			uint32_t& AsUInt32() { return w.ui32; }

			const float& AsSPFloat() const { return w.fp32; }
			float& AsSPFloat() { return w.fp32; }

			bool operator==(const BW& bw2) const { return (this->AsInt32() == bw2.AsInt32()); }
			bool operator!=(const BW& bw2) const { return (this->AsInt32() != bw2.AsInt32()); }

		private:
			char * w_addr() { return (char*)&w.i32; }
			
			union BW_32_internal
			{
				int16_t i16;
				uint16_t ui16;
				int32_t i32;
				uint32_t ui32;
				float fp32;
			};

			BW_32_internal w;
	};

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

	enum HighLevelType
	{
		T_NONE,
		T_BW32,
		T_INT,
		T_INT32,
		T_UINT,
		T_UINT32,
		T_FLOAT,
		T_DOUBLE,
		T_STRING,
		T_OTHER
	};

	template<class TC> HighLevelType getTypeGeneric()
	{
		if(typeid(TC) == typeid(int))
		{
			return T_INT;
		}

		else if(typeid(TC) == typeid(unsigned))
		{
			return T_UINT;
		}
			
		else if(typeid(TC) == typeid(long) || typeid(TC) == typeid(int32_t))
		{
			return T_INT32;
		}

		else if(typeid(TC) == typeid(unsigned long) || typeid(TC) == typeid(uint32_t))
		{
			return T_UINT32;
		}

		else if(typeid(TC) == typeid(float))
		{
			return T_FLOAT;
		}

		else if(typeid(TC) == typeid(double))
		{
			return T_DOUBLE;
		}

		else if(typeid(T_STRING) == typeid(std::string))
		{
			return T_STRING;
		}
		/*
		else if(typeid(TC) == typeid(BW_16))
		{
			return T_BW16;
		}*/

		else if(typeid(TC) == typeid(BW_32))
		{
			return T_BW32;
		}
		/*
		else if(typeid(TC) == typeid(BW_64))
		{
			return T_BW64;
		}*/
		else
		{
			return T_OTHER;
		}
	}

	
	/* Bit-Word (Managed)
	 * (mBW)
	 */
	typedef std::shared_ptr<BW> mBW;
}

#endif
