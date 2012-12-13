/**
 * @file
 */
#ifndef FPL_INTEROPERABILITY_H
#define FPL_INTEROPERABILITY_H

#include <FPL/float.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Converts double to integral type FPL_float64. It's a function instead of 
 * macro casting pointers because this would preclude using it with rvalues.
 *
 * @param[in] x Value to convert
 * @return FPL_float64 with the same binary representation
 *
 * @sa FPL_float64_to_double
 */
FPL_float64 FPL_double_to_float64(double x);

/**
 * Converts FPL_float64 to native double.
 *
 * @param[in] x Value to convert
 * @return double with the same binary represenation
 *
 * @sa FPL_double_to_float64
 */
double FPL_float64_to_double(FPL_float64 x);

/**
 * Converts native double to FPL_ieee754_64 structure
 *
 * @param[in]  x native floating point value 
 * @param[out] ieee pointer to output structure
 */
void FPL_double_to_ieee(double x, FPL_ieee754_64* ieee);

/**
 * Converts FPL_ieee754_64 structure to native double
 * 
 * @param[in] ieee structure with components
 * @return double precision floating point number with components as
 *     defined by @ref ieee
 */
double FPL_ieee_to_double(FPL_ieee754_64* ieee);

#ifdef __cplusplus
}
#endif


#endif // FPL_INTEROPERABILITY_H

