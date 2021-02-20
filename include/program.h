#ifndef __PROGRAM_H__
#define __PROGRAM_H__
#include <cstdint>
#include "mt_exception.h"

#define KB 1024
#define MB KB*1024
#define MAX_PROGRAM 64*MB

namespace priscas
{

	/* Program
	 * A program is simple a range of bytes that correspond to
	 * instruction code.
	 */

	class Program
	{
		public:
			uint8_t read(ptrdiff_t addr);
			void write_next(uint8_t data);

			const uint8_t* get_PC_zero() { return prog_range; }

			// Empty program
			Program() : eop(0), eor(MAX_PROGRAM){}

		private:
			// Memory Range
			uint8_t prog_range[MAX_PROGRAM];

			// End of program pointer (non inclusive)
			uint64_t eop;

			// End of range pointer (non-inclusive)
			uint64_t eor;

	};
}

#endif
