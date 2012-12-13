#include "config.h"
#include <FPL/float.h>

FPL_float64 FPL_poly_eval_64(FPL_float64 x, const FPL_float64* coeffs, int N)
{
    FPL_float64 z = coeffs[N];
    int i;
    for (i = N - 1; i >= 0; -- i)
    {
        z = FPL_multiplication_64(z, x);
        z = FPL_addition_64(z, coeffs[i]);
    }
    return z;
}
