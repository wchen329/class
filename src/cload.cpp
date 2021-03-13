#include <csignal>
#include <cstdio>
#include "branding.h"
#include "priscas_global.h"
#include "mtsstream.h"
#include "primitives.h"
#include "priscas_global.h"
#include "priscas_osi.h"
#include "program.h"
#include "shell.h"
#include "syms_table.h"
#include "streams.h"

#include "shell_cload.h"

priscas::Shell_Cload runtime;

void break_machine(int arg)
{
	runtime.modeset_Interactive();
	signal(SIGINT, break_machine);
}

int main(int argc, char ** argv)
{
	using namespace priscas;

	Arg_Vec args; // Arguments to Simulator Runtime

	// Simple pass each argument as an argument to the shell.
	for(int carg = 0; carg < argc; carg++)
	{
		args.push_back(UPString(argv[carg]));
	}

	// Set up sigint handler
	signal(SIGINT, break_machine);

	// Set arguments to the ones just built from the passed in argv
	runtime.SetArgs(args);
	runtime.modeset_Interactive();
	
	// Start the shell
	runtime.Run();

	return 0;
}
