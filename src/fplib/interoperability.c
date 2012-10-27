#include "config.h"
#include <fplib/interoperability.h>


float64 double_to_float64(double x)
{
    union
    {
        double val;
        float64 n;
    } u;
    u.val = x;
    return u.n;
}


void double_to_ieee(double x, ieee754_64* p)
{
    uint64_t n = double_to_float64(x);
    p->sign = GET_SIGN_64(n);
    p->exponent = GET_EXP_64(n) - EXP_BIAS_64;
    p->mantissa = GET_MANTISSA_64(n);
}
 
