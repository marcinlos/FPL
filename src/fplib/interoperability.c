#include "config.h"
#include <fplib/interoperability.h>


typedef union
{
    double double_val;
    float64 float64_val;
}
float64_double_conv;


float64 double_to_float64(double x)
{
    float64_double_conv u;
    u.double_val = x;
    return u.float64_val;
}


double float64_to_double(float64 x)
{
    float64_double_conv u;
    u.float64_val =  x;
    return u.double_val;
}


void double_to_ieee(double x, ieee754_64* ieee)
{
    float64 n = double_to_float64(x);
    float64_to_ieee(n, ieee);
}


double ieee_to_double(ieee754_64* ieee)
{
    float64 n = ieee_to_float64(ieee);
    return float64_to_double(n);
}
 
