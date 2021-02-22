#ifndef __BASIC_ASSERT_H__
#define __BASIC_ASSERT_H__

	#ifndef WIN32
		#include <cassert>
		#define assertEquals(arg_1, arg_2) assert(arg_1 == arg_2);
	
		#define assertNotEquals(arg_1, arg_2) assert(arg_1 != arg_2);
	
		#define assertLessThan(arg_1, arg_2) assert(arg_1 < arg_2);
	
		#define assertGreaterThan(arg_1, arg_2) assert(arg_1 > arg_2);
	
		#define assertNotZero(arg_1) assert(arg_1 != 0);
	
		#define assertIsNullPtr(arg_1) assert(arg_1 == nullptr);
	
		#define assertNotNullPtr(arg_1) assert(arg_1 != nullptr);
	#else
		#include <cstdlib>
		#include <cstdio>
		#include <cwchar>
		extern int __sym_l__;
		extern const char* __sym_f__;

		#define ERR_MSG "Assertion at %s:%d failed.\n"
		#define FillLine __sym_l__ = __LINE__; __sym_f__ = __FILE__;
		#define AssertExpr(expression) if(!(expression)) { FillLine; fprintf(stderr, ERR_MSG, __sym_f__, __sym_l__); exit(1); }

		#define assertEquals(arg_1, arg_2) AssertExpr(arg_1 == arg_2);
	
		#define assertNotEquals(arg_1, arg_2) AssertExpr(arg_1 != arg_2);
	
		#define assertLessThan(arg_1, arg_2) AssertExpr(arg_1 < arg_2);
	
		#define assertGreaterThan(arg_1, arg_2) AssertExpr(arg_1 > arg_2);
	
		#define assertNotZero(arg_1) AssertExpr(arg_1 != 0);
	
		#define assertIsNullPtr(arg_1) AssertExpr(arg_1 == nullptr);
	
		#define assertNotNullPtr(arg_1) AssertExpr(arg_1 != nullptr);
	#endif

#endif