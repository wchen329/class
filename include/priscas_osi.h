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
