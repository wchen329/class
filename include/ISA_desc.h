#ifndef __ISA_DESC_H__
#define __ISA_DESC_H__
/* ISA Description Code
 */
#include "priscas_global.h"
#include "primitives.h"
#include "shell.h"
#include "ISA.h"
#include "ustrop.h"
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
		private:
			std::vector<UPString> reg_names; 

	};

	class Field
	{
		public:
			typedef std::map<UPString, uint64_t> AB_pair;

			uint64_t get_Binary_Value(const UPString& key){ return value_map.at(key); }
			
			const AB_pair get_KVpairs() { return value_map; }
			

			enum type
			{
				TYPE_MNEMONIC,
				TYPE_LITERAL,
				TYPE_REGISTER
			};
		private:

			// This map is used to lookup various values of a
			// field.
			AB_pair value_map;
	};

	typedef std::vector<Field> Field_Vec;

	class InstFormat
	{
		public:
			void setName(UPString& nin) { name = nin; }	
			const UPString& getName() const { return this->name; }
		private:
			UPString name;
			Field_Vec flds;
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
			ISA_Attrib::endian e_type;
			InstFormat_Vec isa_formats;
			RG_Vec rgrps;
	
			std::stack<OpMode> oms;
			
	};
}

#endif
