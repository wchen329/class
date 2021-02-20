#include "test_register.h"

#ifndef WIN32
		void register_test(std::string name, void(*test)())
		{
			runnable_tests.push_back(test);
			test_names.push_back(name);
		}
#endif

void Setup_Tests()
{
	// Add new tests here
	AddTest(test_assert, "test_assert");
	AddTest(test_register_32, "test_register_32");
	AddTest(test_shell_comments, "test_shell_comments");
	AddTest(test_shell_justexit, "test_shell_justexit");
	AddTest(test_shell_multidirectsymbol, "test_shell_multidirect_symbol");
	AddTest(test_sc_cpu_core_instructions, "test_sc_cpu_core_instructions");
	AddTest(test_sc_cpu_loop, "test_sc_cpu_loop");
	AddTest(test_sc_cpu_ops, "test_sc_cpu_ops");
	AddTest(test_sc_cpu_subroutine, "test_sc_cpu_subroutine");
	AddTest(test_sc_cpu_registers, "test_sc_cpu_registers");
	AddTest(test_sc_cpu_storeclose, "test_sc_cpu_storeclose");
	AddTest(test_mips32fsp_load_to_use, "test_mips32fsp_load_to_use");
	AddTest(test_mips32fsp_registers, "test_mips32fsp_registers");
	AddTest(test_mips32fsp_core_instructions, "test_mips32fsp_core_instructions");
	AddTest(test_mips32fsp_subroutine, "test_mips32fsp_subroutine");
	AddTest(test_mips32fsp_nostall, "test_mips32fsp_nostall");
	AddTest(test_mips32fsp_forwarding_basic, "test_mips32fsp_forwarding_basic");
	AddTest(test_mips32fsp_forwarding_control, "test_mips32fsp_forwarding_control");
	AddTest(test_mips32fsp_load_to_use_control, "test_mips32fsp_load_to_use_control");
	AddTest(test_mips32fsp_bigstallforward, "test_mips32fsp_bigstallforward");
	AddTest(test_mips32fsp_memtomem, "test_mips32fsp_memtomem");
	AddTest(test_mips32fsp_storeclose, "test_mips32fsp_storeclose");
	AddTest(test_mips32fsp_fake_hazard, "test_mips32fsp_fake_hazard");
	AddTest(test_mips32fsp_fake_load_to_use, "test_mips32fsp_fake_load_to_use");
	AddTest(test_mips32fsp_ece552_f18midterm6, "test_mips32fsp_ece552_f18midterm6");
	AddTest(test_mips32fsp_ece552_f181009, "test_mips32fsp_ece552_f181009");
	AddTest(test_mips32fsp_fake_load_to_use_r, "test_mips32fsp_fake_load_to_use_r");
	AddTest(test_mips32fsp_memtomem_stall, "test_mips32fsp_memtomem_stall");
}