#include "config.h"
#include <FPL/float.h>


void FPL_float64_to_ieee(FPL_float64 x, FPL_ieee754_64* p)
{
    p->sign     = FPL_GET_SIGN_64(x);
    p->exponent = FPL_GET_EXP_64(x);
    p->mantissa = FPL_GET_MANTISSA_64(x);
}


FPL_float64 FPL_ieee_to_float64(FPL_ieee754_64* ieee)
{
    return FPL_MAKE_FLOAT_64(
        ieee->sign, 
        ieee->exponent, 
        ieee->mantissa
    );
}
