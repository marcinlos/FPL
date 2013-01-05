/**
 * @file
 */
#include "config.h"
#include <FPL/float.h>
#include <FPL/functions.h>
#include <FPL/interoperability.h>
#include <FPL/arctan_table.h>
#include <math.h>

double poly(double x, const double* coeffs, int N)
{
    double z = coeffs[N];
    int i;
    for (i = N - 1; i >= 0; -- i)
        z = z * x + coeffs[i];
    return z;
}

static double arct_aux(double x)
{
    if (x < 1./16)
        return poly(x, BASE, sizeof(BASE) / sizeof(double));
    else
    {
        int i = (int)(x * 256 + 0.5);
        x -= 1./256 * i;
        printf("i = %d, x = %f\n", i, x);
        return poly(x, MAIN[i - 16], sizeof(MAIN[i - 16]) / sizeof(double));
    }
}


static double arct(double x)
{
    double y = fabs(x);
    double res;
    if (y > 1)
    {
        res = M_PI / 2 - arct_aux(1/y);
    }
    else
    {
        res = arct_aux(y);
    }
    return x > 0 ? res : - res;
}


FPL_float64 FPL_arctan_64(FPL_float64 x)
{
    double arg = FPL_float64_to_double(x);
    double res = arct(arg);
    return FPL_double_to_float64(res);
}


