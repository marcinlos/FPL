#include "config.h"
#include <FPL/float.h>
#include <FPL/utils.h>
#include <FPL/interoperability.h>
#include <stdint.h>


int64_t FPL_to_integer(FPL_float64 x)
{
    FPL_unpacked64 u;
    FPL_UNPACK_64(x, u);
    int64_t value = u.m;
    int diff = u.e - 52;
    if (diff > 0)
        value <<= diff;
    else
        value >>= (-diff);
    return u.s ? -value : value;
}

int64_t FPL_round(FPL_float64 x)
{
    FPL_unpacked64 u;
    FPL_UNPACK_64(x, u);
    int64_t value = u.m;
    int diff = u.e - 52;
    if (diff > 0)
        value <<= diff;
    else
    {
        value >>= (-diff - 1);
        int lowb = value & 1;
        value >>= 1;
        value += lowb;
    }
    return u.s ? -value : value;
}

FPL_float64 FPL_to_float64(int x)
{
    // TODO: Implement
    return FPL_double_to_float64((double) x);
    return FPL_POSITIVE_NAN_64;
}
