#include "config.h"
#include <FPL/float.h>
#include <FPL/utils.h>
#include <FPL/functions.h>

#include <FPL/interoperability.h>
#include <FPL/macros.h>
#include <FPL/utils.h>


FPL_float64 FPL_sqrt_64(FPL_float64 x)
{
    if (FPL_is_sign_minus_64(x)) {
        return FPL_NEGATIVE_NAN_64;
    }
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
    for (i = 0; i < 10; ++ i)
        res = MUL(HALF, ADD(res, DIV(x, res)));

    if (e % 2 != 0)
    {
        if (e < 0)
            res = DIV(res, SQRT2);
        else
            res = MUL(res, SQRT2);
    }
    FPL_UNPACK_64(res, u);
    u.e += e / 2;
    res = FPL_PACK_64(u);
    return res;
}
