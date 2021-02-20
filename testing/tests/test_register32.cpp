#include "basic_assert.h"
#include "primitives.h"
#include "reg_32.h"

void test_register_32()
{
	using namespace priscas;
	reg_32 r;
	r.set_data(150);
	BW_32 value = r.get_data();
	assertEquals(value.AsInt32(), 150);
}