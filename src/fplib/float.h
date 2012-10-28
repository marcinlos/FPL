/**
 * @file
 */
#ifndef FPLIB_FLOAT_H
#define FPLIB_FLOAT_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/// 64-bit floating point number
typedef uint64_t float64;

/**
 * @name Sizes
 * Sizes of all parts of binary represenation of floating point number
 */
///@{
#define SIGN_SIZE_64         1
#define EXP_SIZE_64         11
#define MANTISSA_SIZE_64    52
///@}

/**
 * @name Offsets
 * Bit offsets of components of floating point number.
 */
///@{
#define MANTISSA_OFFSET_64       0
#define EXP_OFFSET_64           ((MANTISSA_OFFSET_64) + (MANTISSA_SIZE_64))
#define SIGN_OFFSET_64          ((EXP_OFFSET_64) + (EXP_SIZE_64))
///@}



/**
 * @name Masks
 * Binary masks for all the components of 64-bit floating point number.
 * Bits are numbered from lowest to highest:
 */
///@{

/// Mask of sign (bit 64)
#define SIGN_MASK_64        (((1uL << SIGN_SIZE_64) - 1) << SIGN_OFFSET_64)

/// Mask of exponent (bits 63-53)
#define EXP_MASK_64         (((1uL << EXP_SIZE_64) - 1) << EXP_OFFSET_64)

/// Mask of mantissa (bits 52-1)
#define MANTISSA_MASK_64    (((1uL << MANTISSA_SIZE_64) - 1) << MANTISSA_OFFSET_64)

/// Exponent bias (actual exponent = exponent - 1023)
#define EXP_BIAS_64         1023

///@}


/**
 * @name Component getters
 * Macros to extract components from @ref float64 
 */
///@{
#define GET_SIGN_64(n)      (((n) & SIGN_MASK_64) >> SIGN_OFFSET_64)
#define GET_EXP_64(n)       ((((n) & EXP_MASK_64) >> EXP_OFFSET_64) - EXP_BIAS_64)
#define GET_MANTISSA_64(n)  (((n) & MANTISSA_MASK_64) >> MANTISSA_OFFSET_64)
/// @}

/**
 * Builds float64 from components
 */
#define MAKE_FLOAT_64(sign, exp, mant)                          \
    (                                                           \
     (((float64) sign) << SIGN_OFFSET_64) |                     \
     (((float64)(exp) + EXP_BIAS_64) << EXP_OFFSET_64) |        \
     (((float64) mant) << MANTISSA_OFFSET_64)                   \
    )   

/**
 * Structure containing all the parts of binary represenation of 64-bit
 * floating point number as defined by ieee 754
 */
typedef struct ieee754_64
{
    int8_t sign;
    int16_t exponent;
    uint64_t mantissa;
}
ieee754_64;

/**
 * @name Conversions between float64 and ieee754_64 structure
 */
///@{
/**
 * Converts float64 to ieee765_64 structure
 *
 * @param[in]  ieee emulated float64 value
 * @param[out] p pointer to output structure
 */
void float64_to_ieee(float64 x, ieee754_64* ieee);

/**
 * Converts ieee754_64 structure to float64
 * 
 * @param[in] ieee structure with components
 * @return float64 having components as specified by @ref ieee
 */
float64 ieee_to_float64(ieee754_64* ieee);
///@}

/**
 * @name Float properties
 * Tests for various special properties of @ref float64 values
 * (e.g. being NaN).
 */
///@{
/**
 * @return true if x is NaN, false otherwise
 */
bool is_nan_64(float64 x);

/**
 * @return true if x represents infinite value, false otherwise
 */
bool is_inf_64(float64 x);

/**
 * @return true if x is subnormal number (i.e. is to be interpreted
 *     without implicit 1 before the dot in mantissa), false otherwise
 */
bool is_subnormal_64(float64 x);
///@}

#ifdef __cplusplus
}
#endif

#endif // FPLIB_FLOAT_H

