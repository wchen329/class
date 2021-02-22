#include <memory>
#include "primitives.h"
#include "basic_assert.h"

void test_reverse_endian()
{
	using namespace priscas;
	BW_32 bits(0xAABBCCDD);
	BW_32 bits_2(0x018F10F8);
	bits.reverse_endian();
	bits_2.reverse_endian();

	assertEquals(bits.AsUInt32(), 0xDDCCBBAA);
	assertEquals(bits_2.AsUInt32(), 0xF8108F01);
}
