#include "config.h"
#include <FPL/float.h>
#include <FPL/utils.h>
#include <stdint.h>
#include <math.h>

//#include <FPL/interoperability.h>
#include <FPL/macros.h>


FPL_float64 FPL_sqrt_64(FPL_float64 x)
{
    //double z = FPL_float64_to_double(x);
    //return FPL_double_to_float64(sqrt(z));

    FPL_float64 res = x;
    int i;
    FPL_float64 half = 0x3fe0000000000000; //  = 0.5

    for (i = 0; i < 10; ++ i)
    {
        res = MUL(half, ADD(res, DIV(x, res)));
    }
    return res;
}
