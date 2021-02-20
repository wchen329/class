#ifndef __PRISCAS_CORE_H__
#define __PRISCAS_CORE_H__

/* PRISCAS Core Library Include
 * wchen329
 */

// Change all exports to imports
#include "dynamic.h"
#undef LINK_DE
#define LINK_DE LINK_DI

#include "cpu.h"
#include "diag_cpu.h"
#include "primitives.h"
#include "priscas_global.h"
#include "range.h"
#include "shell.h"
#include "streams.h"

#endif