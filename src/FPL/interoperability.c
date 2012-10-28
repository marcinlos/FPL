#include "config.h"
#include <FPL/interoperability.h>


typedef union
{
    double double_val;
    FPL_float64 float64_val;
}
FPL_float64_double_conv;


FPL_float64 FPL_double_to_float64(double x)
{
    FPL_float64_double_conv u;
    u.double_val = x;
    return u.float64_val;
}


double FPL_float64_to_double(FPL_float64 x)
{
    FPL_float64_double_conv u;
    u.float64_val =  x;
    return u.double_val;
}


void FPL_double_to_ieee(double x, FPL_ieee754_64* ieee)
{
    FPL_float64 n = FPL_double_to_float64(x);
    FPL_float64_to_ieee(n, ieee);
}


double FPL_ieee_to_double(FPL_ieee754_64* ieee)
{
    FPL_float64 n = FPL_ieee_to_float64(ieee);
    return FPL_float64_to_double(n);
}
 
