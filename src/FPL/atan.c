/**
 * @file
 */
#include "config.h"
#include <FPL/float.h>
#include <FPL/functions.h>
#include <FPL/interoperability.h>
#include <FPL/macros.h>
#include <FPL/arctan_FPLTAB.h>


static FPL_float64 arct_aux(FPL_float64 x)
{
    static const FPL_float64 _1_16 = 0x3fafffffffffffff;
    static const FPL_float64 _256 = 0x4070000000000000;
    static const FPL_float64 _1_256 = 0x3f6fffffffffffff;
    if (LT(x, _1_16))
        return FPL_poly_eval_64(x, BASE, sizeof(BASE) / sizeof(BASE[0]));
    else
    {
        int i = ROUND(MUL(x, _256));
        x = SUB(x, MUL(_1_256, FLOAT64(i)));
        int N = sizeof(MAIN[i - 16]) / sizeof(FPL_float64);
        return FPL_poly_eval_64(x, MAIN[i - 16], N);
    }
}



FPL_float64 FPL_arctan_64(FPL_float64 x)
{
    FPL_float64 y = FPL_abs_64(x);
    FPL_float64 res;

    if (GT(y, ONE))
    {
        res = ADD(PI_2, NEG(arct_aux(DIV(ONE, y))));
    }
    else
    {
        res = arct_aux(y);
    }
    return FPL_is_sign_minus_64(x) ? NEG(res) : res;
}


