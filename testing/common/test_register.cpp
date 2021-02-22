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
	AddTest(test_assert, "test_assert");
	AddTest(test_shell_comments, "test_shell_comments");
	AddTest(test_shell_justexit, "test_shell_justexit");
	AddTest(test_shell_multidirectsymbol, "test_shell_multidirect_symbol");
}
