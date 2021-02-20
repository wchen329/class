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
#ifndef __SYMS_TABLE_H__
#define __SYMS_TABLE_H__
#include <cstddef>
#include <cstring>
#include <map>
#include <string>
#include <vector>
#include "priscas_global.h"
#include "primitives.h"
#include "mt_exception.h"

namespace priscas
{
	/* Wrapper class for the symbols table
	 *
	 */
	class syms_table
	{
		/* There can be many symbols assigned to one PC
		 * However, only one PC can be assigned to a symbol
		 *
		 */

		public:
			void insert(std::string key, unsigned long PC);
			bool has(std::string key) { return this->syms_to_PC.count(key) > 0; };
			bool has(unsigned long pc) { return this->PC_to_syms.count(pc) > 0; }
			std::vector<std::string>& lookup_from_PC(unsigned long PC) { return this->PC_to_syms.at(PC); }
			unsigned long lookup_from_sym(const std::string & sym) { return this->syms_to_PC.at(sym); }
		private:
			std::map<uint32_t, std::vector<std::string>> PC_to_syms;
			std::map<std::string, uint32_t> syms_to_PC;
			
	};

	/* Monodirectional Symbol Table
	 * This symbol table only allows lookup from PC to symbols.
	 * However, this way a symbol can be assigned to many PCs (such as directives which are not necessarily unique).
	 */
	class mono_syms_table
	{
		public:
			void insert(std::string key, unsigned long PC);
			bool has(unsigned long pc) { return this->PC_to_syms.count(pc) > 0; }
			std::vector<std::string>& lookup_from_PC(unsigned long PC) { return this->PC_to_syms.at(PC); }
		private:
			std::map<uint32_t, std::vector<std::string>> PC_to_syms;
	};
}

#endif
