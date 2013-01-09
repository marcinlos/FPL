/**
 * @file
 */
#ifndef FPL_FUNCTIONS_H
#define FPL_FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name Constants
 */
///{

#define LOGN2       0x3fe62e42fefa39ef
#define SQRT2       0x3ff6a09e667f3bcd

#define PI          0x400921fb54442d18
#define PI_2        0x3ff921fb54442d18

#define ONE         0x3ff0000000000000
#define HALF        0x3fe0000000000000

///}

/**
 * Computes the value of polynomial with coefficients coeffs[0]...coeffs[N],
 * that is
 *      coeffs[N] x^N + ... + coeffs[0]
 * using Horner method.
 */
FPL_float64 FPL_poly_eval_64(FPL_float64 x, const FPL_float64* coeffs, int N);


FPL_float64 FPL_exponent_64(FPL_float64 x);

FPL_float64 FPL_logarithm_64(FPL_float64 a, FPL_float64 b);
FPL_float64 FPL_logarithm_E_64(FPL_float64 a);

FPL_float64 FPL_sin_64(FPL_float64 a);
FPL_float64 FPL_cos_64 (FPL_float64 a);

double FPL_tan_64(double x);
double FPL_cotan_64(double x);



FPL_float64 FPL_arctan_64(FPL_float64 x);

FPL_float64 FPL_sqrt_64(FPL_float64 x);

#ifdef __cplusplus
}
#endif

#endif // FPL_FUNCTIONS_H
