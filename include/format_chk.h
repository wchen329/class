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
#ifndef __FORMAT_CHK_H__
#define __FORMAT_CHK_H__

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <memory>
#include <vector>
#include "messages.h"
#include "mt_exception.h"
#include "priscas_global.h"

namespace priscas
{

	// From a register specifier, i.e. %so get an integer representation
	int get_reg_num(const char *);
	
	// From a immediate string, get an immediate value.
	int get_imm(const char *);

}

#endif
