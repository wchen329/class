//////////////////////////////////////////////////////////////////////////////
//
//    CLASS - Cloud Loader and ASsembler System
//    Copyright (C) 2021 Winor Chen
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
			w = (w.AsUInt32() | (funct & ((1 << 6) - 1)  ));
			w = (w.AsUInt32() | ((imm_shamt_jaddr & ((1 << 5) - 1) ) << 6 ));
			w = (w.AsUInt32() | ((rd & ((1 << 5) - 1) ) << 11 ));
			w = (w.AsUInt32() | ((rt & ((1 << 5) - 1) ) << 16 ));
			w = (w.AsUInt32() | ((rs & ((1 << 5) - 1) ) << 21 ));
			w = (w.AsUInt32() | ((op & ((1 << 6) - 1) ) << 26 ));
		}

		if(i_inst(op))
		{
			w = (w.AsUInt32() | (imm_shamt_jaddr & ((1 << 16) - 1)));
			w = (w.AsUInt32() | ((rt & ((1 << 5) - 1) ) << 16 ));
			w = (w.AsUInt32() | ((rs & ((1 << 5) - 1) ) << 21 ));
			w = (w.AsUInt32() | ((op & ((1 << 6) - 1) ) << 26 ));
		}

		if(j_inst(op))
		{
			w = (w.AsUInt32() | (imm_shamt_jaddr & ((1 << 26) - 1)));
			w = (w.AsUInt32() | ((op & ((1 << 6) - 1) ) << 26 ));
		}

		return w;
	}

	BW_32 offset_to_address_br(BW_32 current, BW_32 target)
	{
		BW_32 ret = target.AsUInt32() - current.AsUInt32();
		ret = ret.AsUInt32() - 4;
		ret = (ret.AsUInt32() >> 2);
		return ret;
	}

	// Main interpretation routine
	mBW MIPS_32::assemble(const Arg_Vec& args, const BW& baseAddress, syms_table& jump_syms) const
	{
		if(args.size() < 1)
			return std::shared_ptr<BW>(new BW_32());

		priscas::opcode current_op = priscas::SYS_RES;
		priscas::funct f_code = priscas::NONE;

		int rs = 0;
		int rt = 0;
		int rd = 0;
		int imm = 0;

		// Mnemonic resolution
		
		if("add" == args[0]) { current_op = priscas::R_FORMAT; f_code = priscas::ADD; }
		else if("addiu" == args[0]) { current_op = priscas::ADDIU; }
		else if("addu" == args[0]) { current_op = priscas::R_FORMAT; f_code = priscas::ADDU; }
		else if("addi" == args[0]) { current_op = priscas::ADDI; }
		else if("beq" == args[0]) { current_op = priscas::BEQ; }
		else if("bne" == args[0]) { current_op = priscas::BNE; }
		else if("sub" == args[0]) { current_op = priscas::R_FORMAT; f_code = priscas::SUB; }
		else if("and" == args[0]) { current_op = priscas::R_FORMAT; f_code = priscas::AND; }
		else if("andi" == args[0]) { current_op = priscas::ANDI; }
		else if("or" == args[0]) { current_op = priscas::R_FORMAT; f_code = priscas::OR; }	
		else if("ori" == args[0]) { current_op = priscas::ORI; }	
		else if("nor" == args[0]) { current_op = priscas::R_FORMAT; f_code = priscas::NOR; }	
		else if("xori" ==  args[0]) { current_op = priscas::XORI; }
		else if("lbu" == args[0]) { current_op = priscas::LBU; }
		else if("lhu" == args[0]) { current_op = priscas::LHU; }
		else if("lw" == args[0]) { current_op = priscas::LW; }
		else if("sb" == args[0]) { current_op = priscas::SB; }
		else if("sh" == args[0]) { current_op = priscas::SH; }
		else if("sw" == args[0]) { current_op = priscas::SW; }
		else if("sll" == args[0]) { current_op = priscas::R_FORMAT; f_code = priscas::SLL; }
		else if("srl" == args[0]) { current_op = priscas::R_FORMAT; f_code = priscas::SRL; }
		else if("slt" == args[0]) { current_op = priscas::R_FORMAT; f_code = priscas::SLT; }	
		else if("slti" == args[0]) { current_op = priscas::SLTI;}
		else if("sltiu" == args[0]) { current_op = priscas::SLTIU; }	
		else if("sltu" == args[0]) { current_op = priscas::R_FORMAT; f_code = priscas::SLTU; }
		else if("subu" == args[0]) { current_op = priscas::R_FORMAT; f_code = priscas::SUBU; }
		else if("j" == args[0]) { current_op = priscas::JUMP;}
		else if("jal" == args[0]) { current_op = priscas::JAL;}	
		else if("jr" == args[0]) { current_op = priscas::R_FORMAT; f_code = priscas::JR;}
		else
		{
			throw mt_bad_mnemonic();
		}

		// Check for insufficient arguments
		if(args.size() >= 1)
		{
			if	(
					(r_inst(current_op) && args.size() != 4 && f_code != priscas::JR) ||
					(r_inst(current_op) && args.size() != 2 && f_code == priscas::JR) ||
					(i_inst(current_op) && args.size() != 4 && !mem_inst(current_op)) ||
					(i_inst(current_op) && args.size() != 3 && mem_inst(current_op)) ||
					(j_inst(current_op) && args.size() != 2)				
				)
			{
				throw priscas::mt_asm_bad_arg_count();
			}

			// Now first argument parsing
			if(r_inst(current_op))
			{
					if(f_code == priscas::JR)
					{
						if((rs = priscas::friendly_to_numerical(args[1].c_str())) <= priscas::INVALID)
						rs = priscas::get_reg_num(args[1].c_str());
					}

					else
					{
						if((rd = priscas::friendly_to_numerical(args[1].c_str())) <= priscas::INVALID)
						rd = priscas::get_reg_num(args[1].c_str());
					}
			}

			else if(i_inst(current_op))
			{
				// later, check for branches
				if((rt = priscas::friendly_to_numerical(args[1].c_str())) <= priscas::INVALID)
				rt = priscas::get_reg_num(args[1].c_str());
			}

			else if(j_inst(current_op))
			{
				if(jump_syms.has(args[1]))
				{
					priscas::BW_32 label_PC = static_cast<int32_t>(jump_syms.lookup_from_sym(std::string(args[1].c_str())));
					imm = (label_PC.AsUInt32() >> 2);
				}

				else
				{
					imm = priscas::get_imm(args[1].c_str());
				}
			}
	
			else
			{
				priscas::mt_bad_mnemonic();
			} 
		}

		// Second Argument Parsing
		
		if(args.size() > 2)
		{
			if(r_inst(current_op))
			{
				if (f_code != priscas::JR)
				{
					if((rs = priscas::friendly_to_numerical(args[2].c_str())) <= priscas::INVALID)
						rs = priscas::get_reg_num(args[2].c_str());
				}
			}
						
			else if(i_inst(current_op))
			{
				if(mem_inst(current_op))
				{
					bool left_parenth = false; bool right_parenth = false;
					std::string wc = args[2];
					std::string imm_s = std::string();
					std::string reg = std::string();

					for(size_t i = 0; i < wc.length(); i++)
					{
						if(wc[i] == '(') { left_parenth = true; continue; }
						if(wc[i] == ')') { right_parenth = true; continue; }

						if(left_parenth)
						{
							reg.push_back(wc[i]);
						}

						else
						{
							imm_s.push_back(wc[i]);
						}
					}

					if(!right_parenth || !left_parenth) throw mt_unmatched_parenthesis();
					if((rs = priscas::friendly_to_numerical(reg.c_str())) <= priscas::INVALID) rs = priscas::get_reg_num(reg.c_str());
					imm = priscas::get_imm(imm_s.c_str());
								
				}

				else
				{
					// later, MUST check for branches
					if((rs = priscas::friendly_to_numerical(args[2].c_str())) <= priscas::INVALID)
					rs = priscas::get_reg_num(args[2].c_str());
				}
			}

			else if(j_inst(current_op)){}
		}

		if(args.size() > 3)
		{
			// Third Argument Parsing
			if(r_inst(current_op))
			{
				if(f_code != priscas::JR)
				{
					if(shift_inst(f_code))
					{
						imm = priscas::get_imm(args[3].c_str());
					}

					else
					{	
						if((rt = priscas::friendly_to_numerical(args[3].c_str())) <= priscas::INVALID)
							rt = priscas::get_reg_num(args[3].c_str());
					}
				}
			}
						
			else if(i_inst(current_op))
			{

				if(jump_syms.has(args[3]))
				{
					priscas::BW_32 addr = baseAddress.AsUInt32();
					priscas::BW_32 label_PC = static_cast<uint32_t>(jump_syms.lookup_from_sym(std::string(args[3].c_str())));
					imm = priscas::offset_to_address_br(addr, label_PC).AsUInt32();
				}

				else
				{
					imm = priscas::get_imm(args[3].c_str());
				}
			}

			else if(j_inst(current_op)){}
		}

		// Pass the values of rs, rt, rd to the processor's encoding function
		BW_32 inst = generic_mips32_encode(rs, rt, rd, f_code, imm, current_op);

		return std::shared_ptr<BW>(new BW_32(inst));
	}

	// Returns register number corresponding with argument if any
	// Returns -1 if invalid or out of range
	int get_reg_num(const char * reg_str)
	{
		std::vector<char> numbers;
		int len = strlen(reg_str);
		if(len <= 1) throw priscas::mt_bad_imm();
		if(reg_str[0] != '$') throw priscas::mt_parse_unexpected("$", reg_str);
		for(int i = 1; i < len; i++)
		{
			if(reg_str[i] >= '0' && reg_str[i] <= '9')
			{
				numbers.push_back(reg_str[i]);
			}

			else throw priscas::mt_bad_reg_format();
		}

		int num = -1;

		if(numbers.empty()) throw priscas::mt_bad_reg_format();
		else
		{
			char * num_str = new char[numbers.size()];

			int k = 0;
			for(std::vector<char>::iterator itr = numbers.begin(); itr < numbers.end(); itr++)
			{
				num_str[k] = *itr;
				k++;
			}
			num = atoi(num_str);
			delete[] num_str;
		}

		return num;
	}

	// Returns immediate value if valid
	int get_imm(const char * str)
	{
		return StrOp::StrToUInt32(UPString(str));
	}
}
