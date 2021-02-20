#include <string>
#include <vector>
#include <cstdio>
#include "basic_assert.h"
#include "sc_cpu.h"
#include "mb.h"
#include "shell.h"

void test_mips32fsp_registers()
{
	using namespace priscas;
	std::string prog_name = "mtshell";
	std::string flag_i = "-i";
	std::string file_name = "asm/allregisters.s";
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
	diag_cpu & single_cycle = dynamic_cast<diag_cpu&>(c);
	BW_32 reg_check = single_cycle.get_reg_data(0);
	assertEquals(reg_check.AsUInt32(), 0);

	reg_check = single_cycle.get_reg_data(1);
	assertEquals(reg_check.AsUInt32(), 1);

	reg_check = single_cycle.get_reg_data(2);
	assertEquals(reg_check.AsUInt32(), 2);

	reg_check = single_cycle.get_reg_data(3);
	assertEquals(reg_check.AsUInt32(), 3);

	reg_check = single_cycle.get_reg_data(4);
	assertEquals(reg_check.AsUInt32(), 4);

	reg_check = single_cycle.get_reg_data(5);
	assertEquals(reg_check.AsUInt32(), 5);

	reg_check = single_cycle.get_reg_data(6);
	assertEquals(reg_check.AsUInt32(), 6);

	reg_check = single_cycle.get_reg_data(7);
	assertEquals(reg_check.AsUInt32(), 7);

	reg_check = single_cycle.get_reg_data(8);
	assertEquals(reg_check.AsUInt32(), 8);

	reg_check = single_cycle.get_reg_data(9);
	assertEquals(reg_check.AsUInt32(), 9);

	reg_check = single_cycle.get_reg_data(10);
	assertEquals(reg_check.AsUInt32(), 10);

	reg_check = single_cycle.get_reg_data(11);
	assertEquals(reg_check.AsUInt32(), 11);

	reg_check = single_cycle.get_reg_data(12);
	assertEquals(reg_check.AsUInt32(), 12);

	reg_check = single_cycle.get_reg_data(13);
	assertEquals(reg_check.AsUInt32(), 13);

	reg_check = single_cycle.get_reg_data(14);
	assertEquals(reg_check.AsUInt32(), 14);

	reg_check = single_cycle.get_reg_data(15);
	assertEquals(reg_check.AsUInt32(), 15);

	reg_check = single_cycle.get_reg_data(16);
	assertEquals(reg_check.AsUInt32(), 16);

	reg_check = single_cycle.get_reg_data(17);
	assertEquals(reg_check.AsUInt32(), 17);

	reg_check = single_cycle.get_reg_data(18);
	assertEquals(reg_check.AsUInt32(), 18);

	reg_check = single_cycle.get_reg_data(19);
	assertEquals(reg_check.AsUInt32(), 19);

	reg_check = single_cycle.get_reg_data(20);
	assertEquals(reg_check.AsUInt32(), 20);

	reg_check = single_cycle.get_reg_data(21);
	assertEquals(reg_check.AsUInt32(), 21);

	reg_check = single_cycle.get_reg_data(22);
	assertEquals(reg_check.AsUInt32(), 22);

	reg_check = single_cycle.get_reg_data(23);
	assertEquals(reg_check.AsUInt32(), 23);

	reg_check = single_cycle.get_reg_data(24);
	assertEquals(reg_check.AsUInt32(), 24);

	reg_check = single_cycle.get_reg_data(25);
	assertEquals(reg_check.AsUInt32(), 25);

	reg_check = single_cycle.get_reg_data(26);
	assertEquals(reg_check.AsUInt32(), 26);

	reg_check = single_cycle.get_reg_data(27);
	assertEquals(reg_check.AsUInt32(), 27);

	reg_check = single_cycle.get_reg_data(28);
	assertEquals(reg_check.AsUInt32(), 28);

	reg_check = single_cycle.get_reg_data(29);
	assertEquals(reg_check.AsUInt32(), 29);

	reg_check = single_cycle.get_reg_data(30);
	assertEquals(reg_check.AsUInt32(), 30);

	reg_check = single_cycle.get_reg_data(31);
	assertEquals(reg_check.AsUInt32(), 31);
}
