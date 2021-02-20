#include <string>
#include <vector>
#include <cstdio>
#include "basic_assert.h"
#include "sc_cpu.h"
#include "mb.h"
#include "shell.h"

void test_mips32fsp_storeclose()
{
	using namespace priscas;
	std::string prog_name = "mtshell";
	std::string flag_i = "-i";
	std::string file_name = "asm/storeclose.s";
	std::string option_c = "-c";
	std::string pipe_select = "1";
	std::vector<std::string> args;
	args.push_back(prog_name);
	args.push_back(flag_i);
	args.push_back(file_name);
	args.push_back(option_c);
	args.push_back(pipe_select);
	Shell test_shell;
	test_shell.SetArgs(args);
	test_shell.SetQuiet();
	test_shell.Run();
	mb & test_m = test_shell.GetMotherboard();
	cpu & c = test_m.get_cpu();
	diag_cpu & single_cycle = dynamic_cast<diag_cpu&>(c);

	// Check addition operations
	assertEquals(single_cycle.get_reg_data($t0).AsInt32(), 1);
	assertEquals(single_cycle.get_reg_data($t1).AsInt32(), 2);
	assertEquals(single_cycle.get_reg_data($t2).AsInt32(), 3);
	assertEquals(single_cycle.get_reg_data($t3).AsInt32(), 4);
	assertEquals(single_cycle.get_reg_data($t4).AsInt32(), 5);
	assertEquals(single_cycle.get_reg_data($t5).AsInt32(), 6);

	// Check Load Values
	assertEquals(single_cycle.get_reg_data($s0).AsInt32(), 1);
	assertEquals(single_cycle.get_reg_data($s1).AsInt32(), 2);
	assertEquals(single_cycle.get_reg_data($s2).AsInt32(), 3);
	assertEquals(single_cycle.get_reg_data($s3).AsInt32(), 4);
	assertEquals(single_cycle.get_reg_data($s4).AsInt32(), 5);
	assertEquals(single_cycle.get_reg_data($s5).AsInt32(), 6);

	// Finally, check cycle count
	assertEquals(test_m.get_cycles(), 23);
}