#include "config.h"
#include <float.h>


void float64_to_ieee(float64 x, ieee754_64* p)
{
    p->sign = GET_SIGN_64(x);
    p->exponent = GET_EXP_64(x) - EXP_BIAS_64;
    p->mantissa = GET_MANTISSA_64(x);
}


float64 ieee_to_float64(ieee754_64* ieee)
{
    return MAKE_FLOAT64(ieee->sign, ieee->exponent, ieee->mantissa);
}
