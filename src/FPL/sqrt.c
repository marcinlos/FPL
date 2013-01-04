#include "config.h"
#include <FPL/float.h>
#include <FPL/utils.h>
#include <FPL/functions.h>
#include <stdint.h>
#include <math.h>

#include <FPL/interoperability.h>
#include <FPL/macros.h>
#include <FPL/utils.h>


FPL_float64 FPL_sqrt_64(FPL_float64 x)
{
    if (FPL_is_sign_minus_64(x)) {
        return FPL_NEGATIVE_NAN_64;
    }
    printf("x = %e\n", FPL_float64_to_double(x));
    FPL_unpacked64 u;
    int i;
    FPL_UNPACK_64(x, u);
    FPL_NORMALIZE_MANTISSA_64(u);
    int e = u.e;
    // Compute exponent-reduced argument
    u.e = 0;
    x = FPL_PACK_64(u);

    // Compute its square root
    FPL_float64 res = x;
    printf("res = %f\n", FPL_float64_to_double(res));
    for (i = 0; i < 10; ++ i)
        res = MUL(HALF, ADD(res, DIV(x, res)));

    printf("res = %f\n", FPL_float64_to_double(res));

    // Re
    if (e % 2 != 0)
    {
        if (e < 0)
            res = DIV(res, SQRT2);
        else
            res = MUL(res, SQRT2);
    }
    FPL_UNPACK_64(res, u);
    printf("e = %d, e/2 = %d, u.e = %d\n", e, e/2, u.e);
    u.e += e / 2;
    res = FPL_PACK_64(u);
    printf("Result: %e\n", FPL_float64_to_double(res));
    return res;
}
