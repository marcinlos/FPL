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
    //int d =
}

bool FPL_lt_64(FPL_float64 x, FPL_float64 y)
{

}

bool FPL_lte_64(FPL_float64 x, FPL_float64 y)
{

}

bool FPL_gt_64(FPL_float64 x, FPL_float64 y)
{

}

bool FPL_gte_64(FPL_float64 x, FPL_float64 y)
{

}


