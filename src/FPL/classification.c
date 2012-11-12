#include "config.h"
#include <FPL/float.h>


bool FPL_is_nan_64(FPL_float64 x)
{
    return 
        FPL_GET_EXP_BITS_64(x) == FPL_MAX_EXP_BITS_64 &&
        FPL_GET_MANTISSA_BITS_64(x) != 0;
}


bool FPL_is_inf_64(FPL_float64 x)
{
    return 
        FPL_GET_EXP_BITS_64(x) == FPL_MAX_EXP_BITS_64 &&
        FPL_GET_MANTISSA_BITS_64(x) == 0;
}


bool FPL_is_subnormal_64(FPL_float64 x)
{
    return 
        FPL_GET_EXP_BITS_64(x) == 0 && 
        FPL_GET_MANTISSA_BITS_64(x) != 0;
}


bool FPL_is_normal_64(FPL_float64 x)
{
    int16_t exp = FPL_GET_EXP_BITS_64(x);
    return exp < 0x7FF && exp > 0;
}


bool FPL_is_zero_64(FPL_float64 x)
{
    return (x & ~FPL_SIGN_MASK_64) == 0;
}


bool FPL_is_sign_minus_64(FPL_float64 x)
{
    return (x & FPL_SIGN_MASK_64) != 0;
}


