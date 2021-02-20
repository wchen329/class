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
#include "priscas_osi.h"

namespace priscas_osi
{
	mlock::mlock()
	{
		#ifdef WIN32
			InitializeCriticalSection(&this->mutex);
		#else
			this->mutex = PTHREAD_MUTEX_INITIALIZER;
		#endif
	}

	void mlock::lock()
	{
		#ifdef WIN32
			EnterCriticalSection(&this->mutex);
		#else
			pthread_mutex_lock(&this->mutex);
		#endif
	}

	void mlock::unlock()
	{
		#ifdef WIN32
			LeaveCriticalSection(&this->mutex);
		#else
			pthread_mutex_unlock(&this->mutex);
		#endif
	}

	void sleep(int ms)
	{
		#ifdef WIN32
			Sleep(ms);
		#else
			usleep(1000*ms);
		#endif
	}
}
