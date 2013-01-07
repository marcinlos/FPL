#include "config.h"
#include <FPL/float.h>
#include <FPL/macros.h>


FPL_float64 FPL_poly_eval_64(FPL_float64 x, const FPL_float64* coeffs, int N)
{
    FPL_float64 z = coeffs[N];
    int i;
    for (i = N - 1; i >= 0; -- i)
    {
        z = ADD(MUL(z, x), coeffs[i]);
    }
    return z;
}
