#include "config.h"
#include <FPL/float.h>


FPL_float64 FPL_negate_64(FPL_float64 x)
{
    return x ^ FPL_SIGN_MASK_64;
}


FPL_float64 FPL_abs_64(FPL_float64 x)
{
    return x & ~FPL_SIGN_MASK_64;
}


FPL_float64 FPL_copy_sign_64(FPL_float64 x, FPL_float64 y)
{
    return (x & ~FPL_SIGN_MASK_64) | (y & FPL_SIGN_MASK_64);
}
