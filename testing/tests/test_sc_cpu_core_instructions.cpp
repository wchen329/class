#include "basic_assert.h"
#include "mmem.h"
#include "mips.h"
#include "sc_cpu.h"
#include "shell.h"
#include <cstdio>

void test_sc_cpu_core_instructions()
{
	using namespace priscas;
	std::string prog_name = "mtshell";
	std::string flag_i = "-i";
	std::string file_name = "asm/core_instructions.s";
	std::vector<std::string> args;
	args.push_back(prog_name);
	args.push_back(flag_i);
	args.push_back(file_name);
	priscas::Shell test_shell;
	test_shell.SetArgs(args);
	test_shell.SetQuiet();
	test_shell.Run();

	// Assert operation results
	cpu & raw_cpu = test_shell.GetMotherboard().get_cpu();
	diag_cpu & dcpu = static_cast<diag_cpu&>(raw_cpu);
	assertEquals(dcpu.get_reg_data($s0).AsInt32(), 100);
	assertEquals(dcpu.get_reg_data($t0).AsInt32(), 400);
	assertEquals(dcpu.get_reg_data($t9).AsInt32(), 400);
	assertEquals(dcpu.get_reg_data($at).AsInt32(), 200);
	assertEquals(dcpu.get_reg_data($a3).AsInt32(), 100);
	assertEquals(dcpu.get_reg_data($v0).AsInt32(), 777);
	assertEquals(dcpu.get_reg_data($v1).AsInt32(), 777);
	assertEquals(dcpu.get_reg_data($a0).AsInt32(), -1);
	assertEquals(dcpu.get_reg_data($a1).AsInt32(), 2);
	assertEquals(dcpu.get_reg_data($a2).AsInt32(), 777);
	assertEquals(dcpu.get_reg_data($s1).AsInt32(), 4);
	assertEquals(dcpu.get_reg_data($s2).AsInt32(), 1);
	assertEquals(dcpu.get_reg_data($t1).AsInt32(), 0x000000FF);
	assertEquals(dcpu.get_reg_data($t2).AsInt32(), 0x0000FFFF);
	assertEquals(dcpu.get_reg_data($t3).AsInt32(), -1);
	assertEquals(dcpu.get_reg_data($s3).AsInt32(), 0);
	assertEquals(dcpu.get_reg_data($s4).AsInt32(), 1);
	assertEquals(dcpu.get_reg_data($s5).AsInt32(), 1);
	assertEquals(dcpu.get_reg_data($s6).AsInt32(), 1);
	assertEquals(dcpu.get_reg_data($k0).AsUInt32(), 0);
}