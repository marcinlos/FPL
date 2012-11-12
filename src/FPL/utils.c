#include "config.h"
#include <FPL/float.h>


int FPL_highest_nonzero_bit(uint64_t x)
{
    int i;
    for (i = 63; x != 0; --i)
        {
            if (x & 0x8000000000000000)
                return i;
            x <<= 1;
        }
    return -1;
}
