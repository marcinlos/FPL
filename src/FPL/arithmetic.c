#include "config.h"
#include <FPL/float.h>
#include <FPL/utils.h>


FPL_float64 FPL_addition_64(FPL_float64 x, FPL_float64 y)
{
    
}


FPL_float64 FPL_subtraction_64(FPL_float64 x, FPL_float64 y)
{
    uint64_t mx = FPL_GET_MANTISSA_64(x), my = FPL_GET_MANTISSA_64(y);
    mx = FPL_add_implicit_one(mx), my = FPL_add_implicit_one(my);
    
}


FPL_float64 FPL_multiplication_64(FPL_float64 x, FPL_float64 y)
{
    uint64_t mx = FPL_GET_MANTISSA_64(x), my = FPL_GET_MANTISSA_64(y);
    mx = FPL_add_implicit_one(mx);
    my = FPL_add_implicit_one(my);
    // -1 for implicit bit
    int shift = FPL_MANTISSA_SIZE_64 - 32 + 1;
    mx >>= shift;
    my >>= shift;
    uint64_t mr = mx * my;
    int exp = FPL_GET_EXP_64(x) + FPL_GET_EXP_64(y);
    uint64_t sign = FPL_GET_SIGN_64(x) ^ FPL_GET_SIGN_64(y);
    // highest bit  set means overflow occured
    if ((mr & 0x8000000000000000) != 0)
    {
    	++ exp;
    	mr >>= 1;
    }
    // highest bit = overflow, second = implicit 1
    mr &= 0x3FFFFFFFFFFFFFFF;
    mr >>= 10;
    return FPL_MAKE_FLOAT_64(sign, exp, mr);
}


FPL_float64 FPL_division_64(FPL_float64 x, FPL_float64 y)
{
}
