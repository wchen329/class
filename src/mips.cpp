//////////////////////////////////////////////////////////////////////////////
//
//    PRISCAS - Computer architecture simulator
//    Copyright (C) 2019 Winor Chen
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////
#include "mips.h"

namespace priscas
{
	int friendly_to_numerical(const char * fr_name)
	{
		int len = strlen(fr_name);
		if(len < 2) return INVALID;

		REGISTERS reg_val
			=
			// Can optimize based off of 
			fr_name[1] == 'a' ?
				!strcmp("$a0", fr_name) ? $a0 :
				!strcmp("$a1", fr_name) ? $a1 :
				!strcmp("$a2", fr_name) ? $a2 :
				!strcmp("$a3", fr_name) ? $a3 :
				!strcmp("$at", fr_name) ? $at : INVALID
			:

			fr_name[1] == 'f' ?
				!strcmp("$fp", fr_name) ? $fp : INVALID
			:

			fr_name[1] == 'g' ?
				!strcmp("$gp", fr_name) ? $gp : INVALID
			:

			fr_name[1] == 'k' ?
				!strcmp("$k0", fr_name) ? $k0 :
				!strcmp("$k1", fr_name) ? $k1 : INVALID
			:

			fr_name[1] == 'r' ?
				!strcmp("$ra", fr_name) ? $ra : INVALID
			:

			fr_name[1] == 's' ?
				!strcmp("$s0", fr_name) ? $s0 :
				!strcmp("$s1", fr_name) ? $s1 :
				!strcmp("$s2", fr_name) ? $s2 :
				!strcmp("$s3", fr_name) ? $s3 :
				!strcmp("$s4", fr_name) ? $s4 :
				!strcmp("$s5", fr_name) ? $s5 :
				!strcmp("$s6", fr_name) ? $s6 :
				!strcmp("$s7", fr_name) ? $s7 :
				!strcmp("$sp", fr_name) ? $sp : INVALID
			:

			fr_name[1] == 't' ?
				!strcmp("$t0", fr_name) ? $t0 :
				!strcmp("$t1", fr_name) ? $t1 :
				!strcmp("$t2", fr_name) ? $t2 :
				!strcmp("$t3", fr_name) ? $t3 :
				!strcmp("$t4", fr_name) ? $t4 :
				!strcmp("$t5", fr_name) ? $t5 :
				!strcmp("$t6", fr_name) ? $t6 :
				!strcmp("$t7", fr_name) ? $t7 :
				!strcmp("$t8", fr_name) ? $t8 :
				!strcmp("$t9", fr_name) ? $t9 : INVALID
			:

			fr_name[1] == 'v' ?
				!strcmp("$v0", fr_name) ? $v0 :
				!strcmp("$v1", fr_name) ? $v1 : INVALID
			:
			fr_name[1] == 'z' ?
				!strcmp("$zero", fr_name) ? $zero : INVALID
			: INVALID;

		return reg_val;
	}

	std::string MIPS_32::get_reg_name(int id)
	{
		std::string name =
			id == 0 ? "$zero" :
			id == 1 ? "$at" :
			id == 2 ? "$v0" :
			id == 3 ? "$v1" :
			id == 4 ? "$a0" :
			id == 5 ? "$a1" :
			id == 6 ? "$a2" :
			id == 7 ? "$a3" :
			id == 8 ? "$t0" :
			id == 9 ? "$t1" :
			id == 10 ? "$t2" :
			id == 11 ? "$t3" :
			id == 12 ? "$t4" :
			id == 13 ? "$t5" :
			id == 14 ? "$t6" :
			id == 15 ? "$t7" :
			id == 16 ? "$s0" :
			id == 17 ? "$s1" :
			id == 18 ? "$s2" :
			id == 19 ? "$s3" :
			id == 20 ? "$s4" :
			id == 21 ? "$s5" :
			id == 22 ? "$s6" :
			id == 23 ? "$s7" :
			id == 24 ? "$t8" :
			id == 25 ? "$t9" :
			id == 26 ? "$k0" :
			id == 27 ? "$k1" :
			id == 28 ? "$gp" :
			id == 29 ? "$sp" :
			id == 30 ? "$fp" :
			id == 31 ? "$ra" : "";
		
		if(name == "")
		{
			throw reg_oob_exception();
		}
		
		return name;
	}

	bool r_inst(opcode operation)
	{
		return
		
			operation == R_FORMAT ? true :
			false ;
	}

	bool i_inst(opcode operation)
	{
		return
			operation == ADDI ? true :
			operation == ADDIU ? true:
			operation == ANDI ? true :
			operation == ORI ? true :
			operation == XORI ? true :
			operation == LB ? true :
			operation == LBU ? true :
			operation == LH ? true :
			operation == LHU ? true :
			operation == LUI ? true :
			operation == LW ? true :
			operation == LWL ? true :
			operation == SB ? true :
			operation == SH ? true :
			operation == SW ? true :
			operation == BEQ ? true :
			operation == BNE ? true :
			operation == BLEZ ? true :
			operation == BGTZ ? true :
			operation == SLTI ? true :
			operation == SLTIU ? true :
			operation == SWL ? true : false ;
	}

	bool j_inst(opcode operation)
	{
		return
			operation == JUMP ? true :
			operation == JAL ? true: false;
	}

	bool mem_inst(opcode operation)
	{
		return
			(mem_write_inst(operation) || mem_read_inst(operation))?
			true : false;
	}

	bool mem_write_inst(opcode operation)
	{
		return
			(operation == SW || operation == SB || operation == SH )?
			true : false;
	}

	bool mem_read_inst(opcode operation)
	{
		return
			(operation == LW || operation == LBU || operation == LHU )?
			true : false;
	}

	bool reg_write_inst(opcode operation, funct func)
	{
		return
			(mem_read_inst(operation)) || (operation == R_FORMAT && func != JR) || (operation == ADDI) || (operation == ORI)
			|| (operation == ANDI) || (operation == XORI) || (operation == SLTI) || (operation == SLTIU) || (operation == ADDIU) || (operation == JAL);
	}

	bool shift_inst(funct f)
	{
		return
			f == SLL ? true :
			f == SRL ? true :
			false;
	}

	bool jorb_inst(opcode operation, funct fcode)
	{
		// First check jumps
		bool is_jump = j_inst(operation);

		bool is_jr = operation == R_FORMAT && fcode == JR;

		bool is_branch =
			operation == BEQ ? true :
			operation == BNE ? true :
			operation == BLEZ ? true :
			operation == BGTZ ? true : false;

		return is_jump || is_branch || is_jr;
	}

	BW_32 generic_mips32_encode(int rs, int rt, int rd, int funct, int imm_shamt_jaddr, opcode op)
	{
		BW_32 w = 0;

		if(r_inst(op))
		{
			w = (w.AsInt32() | (funct & ((1 << 6) - 1)  ));
			w = (w.AsInt32() | ((imm_shamt_jaddr & ((1 << 5) - 1) ) << 6 ));
			w = (w.AsInt32() | ((rd & ((1 << 5) - 1) ) << 11 ));
			w = (w.AsInt32() | ((rt & ((1 << 5) - 1) ) << 16 ));
			w = (w.AsInt32() | ((rs & ((1 << 5) - 1) ) << 21 ));
			w = (w.AsInt32() | ((op & ((1 << 6) - 1) ) << 26 ));
		}

		if(i_inst(op))
		{
			w = (w.AsInt32() | (imm_shamt_jaddr & ((1 << 16) - 1)));
			w = (w.AsInt32() | ((rt & ((1 << 5) - 1) ) << 16 ));
			w = (w.AsInt32() | ((rs & ((1 << 5) - 1) ) << 21 ));
			w = (w.AsInt32() | ((op & ((1 << 6) - 1) ) << 26 ));
		}

		if(j_inst(op))
		{
			w = (w.AsInt32() | (imm_shamt_jaddr & ((1 << 26) - 1)));
			w = (w.AsInt32() | ((op & ((1 << 6) - 1) ) << 26 ));
		}

		return w;
	}

	BW_32 offset_to_address_br(BW_32 current, BW_32 target)
	{
		BW_32 ret = target.AsInt32() - current.AsInt32();
		ret = ret.AsInt32() - 4;
		ret = (ret.AsInt32() >> 2);
		return ret;
	}

	void mips_decoding_unit_32::decode(	BW_32 inst,
										format& fm,
										opcode& op,
										int& rs,
										int& rt,
										int& rd,
										funct& func,
										int32_t& shamt,
										int32_t& imm )
	{
		int32_t inst_word = inst.AsInt32();

		// -Masks-
		int32_t opcode_mask = (~(1 << 26)) + 1;
		int32_t rs_mask = ~( ((~(1 << 26)) + 1) | ((1 << 21) - 1));
		int32_t rt_mask = ~( ((~(1 << 21)) + 1) | ((1 << 16) - 1));
		int32_t rd_mask = ~( ((~(1 << 16)) + 1) | ((1 << 11) - 1));
		int32_t funct_mask = 63;
		int32_t shamt_mask = (1 << 11) - 1 - funct_mask;
		int32_t imm_mask_i = (1 << 16) - 1;
		int32_t addr_mask = (1 << 26) - 1;

		// - Actual values
		op = static_cast<opcode>(((opcode_mask & inst_word) >> 26) & ((1 << 6) - 1));

		// Set a mode based on OP
		if(op == R_FORMAT) fm = R;
		else if(j_inst(static_cast<opcode>(op))) fm = J;
		else fm = I;

		// Then decode!
		rs = (rs_mask & inst_word) >> 21;
		rt = (rt_mask & inst_word) >> 16;
		rd = (rd_mask & inst_word) >> 11;
		func = static_cast<funct>((funct_mask & inst_word));
		shamt = (shamt_mask & inst_word) >> 6;
		imm = fm == I	? (imm_mask_i & inst_word) | (~(inst_word & (1 << 15)) + 1)
						: (addr_mask & inst_word) | (~(inst_word & (1 << 25)) + 1); // make it signed
	}
}
