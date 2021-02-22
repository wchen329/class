#ifndef __TEST_REGISTER_H__
#define __TEST_REGISTER_H__
#include "test_list.h"
#include <string>
#include <vector>

	#ifdef WIN32
		#include "test_ds.h"
		extern test_collection online_tests;
		#define AddTest(test_ptr, test_namestr) online_tests.add_test(test_ptr, test_namestr)
	#else
		extern std::vector<std::string> test_names;
		extern std::vector<void(*)()> runnable_tests;

		void register_test(std::string name, void(*test)());
		
		#define AddTest(test_ptr, test_namestr) register_test(test_namestr, test_ptr)

	#endif

	void Setup_Tests();

#endif