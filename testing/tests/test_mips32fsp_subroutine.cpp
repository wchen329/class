#include <string>
#include <vector>
#include <cstdio>
#include "basic_assert.h"
#include "sc_cpu.h"
#include "mb.h"
#include "shell.h"

void test_mips32fsp_subroutine()
{
	using namespace priscas;
	std::string prog_name = "mtshell";
	std::string flag_i = "-i";
	std::string file_name = "asm/subroutine.s";
	std::string option_c = "-c";
	std::string cpu_select = "1";
	std::vector<std::string> args;
	args.push_back(prog_name);
	args.push_back(flag_i);
	args.push_back(file_name);
	args.push_back(option_c);
	args.push_back(cpu_select);
	priscas::Shell test_shell;
	test_shell.SetArgs(args);
	test_shell.SetQuiet();
	test_shell.Run();
	mb & test_m = test_shell.GetMotherboard();
	cpu & c = test_m.get_cpu();
	diag_cpu & single_cycle = dynamic_cast<diag_cpu&>(c);
	BW_32 reg_check = single_cycle.get_reg_data(16);
	assertEquals(reg_check.AsInt32(), 20);
	assertEquals(test_m.get_cycles(), 29);
}