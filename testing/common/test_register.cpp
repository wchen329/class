#include "test_register.h"

#ifndef WIN32
		void register_test(std::string name, void(*test)())
		{
			runnable_tests.push_back(test);
			test_names.push_back(name);
		}
#endif

void Setup_Tests()
{
	// Add new tests here
	AddTest(test_assert, "Assert Test");
	AddTest(test_shell_comments, "Comments [not implemented]");
	AddTest(test_shell_justexit, "Exit [not implemented]");
	AddTest(test_shell_multidirectsymbol, "Multi Symbols [not implemented]");
	AddTest(test_reverse_endian, "Reverse Endian Test");
}
