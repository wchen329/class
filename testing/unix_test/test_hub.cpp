#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>
#include <string>
#include <vector>
#include "test_list.h"
#include "test_register.h"

std::vector<std::string> test_names;
std::vector<void(*)()> runnable_tests;

int main()
{
	fprintf(stdout, "PRISCAS Testing Suite for UNIX and UNIX-Like Systems\n");

	size_t passes = 0;
	int ret_code = 0;
	
	// Add tests here
	Setup_Tests();

	fprintf(stdout, "%ld tests detected...\n", runnable_tests.size());

	fprintf(stdout, "Running tests...\n");

	for(size_t tc = 0; tc < runnable_tests.size(); tc++)
	{
		int ret_value;
		fprintf(stdout, "Running test [%ld]: %s ", tc, test_names[tc].c_str());
		fflush(stdout);

		pid_t id = fork();
		if(id == 0)
		{
			runnable_tests[tc]();
			exit(0);		
		}

		waitpid(id, &ret_value, 0); 
		if(ret_value == 0)
		{
			fprintf(stdout, "PASSED\n");
			passes++;
		}

		else
		{
			fprintf(stdout, "FAILED\n");
			ret_code = 1;
		}
		fflush(stdout);
	}

	fprintf(stdout, "%ld out of %ld tests passed.\n", passes, test_names.size());

	if(ret_code != 0) fprintf(stdout, "Exiting with code 1 [failure]\n");
	else fprintf(stdout, "Exiting with code 0 [success]\n");	
	return ret_code;
}
