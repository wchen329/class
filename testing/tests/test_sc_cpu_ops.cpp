#include "basic_assert.h"
#include "mmem.h"
#include "mips.h"
#include "sc_cpu.h"
#include <cstdio>

void test_sc_cpu_ops()
{
	/*
	using namespace priscas;
	mmem cpu_mem;
	sc_cpu cpu(cpu_mem);
	cpu.rst();
	BW_32 pc = cpu.get_PC();
	assertEquals(pc.AsUInt32(), 0);
	cpu.cycle();
	pc = cpu.get_PC();
	assertEquals(pc.AsUInt32(), 4);
	BW_32 inst = generic_mips32_encode(0, 5, -1, NONE, 100, ADDI);	// $5 <- $0 + 100
	cpu_mem[pc.AsUInt32()] = inst.b_0();
	cpu_mem[pc.AsUInt32() + 1] = inst.b_1();
	cpu_mem[pc.AsUInt32() + 2] = inst.b_2();
	cpu_mem[pc.AsUInt32() + 3] = inst.b_3();
	cpu.cycle();
	pc = cpu.get_PC();

	assertEquals(cpu.get_reg_data(5).AsInt32(), 100);

	inst = generic_mips32_encode(0, 5, -1, NONE, 16, SW); // sw $5, $0(16)
	cpu_mem[pc.AsUInt32()] = inst.b_0();
	cpu_mem[pc.AsUInt32() + 1] = inst.b_1();
	cpu_mem[pc.AsUInt32() + 2] = inst.b_2();
	cpu_mem[pc.AsUInt32() + 3] = inst.b_3();
	cpu.cycle();

	BW_32 res = BW_32(cpu_mem[16], cpu_mem[17], cpu_mem[18], cpu_mem[19]);
	BW_32 cmp = 100;
	assertEquals(res, cmp);*/
}