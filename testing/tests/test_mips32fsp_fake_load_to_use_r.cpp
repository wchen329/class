#include <string>
#include <vector>
#include <cstdio>
#include "basic_assert.h"
#include "sc_cpu.h"
#include "mb.h"
#include "shell.h"

void test_mips32fsp_fake_load_to_use_r()
{
	using namespace priscas;
	std::string prog_name = "pshell";
	std::string flag_i = "-i";
	std::string file_name = "asm/fake_load_to_use_r.s";
	std::string cpuspec = "-c";
	std::string cpunum = "1";
	std::vector<std::string> args;
	args.push_back(prog_name);
	args.push_back(flag_i);
	args.push_back(file_name);
	args.push_back(cpuspec);
	args.push_back(cpunum);
	priscas::Shell test_shell;
	test_shell.SetArgs(args);
	test_shell.SetQuiet();
	test_shell.Run();
	mb & test_m = test_shell.GetMotherboard();
	cpu & c = test_m.get_cpu();
	diag_cpu & pipe = dynamic_cast<diag_cpu&>(c);
	BW_32 reg_check_zero = pipe.get_reg_data($zero);
	assertEquals(reg_check_zero.AsInt32(), 0);
	assertEquals(test_m.get_cycles(), 7);
}
