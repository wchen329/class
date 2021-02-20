#include <memory>
#include "basic_assert.h"

void test_assert()
{
	assertEquals(6, 6);
	assertNotEquals(7, 5);
	assertGreaterThan(8, 1);
	assertLessThan(-10, -3);
	assertNotZero(1);
	
	std::auto_ptr<char> cp(new char);
	assertNotNullPtr(cp.get());
	assertIsNullPtr(nullptr);
}