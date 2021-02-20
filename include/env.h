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
#ifndef __ENV_H__
#define __ENV_H__
#include <algorithm>
#include "priscas_global.h"

namespace priscas
{
	/* Env 
	 * -
	 * Describes the current status of various parts of the PRISCAS environment
	 */
	class Env
	{
		public:
			
			/* Mode
			 * Describes PRISCAS current mode of operation
			 *
			 * ASM - assembler mode
			 * IO_DIRECT - direct IO mode (i.e. direct COM input mode)
			 * MACHINE - machine mode
			 * INTERACTIVE - interactive / debugging mode
			 * SUSPENDED - paused
			 * SHUTDOWN - destroyed (i.e. the machine is no longer alive)
			 */
			enum Mode_t
			{
				ASM,
				IO_DIRECT,
				MACHINE,
				INTERACTIVE,
				SHUTDOWN,
				SUSPENDED
			};

			/* Mode_t get_Mode()
			 * Get the current environment mode
			 */
			Mode_t get_Mode() { return this->current_Mode; };
			
			/* void set_Mode(Mode)
			 * Set the environment mode
			 */
			void update_Mode(Mode_t m) { this->current_Mode = m; }

			/* void get_Option_AsmOnly
			 * Return whether or not ASM Input option -i has been specified
			 */
			bool get_Option_AsmInput() { return this->has_Option_AsmInput; }

			/* void get_Option_AsmOnly
			 * Return whether or not ASM Input option -a has been specified
			 */
			bool get_Option_AsmMode() { return this->has_Option_AsmInput; }

			/* void get_Option_AsmInputSpecified
			 * Return whether or not ASM Input option -i had a value specified for it
			 */
			bool get_Option_AsmInputSpecified() { return this->has_AsmInput_Value; }

			/* void get_Option_Help
			 * Return whether or not help option -h has been specified
			 */
			bool get_Option_Help() { return this->has_AsmInput_Value; }

			/* void get_Option_Memwidth
			 * Return whether or not memory width specifier -m has been specified
			 */
			bool get_Option_Memwidth() { return this->has_Option_MemWidth; }

			/* void get_Option_CpuSelect
			 * Return whether or not memory width specifier -c has been specified
			 */
			bool get_Option_CpuSelect() { return this->has_Option_CpuSelect; }

			/* void characterize_Env
			 * Sets the environment given a set of arguments.
			 */
			void characterize_Env(const Arg_Vec&);

			/* const Filename_Vec& get_asmFilenames()
			 * Get the list of filename inputs
			 */
			const Filename_Vec& get_asmFilenames() { return this->asmInputs; }

			/* get_cpuSelectString();
			 * Get All CPU selection strings
			 */
			const Arg_Vec get_cpuStrings() { return this->cpuStrings; }

			/* get_memBitWidth()
			 * Get the addressable memory bitwidth
			 */
			int get_memBitwidth() { return this->mem_bitwidth; }

			/* Env default constructor
			 * Everything is set to false. Put everything else to default values
			 */
			Env() :
				mem_bitwidth(16),
				current_Mode(INTERACTIVE),
				has_Option_AsmMode(false),
				has_Option_AsmInput(false),
				has_Option_Help(false),
				has_Option_MemWidth(false),
				has_Option_CpuSelect(false),
				has_AsmInput_Value(false),
				channel_count(1),
				cpu_count(1)
			{}

		private:

			Filename_Vec asmInputs;		// vector of asm input to assemble (-i)
			Arg_Vec cpuStrings;			// specified cpu strings (-c)

			Mode_t current_Mode;		// current mode of the shell as specified above
			bool has_Option_AsmMode;	// -a option specified
			bool has_Option_AsmInput;	// -i option specified
			bool has_Option_Help;		// -h option specified
			bool has_Option_MemWidth;	// -m option specified
			bool has_Option_CpuSelect;	// -c option specified
			bool has_AsmInput_Value;	// -i option has a value
			unsigned mem_bitwidth;		// memory bitwidth (default 16)
			unsigned channel_count;		// amount of memory channels (currently 1), future use
			unsigned cpu_count;			// amount of cpu sockets (currently 1), future use
	};
}

#endif