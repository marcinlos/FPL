#include "config.h"
#include <FPL/float.h>

uint64_t FPL_add_implicit_one(uint64_t mantissa)
{
	return (((uint64_t) 1) << FPL_MANTISSA_SIZE_64) | mantissa;
}

int FPL_highest_nonzero_bit(uint64_t x)
{
	return 0;
}
