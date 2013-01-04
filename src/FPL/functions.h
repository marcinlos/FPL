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

#define LOGN2       4604418534313441775
#define SQRT2       4609047870845172685

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
double FPL_logarithm_E_64(double a);
double FPL_sin_64(double a);
double FPL_cos_64 (double a);


FPL_float64 FPL_sqrt_64(FPL_float64 x);

#ifdef __cplusplus
}
#endif

#endif // FPL_FUNCTIONS_H
