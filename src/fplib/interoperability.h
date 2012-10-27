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
 * @param x Value to convert
 * @return float64 with the same binary representation
 */
float64 double_to_float64(double x);

/**
 * Structure containing all the parts of binary represenation of 64-bit
 * floating point number as defined by ieee 754
 */
typedef struct ieee754_64
{
    int8_t sign;
    int16_t exponent;
    int32_t mantissa;
}
ieee754_64;

/**
 * Converts float64 to ieee765_64 structure
 *
 * @param x emulated float64 value
 * @param p pointer to output structure
 */
void float64_to_ieee(float64 x, ieee754_64* p);

/**
 * Converts native double to ieee754_64 structure
 *
 * @param x native floating point value 
 * @param p pointer to output structure
 */
void double_to_ieee(double x, ieee754_64* p);

#ifdef __cplusplus
}
#endif


#endif // FPLIB_INTEROPERABILITY_H
