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
#ifndef __MT_EXCEPTION_H__
#define __MT_EXCEPTION_H__
#include <string>
#include "priscas_global.h"

/* MIPS Tools
 * Exception Class
 */

namespace priscas
{
	namespace exception_nums
	{
		enum exception_code
		{
			GENERAL,
			REGISTER_OOB,
			MEMORY_OOB,
			INVALID_RANGE,
			INVALID_CPU_OPT,
			BAD_ESCAPE,
			INVALID_MULTIDEF,
			INSUFFICIENT_ARG,
			BAD_IMM,
			BAD_INST_FORMAT,
			BAD_MNEMONIC,
			ASM_BAD_ARG_COUNT,
			PARSER_UNEXPECTED,
			PARSER_UNMATCHED_PARENTHESIS,
			NO_SUCH_CPU_OPTION,
			IO_FILE_OPEN_FAILURE,
			IO_DEVICE_TYPE_MISMATCH,
			IO_HDD_SEEK_FAILURE,
			IO_HDD_READWRITE_FAILURE
		};
	}


	// General Exception
	class mt_exception
	{
		public:
			int number() { return this->except_num; } // get exception number
			std::string& get_err() { std::string & msg = message; msg = msg + '\n'; return this->message; }	// get full exception name
			mt_exception() : except_num(exception_nums::GENERAL), message("An error has occurred") {}

		protected:
			exception_nums::exception_code except_num;
			std::string message;
	};

	// Register Out of Bounds Exception
	class reg_oob_exception : public mt_exception
	{
		public:
			reg_oob_exception()
			{
				mt_exception::except_num = exception_nums::REGISTER_OOB;
				mt_exception::message = "Invalid registry index given : out of bounds";
			
			}
	};

	// Memory Out of Bounds Exception
	class mem_oob_exception : public mt_exception
	{
		public:
			mem_oob_exception()
			{
				mt_exception::except_num = exception_nums::MEMORY_OOB;
				mt_exception::message = "Invalid memory index given: out of bounds";
			}
	};

	// Invalid Range Specifier Exception
	class mt_invalid_range : public mt_exception
	{
		public:
			mt_invalid_range(const char * info)
			{
				mt_exception::except_num = exception_nums::INVALID_RANGE;
				mt_exception::message = "Invalid range specifer: " + std::string(info);
			}
	};

	// Invalid CPU Option Error
	class mt_invalid_cpu_opt : public mt_exception
	{
		public:
			mt_invalid_cpu_opt(const char * info)
			{
				mt_exception::except_num = exception_nums::INVALID_CPU_OPT;
				mt_exception::message = "Invalid CPU Option: " + std::string(info);
			}
	};

	class mt_bad_escape : public mt_exception
	{
		public:
			mt_bad_escape()
			{
				mt_exception::except_num = exception_nums::BAD_ESCAPE;
				mt_exception::message = "Bad escape sequence (or use of quotations)";
			}
	};

	class mt_multidef_symbol : public mt_exception
	{
		public:
			mt_multidef_symbol(const char* info)
			{
				mt_exception::except_num = exception_nums::BAD_ESCAPE;
				mt_exception::message = "Multiple definition of symbol: " + std::string(info) + " is not allowed";
			}
	};

	class mt_insuff_arg : public mt_exception
	{
		public:
			mt_insuff_arg()
			{
				mt_exception::except_num = exception_nums::INSUFFICIENT_ARG;
				mt_exception::message = "Insufficient amount of arguments provided";
			}
	};

	class mt_bad_imm : public mt_exception
	{
		public:
			mt_bad_imm()
			{
				mt_exception::except_num = exception_nums::BAD_IMM;
				mt_exception::message = "Invalid immediate format";
			}
	};

	class mt_bad_reg_format : public mt_exception
	{
		public:
			mt_bad_reg_format()
			{
				mt_exception::except_num = exception_nums::BAD_INST_FORMAT;
				mt_exception::message = "Bad or unsupported register format";
			}
	};

	class mt_bad_mnemonic : public mt_exception
	{
		public:
			mt_bad_mnemonic()
			{
				mt_exception::except_num = exception_nums::BAD_MNEMONIC;
				mt_exception::message = "Unrecognized instruction mnemonic";
			}
	};

	class mt_asm_bad_arg_count : public mt_exception
	{
		public:
			mt_asm_bad_arg_count()
			{
				mt_exception::except_num = exception_nums::ASM_BAD_ARG_COUNT;
				mt_exception::message = "The instruction to be assembled does not have the correct amount of arguments";
			}
	};

	class mt_parse_unexpected : public mt_exception
	{
		public:
			mt_parse_unexpected(const char * expected, const char * unexpected)
			{
				mt_exception::except_num = exception_nums::PARSER_UNEXPECTED;
				mt_exception::message = std::string(unexpected) + " was unexpected at this time. Expecting: " + std::string(expected);
			}
	};

	class mt_unmatched_parenthesis : public mt_exception
	{
		public:
			mt_unmatched_parenthesis()
			{
				mt_exception::except_num = exception_nums::PARSER_UNMATCHED_PARENTHESIS;
				mt_exception::message = "Unmatched or non-existent required parenthesis";
			}
	};

	class mt_no_such_cpu_option : public mt_exception
	{
		public:
			mt_no_such_cpu_option(const UPString& option_name)
			{
				mt_exception::except_num = exception_nums::NO_SUCH_CPU_OPTION;
				mt_exception::message = (UPString("The option ") + UPString(option_name) + UPString(" does not exist for this CPU."));
			}
	};

	class mt_io_file_open_failure : public mt_exception
	{
		public:
			mt_io_file_open_failure(const UPString& filename)
			{
				mt_exception::except_num = exception_nums::IO_FILE_OPEN_FAILURE;
				mt_exception::message = (UPString("Failed to open file ") + UPString(filename) + UPString("."));
			}
	};

	class mt_io_device_type_mismatch : public mt_exception
	{
		public:
			mt_io_device_type_mismatch()
			{
				mt_exception::except_num = exception_nums::IO_DEVICE_TYPE_MISMATCH;
				mt_exception::message = UPString("A request was sent to a device, but the device does not understand that particular device. The request as a result has been ignored.");
			}

	};

	class mt_io_hdd_rw_io_failure : public mt_exception
	{
		public:
			mt_io_hdd_rw_io_failure()
			{
				mt_exception::except_num = exception_nums::IO_HDD_SEEK_FAILURE;
				mt_exception::message = UPString("A read or write to an HDD failed.");
			}

	};

	class mt_io_hdd_seek_failure : public mt_exception
	{
		public:
			mt_io_hdd_seek_failure()
			{
				mt_exception::except_num = exception_nums::IO_HDD_READWRITE_FAILURE;
				mt_exception::message = UPString("Seeking to the specified address for HDD I/O failed.");
			}
	};
}

#endif
