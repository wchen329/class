#ifndef __TEST_LIST_H__
#define __TEST_LIST_H__



// Basic tests
void test_assert();
void test_register_32();
void test_shell_comments();
void test_shell_justexit();
void test_shell_multidirectsymbol();

// Single CPU - MIPS32
void test_sc_cpu_core_instructions();
void test_sc_cpu_loop();
void test_sc_cpu_ops();
void test_sc_cpu_registers();
void test_sc_cpu_subroutine();
void test_sc_cpu_storeclose();

// Five Stage Pipeline - MIPS32
void test_mips32fsp_load_to_use();
void test_mips32fsp_registers();
void test_mips32fsp_core_instructions();
void test_mips32fsp_subroutine();
void test_mips32fsp_nostall();
void test_mips32fsp_forwarding_basic();
void test_mips32fsp_forwarding_control();
void test_mips32fsp_load_to_use_control();
void test_mips32fsp_bigstallforward();
void test_mips32fsp_memtomem();
void test_mips32fsp_storeclose();
void test_mips32fsp_fake_hazard();
void test_mips32fsp_fake_load_to_use();
void test_mips32fsp_ece552_f18midterm6();
void test_mips32fsp_ece552_f181009();
void test_mips32fsp_fake_load_to_use_r();
void test_mips32fsp_memtomem_stall();

#endif
