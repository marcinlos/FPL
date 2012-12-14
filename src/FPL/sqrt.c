#include "config.h"
#include <FPL/float.h>
#include <FPL/utils.h>
#include <stdint.h>
#include <math.h>

FPL_float64 FPL_sqrt_64(FPL_float64 x)
{
    double z = FPL_float64_to_double(x);
    return FPL_double_to_float64(sqrt(z));
}
