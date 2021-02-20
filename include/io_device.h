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
#ifndef __IO_DEVICE_H__
#define __IO_DEVICE_H__
#include <exception>
#include "priscas_global.h"
#include "mt_exception.h"

namespace priscas
{
	// Forward declarations
	class io_request;
	class io_device;

	/* Handshaking Protocol
	 * Provides an interface to handshaking protocol functors.
	 * Just needs to define the application operator
	 */
	class handshaking_protocol
	{
		public:
			/* application operator
			 * Returns true if the handshaking protocol is ready to proceed to the next step
			 * in the I/O process
			 */
			virtual bool operator()() = 0;

			/* ~handshaking_protocol()
			 * virtual destructor
			 */
			virtual ~handshaking_protocol() {}
	};

	/* IO Request
	 * Presented as a functor which is then executed by I/O devices
	 *
	 * This is just an interface, to build off of it generally
	 * - derive this interface into something concrete
	 * - specify the request execution in the implementation's application operator
	 */
	class io_request
	{
		public:

			/* application operator
			 * Perform the operation on a io_device
			 * Return: nothing
			 */
			virtual void operator()(io_device& iod_ref) = 0;
			
			/* ~io_request
			 * virtual destructor
			 */
			virtual ~io_request() {}

		protected:

			/* detectAs()
			 * Provides typed check conversion to a specific device
			 */
			template<class io_dev_sub> io_dev_sub& detectAs(io_device& src)
			{
				try
				{
					return dynamic_cast<io_dev_sub&>(src);
				}

				catch(std::bad_cast&)
				{
					// A little inefficient, but
					// mt io device is an mt_exception and more explicit
					throw mt_io_device_type_mismatch();
				}
			}
	};

	/* IO Device
	 * This provides an interface to I/O devices, which usually talk to the rest of the machine
	 * through a southbridge.
	 * These are intended to include devices such as:
	 *	- Hard Disk Drives
	 *	- Serial / Communication Ports
	 */
	class io_device
	{
		public:

			/* void receive_req(...)
			 * Send a command to the I/O device
			 */
			virtual void receive_req(io_request& in) = 0;

			/* void cycle(ps_passed)
			 * Perform additional work during the cycle (if any).
			 * This can be used to implement multicycle devices.
			 * Sole argument is how many picoseconds passed since the last cycle
			 */
			virtual void cycle(unsigned long long ps_passed) {};

			/* get_handshaking_protocol(...)
			 * Returns a functor to the handshaking protocol
			 */
			//virtual handshaking_protocol& get_handshaking_protocol() = 0;

			/* ~io_device()
			 * virtual destructor
			 */
			virtual ~io_device() {}
	};
}

#endif