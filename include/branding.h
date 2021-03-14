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
#ifndef __BRANDING_H__
#define __BRANDING_H__
#include "priscas_global.h"

// These predefines are specifically used for Windows Resource Compiler
#define PRISCAS_BRANDING_AUTHOR "Winor Chen"
#define PRISCAS_BRANDING_FULL_VERSION_STRING "0.4.0.10000"
#define PRISCAS_BRANDING_COPYRIGHT "Copyright (c) 2019 Winor Chen"
#define PRISCAS_BRANDING_FULL_VERSION 0,4,0,10000

/* Header that includes information about project name, description, and versioning, etc., etc.
 * wchen329
 */
namespace branding
{
	const std::string APPLICATION_NAME = std::string("CLASS");
	const std::string APPLICATION_DESC = std::string("[assembler]");
	const std::string APPLICATION_VERSION = std::string("1.0");

	#ifdef P_RELEASE
		const std::string APPLICATION_DBG = "(stable)";
	#else
		const std::string APPLICATION_DBG = "(debug build)";
	#endif
}


#endif
