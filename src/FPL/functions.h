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
double FPL_logarithm_64(double a, double b);
double logarithm(double a);

double FPL_sin_64 (double x);


FPL_float64 FPL_arctan_64(FPL_float64 x);

FPL_float64 FPL_sqrt_64(FPL_float64 x);

#ifdef __cplusplus
}
#endif

#endif // FPL_FUNCTIONS_H
