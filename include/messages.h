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
#ifndef __MESSAGES_H__
#define __MESSAGES_H__
#include <string>
#include "priscas_global.h"

namespace priscas
{
	extern const std::string HELP;	// help message
	extern const std::string HELP_HELP;
	extern const std::string HELP_BREAKPOINT;
	extern const std::string HELP_CPUOPTS;
	extern const std::string HELP_CYCLE;
	extern const std::string HELP_EXIT;
	extern const std::string HELP_MEM;
	extern const std::string HELP_RST;
	extern const std::string HELP_STATE;
	extern const std::string HELP_SR;
	extern const std::string HELP_TIME;
	extern const std::string HELP_RUN;
	extern const char * BAD_COMMAND;  	// bad command message
	extern const char * BAD_FORMAT;		// bad format
	extern const char * NON_DIAG_CPU;	// non diagnostic cpu warning
}

#endif
