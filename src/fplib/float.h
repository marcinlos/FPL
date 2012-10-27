/**
 * @file
 */
#ifndef FPLIB_FLOAT_H
#define FPLIB_FLOAT_H

#include <stdint.h>

/// 64-bit floating point number
typedef uint64_t float64;

/**
 * @name Masks
 * Binary masks for all the components of 64-bit floating point number.
 * Bits are numbered from lowest to highest:
 */
///@{

/// Mask of sign (bit 64)
#define SIGN_MASK_64        0x8000000000000000uL

/// Mask of exponent (bits 63-53)
#define EXP_MASK_64         0x7FF0000000000000uL

/// Mask of mantissa (bits 52-1)
#define MANTISSA_MASK_64    0x000FFFFFFFFFFFFFuL

/// Exponent bias (actual exponent = exponent - 1023)
#define EXP_BIAS_64         1023

///@}

/**
 * @name Component getters
 * Macros to extract components from @ref float64
 */
///@{

#define GET_SIGN_64(n)      (((n) & SIGN_MASK_64) >> 63)
#define GET_EXP_64(n)       (((n) & EXP_MASK_64) >> 52)
#define GET_MANTISSA_64(n)  ((n) & MANTISSA_MASK_64)

/// @}


#endif // FPLIB_FLOAT_H
