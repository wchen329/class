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
#ifndef __ISA_H__
#define __ISA_H__

/* An abstract class describing
 * an interface for an Instruction Set Architecture
 *
 * wchen329
 */
#include <memory>
#include <string>
#include <vector>
#include "priscas_global.h"
#include "syms_table.h"

namespace priscas
{
	/* ISA_Attrib
	 * Contains usable values for ISA interface functions below
	 */
	namespace ISA_Attrib
	{

		// Endian. Tells how the CPU to assemble
		enum endian
		{
			CPU_LITTLE_ENDIAN,
			CPU_BIG_ENDIAN
		};

	}

	/* ISA
	 * describes an ISA a.k.a
	 * instruction set architecture
	 */
	class ISA
	{
		public:
			
			/* get_reg_name(int id)
			 * Returns the common or "friendly" name of a register given an ID
			 * Return:	PString
			 *			if name is valid, the register ID
			 */
			virtual UPString get_reg_name(int id) = 0;

			/* get_reg_id(PString&)
			 * Returns the register ID given a common or "friendly" name of a register
			 * Return:	PString
			 *			if ID is valid: name of the register
			 */
			virtual int get_reg_id(UPString&) = 0;
			
			/* get_endian()
			 * Get the endian of the ISA
			 * Return:	ISA_Attrib::endian
			 *			ISA endian
			 */
			virtual ISA_Attrib::endian get_endian() = 0;
			
			/* interpret(arg ...)
			 * Assemble an instruction
			 * Return:	mBW
			 *			bitword corresponding to assembled instruction
			 */
			virtual mBW assemble(const Arg_Vec& args,
				const BW& base_address,
				syms_table& jump_syms) const = 0;
	};
}
#endif
