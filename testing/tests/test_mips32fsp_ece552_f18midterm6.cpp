#include <string>
#include <vector>
#include <cstdio>
#include "basic_assert.h"
#include "sc_cpu.h"
#include "mb.h"
#include "shell.h"

void test_mips32fsp_ece552_f18midterm6()
{
	using namespace priscas;
	std::string prog_name = "mtshell";
	std::string flag_i = "-i";
	std::string file_name = "asm/ece552_f18midterm6.s";
	std::string option_c = "-c";
	std::string cpu_spec = "1"; 
	std::vector<std::string> args;
	args.push_back(prog_name);
	args.push_back(flag_i);
	args.push_back(file_name);
	args.push_back(option_c);
	args.push_back(cpu_spec);
	priscas::Shell test_shell;
	test_shell.SetArgs(args);
	test_shell.SetQuiet();
	test_shell.Run();
	mb & test_m = test_shell.GetMotherboard();
	cpu & c = test_m.get_cpu();
	diag_cpu & fsp = dynamic_cast<diag_cpu&>(c);
	assertEquals(fsp.get_reg_data($t0).AsInt32(), 0);
	assertEquals(fsp.get_reg_data($t1).AsInt32(), 1);
	assertEquals(fsp.get_reg_data($s0).AsInt32(), 1002);	
	assertEquals(test_m.get_cycles(), 20);
}
