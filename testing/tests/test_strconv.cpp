#include <string>
#include <vector>
#include <cstdio>
#include "basic_assert.h"
#include "ustrop.h"

// String conversion operation test
void test_strconv()
{
	using namespace priscas;
	// Basic character interpretation tests
	assertEquals(StrOp::get_digit_value('2'), 2);
	assertEquals(StrOp::get_digit_value('0'), 0);
	assertEquals(StrOp::get_digit_value('9'), 9);
	assertEquals(StrOp::get_digit_value('a'), 10);
	assertEquals(StrOp::get_digit_value('f'), 15);

	// Basic string conversion tests
	assertEquals(StrOp::StrToUInt64("0x0"), 0);
	assertEquals(StrOp::StrToUInt64("1"), 1);
	assertEquals(StrOp::StrToUInt64("10"), 10);
	assertEquals(StrOp::StrToUInt64("200"), 200);
	assertEquals(StrOp::StrToUInt32("o10"), 8);
	assertEquals(StrOp::StrToUInt32("o100"), 64);
	assertEquals(StrOp::StrToUInt64("0x52245"), 0x52245);
	assertEquals(StrOp::StrToUInt64("0x52245000001"), 0x52245000001);
}
