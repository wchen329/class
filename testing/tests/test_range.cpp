#include <string>
#include <vector>
#include <cstdio>
#include "basic_assert.h"
#include "range.h"

// Basic range test
void test_range()
{
	using namespace priscas;
	range r("0:5");
	assertEquals(r.atEnd(), false);
	
	// Simple range
	for(int i = 0; i <= 5; ++i)
	{
		assertEquals(r.next(), i);
	}
	assertEquals(r.atEnd(), true);

	// Step != 1 range
	r = range("2:10:2");
	assertEquals(r.atEnd(), false);

	for(int i = 2; i <= 10; i += 2)
	{
		assertEquals(r.next(), i);
	}
	assertEquals(r.atEnd(), true);

	// Backwards Range
	r = range("30:10:-1");
	assertEquals(r.atEnd(), false);

	for(int i = 30; i >= 10; --i)
	{
		assertEquals(r.next(), i);
	}
	assertEquals(r.atEnd(), true);

	// Backwards Range, -4 Step
	r = range("100:5:-4");
	assertEquals(r.atEnd(), false);

	for(int i = 100; i >= 5; i -= 4)
	{
		assertEquals(r.next(), i);
	}
	assertEquals(r.atEnd(), true);

	// Singleton
	r = range("1");
	assertEquals(r.atEnd(), false);
	assertEquals(r.next(), 1);
	assertEquals(r.atEnd(), true);
	
}
