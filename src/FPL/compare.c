/**
 * @file
 */
#include "config.h"
#include <FPL/float.h>

bool FPL_eq_64(FPL_float64 x, FPL_float64 y)
{
    if (! FPL_is_nan_64(x) && ! FPL_is_nan_64(y))
    {
        if (x == y)
            return true;
        else if (FPL_is_zero_64(x) && FPL_is_zero_64(y))
            return true;
        else
            return false;
    }
    else
        return false;
}


int FPL_compare_64(FPL_float64 x, FPL_float64 y)
{
    FPL_unpacked64 ux, uy;
    FPL_UNPACK_64(x, ux);
    FPL_UNPACK_64(y, uy);
    if (ux.s != uy.s)
    {
        if (FPL_is_zero_64(x) && FPL_is_zero_64(y))
            return 0;
        else
            return ux.s == 0 ? 1 : -1;
    }
    else
    {
        return (x < y ? -1 : x == y ? 0 : 1) * (ux.s == 0 ? 1 : -1);
    }
}

bool FPL_lt_64(FPL_float64 x, FPL_float64 y)
{
    return FPL_compare_64(x, y) < 0;
}

bool FPL_lte_64(FPL_float64 x, FPL_float64 y)
{
    return FPL_compare_64(x, y) <= 0;
}

bool FPL_gt_64(FPL_float64 x, FPL_float64 y)
{
    return FPL_compare_64(x, y) > 0;
}

bool FPL_gte_64(FPL_float64 x, FPL_float64 y)
{
    return FPL_compare_64(x, y) >= 0;
}


