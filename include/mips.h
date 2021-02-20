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
#ifndef __MIPS_H__
#define __MIPS_H__


/* A header for mips specifc details
 * such as register name mappings
 * and a jump list for functional routines
 *
 * Instruction Formats:
 * R - 6 opcode, 5 rs, 5 rt, 5 rd, 5 shamt, 6 funct
 * I - 6 opcode, 5 rs, 5 rt, 16 imm
 * J - 6 opcode, 26 addr
 *
 *
 * wchen329
 */
#include <cstring>
#include <cstddef>
#include <memory>
#include "ISA.h"
#include "mt_exception.h"
#include "primitives.h"
#include "priscas_global.h"
#include "reg_32.h"
#include "syms_table.h"

namespace priscas
{

	// Friendly Register Names -> Numerical Assignments
	enum REGISTERS
	{
		$zero = 0,
		$at = 1,
		$v0 = 2,
		$v1 = 3,
		$a0 = 4,
		$a1 = 5,
		$a2 = 6,
		$a3 = 7,
		$t0 = 8,
		$t1 = 9,
		$t2 = 10,
		$t3 = 11,
		$t4 = 12,
		$t5 = 13,
		$t6 = 14,
		$t7 = 15,
		$s0 = 16,
		$s1 = 17,
		$s2 = 18,
		$s3 = 19,
		$s4 = 20,
		$s5 = 21,
		$s6 = 22,
		$s7 = 23,
		$t8 = 24,
		$t9 = 25,
		$k0 = 26,
		$k1 = 27,
		$gp = 28,
		$sp = 29,
		$fp = 30,
		$ra = 31,
		INVALID = -1
	};

	// instruction formats
	enum format
	{
		R, I, J	
	};

	// MIPS Processor Opcodes
	enum opcode
	{
		R_FORMAT = 0,
		DUMMY = 1,
		JUMP = 2,
		JAL = 3,
		BEQ = 4,
		BNE = 5,
		BLEZ = 6,
		BGTZ = 7,
		ADDI =  8,
		ADDIU = 9,
		SLTI = 10,
		SLTIU = 11,
		ANDI = 12,
		ORI = 13,
		XORI = 14,
		LUI = 15,
		LB = 32,
		LH = 33,
		LWL = 34,
		LW = 35,
		LBU = 36,
		LHU = 37,
		LWR = 38,
		SB = 40,
		SH = 41,
		SWL = 42,
		SW = 43,
		SYS_RES = -1	// system reserved for shell interpreter
	};

	// Function codes for R-Format Instructions
	enum funct
	{
		SLL = 0,
		SRL = 2,
		JR = 8,
		ADD = 32,
		ADDU = 33,
		SUB = 34,
		SUBU = 35,
		AND = 36,
		OR = 37,
		NOR = 39,
		SLT = 42,
		SLTU = 43,
		NONE = -1	// default, if not R format
	};

	int friendly_to_numerical(const char *);

	
	namespace ALU
	{
		enum ALUOp
		{
					ADD = 0,
					SUB = 1,
					SLL = 2,
					SRL = 3,
					OR = 4,
					AND = 5,
					XOR = 6
		};
	}

	/* Generic ALU with four operations:
	 * ADD, SUB, SLL, SRL
	 */
	template <class in_t> class mips_alu
	{

		public:
		in_t execute(ALU::ALUOp op, in_t arg1, in_t arg2, bool unsigned_op)
		{
			in_t ret;

			switch(op)
			{
				case ALU::ADD:
					ret = arg1 + arg2;
					break;
				case ALU::SUB:
					ret = arg1 - arg2;
					break;
				case ALU::SLL:
					ret = arg1 << arg2;
					break;
				case ALU::SRL:
					ret = ((arg1 >> arg2) & ((1 << (32 - arg2)) - 1));
					break;
				case ALU::OR:
					ret = (arg1 | arg2);
					break;
				case ALU::AND:
					ret = (arg1 & arg2);
					break;
				case ALU::XOR:
					ret = (arg1 ^ arg2);
					break;

				default:
				throw new mt_exception();
			}

			return ret;
		}
	};

	/* Decoding unit for MIPS-32
	 *
	 */
	class mips_decoding_unit_32
	{
		public:
			void decode(	BW_32 inst_word,
							format& fm,
							opcode& op,
							int& rs,
							int& rt,
							int& rd,
							funct& funct,
							int32_t& shamt,
							int32_t& imm );
	};

	class mips_cu
	{
		
	};

	// Format check functions
	/* Checks if an instruction is I formatted.
	 */
	bool i_inst(opcode operation);

	/* Checks if an instruction is R formatted.
	 */
	bool r_inst(opcode operation);

	/* Checks if an instruction is J formatted.
	 */
	bool j_inst(opcode operation);

	/* Checks if an instruction performs
	 * memory access
	 */
	bool mem_inst(opcode operation);

	/* Checks if an instruction performs
	 * memory write
	 */
	bool mem_write_inst(opcode operation);

	/* Checks if an instruction performs
	 * memory read
	 */
	bool mem_read_inst(opcode operation);

	/* Checks if an instruction performs
	 * a register write
	 */
	bool reg_write_inst(opcode operation, funct func);

	/* Check if a special R-format
	 * shift instruction
	 */
	bool shift_inst(funct f);

	/* Check if a Jump or
	 * Branch Instruction
	 */
	bool jorb_inst(opcode operation, funct fcode);

	/* "Generic" MIPS-32 architecture
	 * encoding function asm -> binary
	 */
	BW_32 generic_mips32_encode(int rs, int rt, int rd, int funct, int imm_shamt_jaddr, opcode op);

	/* For calculating a label offset in branches
	 */
	BW_32 offset_to_address_br(BW_32 current, BW_32 target);

	/* MIPS_32 ISA
	 *
	 */
	class MIPS_32 : public ISA
	{
		
		public:
			unsigned get_reg_count() { return REG_COUNT; }
			virtual unsigned get_address_bit_width() { return PC_BIT_WIDTH; }
			virtual std::string get_reg_name(int id);
			virtual int get_reg_id(std::string& fr) { return friendly_to_numerical(fr.c_str()); }
			virtual int get_register_bit_width(int id) { return UNIVERSAL_REG_BW; }
			virtual ISA_Attrib::addrmode get_addressing_mode() { return ISA_Attrib::ADDR_BYTE; }
			virtual ISA_Attrib::endian get_endian() { return ISA_Attrib::CPU_LITTLE_ENDIAN; }
			virtual mBW assemble(const Arg_Vec& args, const BW& baseAddress, syms_table& jump_syms) const;
		private:
			static const unsigned REG_COUNT = 32;
			static const unsigned PC_BIT_WIDTH = 32;
			static const unsigned UNIVERSAL_REG_BW = 32;
	};
}

#endif
