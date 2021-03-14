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
#ifndef __ISA_DESC_H__
#define __ISA_DESC_H__

/* ISA Description Code
 */
#include "priscas_global.h"
#include "primitives.h"
#include "shell.h"
#include "ISA.h"
#include "range.h"
#include "streams.h"
#include "ustrop.h"
#include <cerrno>
#include <map>
#include <vector>
#include <stack>
namespace priscas
{
	/* Things that could be part of an ISA
	 */

	/* In a register group,
	 * the id which corresponds to a register is
	 * implicit (in order which they are given)
	 */
	class Register_Group
	{
		public:
			void addRegisterName(const UPString& name)
			{
				reg_names.push_back(name);
			}
			const UPString& getNthName(int n){ return reg_names[n]; }
			unsigned getCount() { return reg_names.size(); }
			const UPString& get_rg_name() { return rg_name; }
			void set_rg_name(const UPString& namein) { rg_name = namein; }
		private:
			std::vector<UPString> reg_names; 
			UPString rg_name;
	};

	class Field
	{
		public:

			// Name
			const UPString& get_field_name() const { return this->field_name; }			

			// Set Name
			void set_field_name(const UPString& us) { this->field_name = us; }			

			// Get the binary part
			virtual int64_t get_Binary_Value(const UPString& key) = 0;

			// Get the string part
			virtual const UPString& get_String_Value(int64_t key) = 0;

			// Get the total number of int-string pairs
			virtual size_t get_num() = 0;

		private:
			UPString field_name; // Every field has a name

	};

	class Register_Field : public Field
	{
		public:
			Register_Field(Register_Group* backing)
			: rg_internal(backing)
			{
			}

			// Get the binary part
			virtual int64_t get_Binary_Value(const UPString& key)
			{
				// Have to do a linear search for this one
				int index = 0;
				bool found = false;
				
				for(int wh = 0; wh < rg_internal->getCount(); ++wh)
				{
					// If match, set.
					if(rg_internal->getNthName(wh) == key)
					{
						found = true;
						break;
					}

					++index;
				}

				if(!found)
				{
					throw mt_exception();
				}

				return index;
			}

			// Get the string part
			virtual const UPString& get_String_Value(int64_t key)
			{
				return rg_internal->getNthName(key);
			}

			// Get the total number of int-string pairs
			virtual size_t get_num() { return rg_internal->getCount(); }

		private:
			Register_Group* rg_internal;
	};

	typedef std::shared_ptr<Field> m_Field;
	typedef std::vector<m_Field> m_Field_Vec;

	class Literal_Field : public Field
	{
		public:

			// Get the binary part
			virtual int64_t get_Binary_Value(const UPString& key);

			// Get the string part
			virtual const UPString& get_String_Value(int64_t key)
			{
				retbuf = priscas_io::StrTypes::Int64ToStr(key);
				return retbuf;
			}

			// Get the total number of pairs, 0 is too much!
			virtual size_t get_num() { return 0; }

		private:
			UPString field_name; // Every field has a name
			UPString retbuf;
	};

	class InstFormat
	{
		public:
			void setName(UPString& nin) { name = nin; }	
			const UPString& getName() const { return this->name; }
		private:
			UPString name;
			m_Field_Vec flds;
			std::vector<unsigned> shift_offset;
			unsigned i_length; // length of the format
	};
	typedef std::vector<InstFormat> InstFormat_Vec;

	/* scope
	 * A scope in the output file
	 */
	class of_scope
	{
		public:
			void set_begin(const UPString& sbegin) { this->begin = sbegin;  }
			void set_end(const UPString& send) { this->end = send; }

			void add_subscope(of_scope& scin) { sub_scopes.push_back(scin); }

			void write_back(FILE* os)
			{
				fprintf(os, "%s", begin.c_str());
				
				// Write all the subscopes... all the subscope...
				for(of_scope sc : sub_scopes)
				{
					sc.write_back(os);
				}

				fprintf(os, "%s", end.c_str());
			}

		protected:
			UPString& get_begin() { return this->begin; }
			UPString& get_end() { return this->end; }
		private:

			UPString begin;
			UPString end;

			std::vector<of_scope> sub_scopes;

	};

	/* of_scope_head
	 * A header scope
	 */
	class of_scope_head : public of_scope
	{
		public:
			void add_header_local(const UPString& in)
			{
				// TODO: dedup by using generics 
				of_scope::set_begin(of_scope::get_begin() + 
					UPString("#include \"") +
					in +
					UPString("\"\n")
				);
			}
			void add_header_sys(const UPString& in)
			{
				of_scope::set_begin(of_scope::get_begin() + 
					UPString("#include <") +
					in +
					UPString(">\n")
				);
			}
	};

	/* ISA Description
	 * An ISA description which can be used to generate a set
	 * of header/source files to plug into the CLASS system.
	 */
	class ISA_desc
	{
		public:
			// Interpret function
			void interpret(const UPString& linein);

			// Dump Function
			void dump(const UPString& hf_name, const UPString& srcf_name);
			const UPString& getName() { return this->name; }

			enum OpMode
			{
				OM_GLOBAL,
				OM_REGISTER,
				OM_DEFINE,
				OM_FORMAT
			};

			ISA_desc() { oms.push(OM_GLOBAL); }

			void ModeSet(OpMode om);
			void EndMode();
			
		private:
			typedef std::vector<Register_Group> RG_Vec;
			UPString name;
			UPString reg_prefix;
			ISA_Attrib::endian e_type;
			InstFormat_Vec isa_formats;
			m_Field_Vec all_fields;
			RG_Vec rgrps;

			// Linear search, return index
			int find_reg_group(const UPString& search);
	
			std::stack<OpMode> oms;

			// State variables
			UPString rg_prefix;
			
	};
}

#endif
