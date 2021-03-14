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
#ifndef __PRISCAS_OSI_H__
#define __PRISCAS_OSI_H__
#include "priscas_global.h"

#ifdef WIN32
	#include <Windows.h>
	typedef CRITICAL_SECTION OS_lock_t;
#else
	#include <pthread.h>
	#include <unistd.h>
	typedef pthread_mutex_t OS_lock_t;
#endif

/* OS Interfacing
 */
namespace priscas_osi
{

	/* mlock
	 * A UNIX and Win32 compatible Mutex / critical section
	 * The Mutex is automatically initialized- no need to initialize it.
	 * It's always unlocked to begin with...
	 */
	class mlock
	{
		private:
			OS_lock_t mutex;

		public:
			void lock();
			void unlock();
			mlock();
	};

	/* (Threading)
	 * Sleep a thread for the given amount of milliseconds
	 */
	void sleep(int ms);
}

#endif
