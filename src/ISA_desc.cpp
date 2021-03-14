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
#include "ISA_desc.h"

namespace priscas
{

	void ISA_desc::interpret(const UPString& linein)
	{
		// First, strip comments and leading whitespace
		UPString linein_1 = StrOp::ws_return(StrOp::strip_comment(linein));

		// If we are at the end, either signal, unexpected end or 
		if(linein_1 == "end")
		{
			if(oms.top() == OM_GLOBAL)
			{
				throw mt_unexpected_end();
			}
			else
			{
				rg_prefix = UPString();
				oms.pop();
				return;
			}
		}

		// If not in global mode, do something else
		switch(oms.top())
		{

			case OM_REGISTER:
			{
				UPString_Vec args = chop_string(linein_1);

				// Split the next strings

				if(args.size() > 2)
				{
					throw mt_reg_toomany();
				}

				// If there is a first argument, take it as a prefix
				if(args.size() == 2)
				{
					rg_prefix = args[0];
					UPString second = args[1];
					args = UPString_Vec();
					args.push_back(second);
				}

				// If there is no prefix, take it as
				// the previous, or throw an error.
				if(args.size() != 0)
				{
					if(rg_prefix == "")
					{
						throw mt_reg_unknown_prefix();
					}
					else
					{
						range genrange(args[0]);
						for(size_t n : genrange)
						{
							UPString suffix = priscas_io::StrTypes::UInt64ToStr(n);	
							UPString reg_name = rg_prefix + suffix;

							rgrps[rgrps.size() - 1].addRegisterName(reg_name);
							
						}
					}
				}

				return;
			}

			case OM_DEFINE:
			{
				
				return;
			}

			case OM_FORMAT:
				return;
		}

		// Then look for direct assignments
		NameValueStringPair nsp = StrOp::tokas(linein_1);
		if(nsp.getName() == "isa")
		{
			this->name = nsp.getValue();
			return;
		}
		else if(nsp.getName() == "endian")
		{
			if(nsp.getValue() == "little")
			{
				this->e_type = ISA_Attrib::CPU_LITTLE_ENDIAN;
				return;
			}
			else if(nsp.getValue() == "big")
			{
				this->e_type = ISA_Attrib::CPU_BIG_ENDIAN;
				return;
			}
			else
			{
				UPString_Vec upsv; upsv.push_back("little");
				upsv.push_back("big");
				throw mt_illegal_value_for(nsp.getName(), nsp.getValue(), upsv);
			}
		}

		// If not any valid direct assignment try something else
		UPString_Vec args = chop_string(linein_1);
		if(args.size() == 0) return;
		if(args[0] == "register")
		{
			ModeSet(OM_REGISTER);

			// We need at least two arguments. Otherwise we are missing
			// the register class namer.
			if(args.size() != 2)
			{
				throw mt_reg_arg_n2();
			}

			Register_Group rg;
			rg.set_rg_name(args[1]);
			rgrps.push_back(rg);

			return;
		}
		else if(args[0] == "define")
		{
			// Define statements. Tricky.
			// Look to see if it's defining a field alias
			// or, if it's defining a field itself.
			UPString_Vec args = chop_string(linein_1);

			// If we only have two arguments, we are only trying
			// to define a new field type
			// Otherwise, we are defining an alias.
			if(args.size() == 2)
			{
				ModeSet(OM_DEFINE);
				if(args.size() != 2)
				{
					throw mt_def_null();
				}

				// TODO: assign a new field with the correct name

				return;
			}
			else
			{
				// Tokenize based on = sign
				NameValueStringPair nvsp = StrOp::tokas(linein_1);

				// If no assignment, this is malformed
				if(nvsp.getName() == "" || nvsp.getValue() == "")
				{
					throw mt_def_alias_malformed();
				}

				// Otherwise define the fields
				UPString_Vec defargs = chop_string(nvsp.getValue());
				UPString_Vec nmargs = chop_string(nvsp.getName());

				UPString& d_type = defargs[0];

				if(d_type == "register")
				{
					if(defargs.size() != 2)
					{
						throw mt_def_reg_arg_err();
					}

					UPString& nm = defargs[1];
					int index = find_reg_group(nm);

					// If non found, then throw exception
					if(index == -1)
					{
						throw mt_def_reg_nonexist();
					}

					// Otherwise, instantiate a register group field
					// Add it to the set of fields
					
					m_Field rfi(new Register_Field(&rgrps[index]));

					if(nmargs.size() != 2)
					{
						throw mt_def_reg_fn_null();
					}

					rfi->set_field_name(nmargs[1]);
					all_fields.push_back(rfi);
				}

				else if(d_type == "literal")
				{
					// In this case, we just add a field type
					// of literal
					m_Field lfi(new Literal_Field());

					if(nmargs.size() != 2)
					{
						throw mt_lit_null();
					}

					lfi->set_field_name(nmargs[1]);
					all_fields.push_back(lfi);
				}
			}

			return;
		}
		else if(args[0] == "format")
		{
			ModeSet(OM_FORMAT);
			return;
		}
		else
		{
			UPString_Vec vec; vec.push_back("register");
			vec.push_back("define"); vec.push_back("format");
			throw mt_illegal_value_for("first argument", args[0], vec);

		}
		
	}

	void ISA_desc::dump(const UPString& hf_name, const UPString& srcf_name)
	{
		FILE* f_h = fopen(hf_name.c_str(), "w");
		FILE* f_s = fopen(srcf_name.c_str(), "w");

		/* Header File
		 * Generation
		 */
		fprintf(f_h, "/* Header File for CLASS\n");
		fprintf(f_h, " * Generated by ISA Gen\n");
		fprintf(f_h, " */\n");

		of_scope inc_global;
		UPString inc_global_b = UPString("#ifndef __")
			+ getName()
			+ UPString("_H__\n#define __")
			+ getName() + UPString("_H__\n");

		UPString inc_global_e = UPString("#endif\n");

		inc_global.set_begin(inc_global_b);
		inc_global.set_end(inc_global_e);


		// Headers
		of_scope_head inc_headers;
		inc_headers.add_header_sys("cstring");
		inc_headers.add_header_sys("cstddef");
		inc_headers.add_header_sys("cstdint");
		inc_headers.add_header_local("ISA.h");
		inc_headers.add_header_local("mt_exception.h");
		inc_headers.add_header_local("primitives.h");
		inc_headers.add_header_local("priscas_global.h");
		inc_headers.add_header_local("syms_table.h");

		// Namespace: priscas
		of_scope inc_ns;
		inc_ns.set_begin("namespace priscas\n{\n");
		inc_ns.set_end("}\n");

		// Arrange Subscopes
		inc_global.add_subscope(inc_headers);
		inc_global.add_subscope(inc_ns);

		// Write the header file
		inc_global.write_back(f_h);

		/* Source File
		 * Generation
		 */
		of_scope src_global;
		of_scope_head src_headers;
		src_headers.add_header_local(hf_name);
		src_global.add_subscope(src_headers);

		of_scope src_ns;
		src_ns.set_begin("namespace priscas\n{\n");
		src_ns.set_end("}\n");

		src_global.add_subscope(src_ns);

		// Write the source file
		src_global.write_back(f_s);

		fclose(f_h);
		fclose(f_s);
	}

	void ISA_desc::ModeSet(OpMode om)
	{
		switch(oms.top())
		{
			case OM_GLOBAL:
				oms.push(om);
				break;
			default:
				throw mt_local_only();
		}
	}

	void ISA_desc::EndMode()
	{
		switch(oms.top())
		{
			case OM_GLOBAL:
				throw mt_unexpected_end();
				break;
			default:
				oms.pop();
		}
	}

	int ISA_desc::find_reg_group(const UPString& search)
	{
		
		int index = -1;
		int iter = 0;

		// Look up the register group of this name
		for(Register_Group rg : rgrps)
		{
			if(rg.get_rg_name() == search)
			{
				index = iter;
				break;
			}

			iter++;
		}

		return index;
	}

	int64_t Literal_Field::get_Binary_Value(const UPString& key)
	{
		int64_t ret = 0;
		int base = 10;

		UPString unprefd;

		// First we need to check for prefixes relating to various bases.
		unprefd = StrOp::has_prefix(key, "0x");		
		if(unprefd != "")
		{
			base = 16;
		}
		else
		{
			unprefd = StrOp::has_prefix(key, "o");		
			if(unprefd != "")
			{
				base = 8;
			}
			else
			{
				// If there are no prefixes, just execute assuming decimal
				unprefd = key;
			}
		}

		ret = strtoll(unprefd.c_str(), nullptr, base);

		if(errno)
		{
			throw mt_bad_imm();
		}
		
		return ret;
	}
}
