/**
 * @file
 */
#ifndef FPLIB_INTEROPERABILITY_H
#define FPLIB_INTEROPERABILITY_H

#include <fplib/float.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Converts double to integral type float64. It's a function instead of macro
 * casting pointers because this would preclude using it with rvalues.
 *
 * @param[in] x Value to convert
 * @return float64 with the same binary representation
 *
 * @sa float64_to_double
 */
float64 double_to_float64(double x);

/**
 * Converts float64 to native double.
 *
 * @param[in] x Value to convert
 * @return double with the same binary represenation
 *
 * @sa double_to_float64
 */
double float64_to_double(float64 x);

/**
 * Converts native double to ieee754_64 structure
 *
 * @param[in]  x native floating point value 
 * @param[out] ieee pointer to output structure
 */
void double_to_ieee(double x, ieee754_64* ieee);

/**
 * Converts ieee754_64 structure to native double
 * 
 * @param[in] ieee structure with components
 * @return double precision floating point number with components as
 *     defined by @ref ieee
 */
double ieee_to_double(ieee754_64* ieee);

#ifdef __cplusplus
}
#endif


#endif // FPLIB_INTEROPERABILITY_H

