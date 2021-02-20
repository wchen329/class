#include "program.h"

namespace priscas
{
	uint8_t Program::read(ptrdiff_t addr)
	{
		if(addr >= eop)
		{
			throw mem_oob_exception();
		}

		return prog_range[addr];
	}

	void Program::write_next(uint8_t data)
	{
		if(eop >= eor)
		{
			throw mt_program_size_violation();
		}

		prog_range[eop] = data;
	}
}
