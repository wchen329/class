#include <string>
#include <vector>
#include <cstdio>
#include "basic_assert.h"
#include "sc_cpu.h"
#include "mb.h"
#include "shell.h"

void test_mips32fsp_load_to_use_control()
{
	using namespace priscas;
	std::string prog_name = "pshell";
	std::string flag_i = "-i";
	std::string file_name = "asm/loadtouse_control.s";
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
	BW_32 reg_check_s0 = pipe.get_reg_data(16);
	BW_32 reg_check_s1 = pipe.get_reg_data(17);
	BW_32 reg_check_k0 = pipe.get_reg_data($k0);
	BW_32 reg_check_k1 = pipe.get_reg_data($k1);
	assertEquals(reg_check_s0.AsInt32(), 2000);
	assertEquals(reg_check_s1.AsInt32(), 2000);
	assertNotEquals(reg_check_k0.AsInt32(), 15);
	assertEquals(reg_check_k1.AsInt32(), 15);
}
