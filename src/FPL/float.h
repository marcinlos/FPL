/**
 * @file
 */
#ifndef FPL_FLOAT_H
#define FPL_FLOAT_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/// 64-bit floating point number
typedef uint64_t FPL_float64;

/**
 * @name Sizes
 * Sizes of all parts of binary represenation of floating point number
 */
///@{
#define FPL_SIGN_SIZE_64         1
#define FPL_EXP_SIZE_64         11
#define FPL_MANTISSA_SIZE_64    52
///@}

/**
 * @name Offsets
 * Bit offsets of components of floating point number.
 */
///@{
#define FPL_MANTISSA_OFFSET_64  0
#define FPL_EXP_OFFSET_64       ((FPL_MANTISSA_OFFSET_64) + (FPL_MANTISSA_SIZE_64))
#define FPL_SIGN_OFFSET_64      ((FPL_EXP_OFFSET_64) + (FPL_EXP_SIZE_64))
///@}


/**
 * @name Masks
 * Binary masks for all the components of 64-bit floating point number.
 * Bits are numbered from lowest to highest:
 */
///@{

/// Mask of sign (bit 64)
#define FPL_SIGN_MASK_64        (((1uL << FPL_SIGN_SIZE_64) - 1) << FPL_SIGN_OFFSET_64)

/// Mask of exponent (bits 63-53)
#define FPL_EXP_MASK_64         (((1uL << FPL_EXP_SIZE_64) - 1) << FPL_EXP_OFFSET_64)

/// Mask of mantissa (bits 52-1)
#define FPL_MANTISSA_MASK_64    (((1uL << FPL_MANTISSA_SIZE_64) - 1) << FPL_MANTISSA_OFFSET_64)

/// Exponent bias (actual exponent = exponent - 1023)
#define FPL_EXP_BIAS_64         1023

///@}


/**
 * @name Component getters
 * Macros to extract components from @ref FPL_float64 
 */
///@{
#define FPL_GET_SIGN_64(n)      (((n) & FPL_SIGN_MASK_64) >> FPL_SIGN_OFFSET_64)
#define FPL_GET_EXP_BITS_64(n)  (((n) & FPL_EXP_MASK_64) >> FPL_EXP_OFFSET_64)
#define FPL_GET_EXP_64(n)       (FPL_GET_EXP_BITS_64(n) - FPL_EXP_BIAS_64)
#define FPL_GET_MANTISSA_64(n)  (((n) & FPL_MANTISSA_MASK_64) >> FPL_MANTISSA_OFFSET_64)
/// @}

/// Extreme values of an exponent
///@{
#define FPL_MAX_EXP_64          0x7FF
#define FPL_MIN_EXP_64              0
///@}

/**
 * Builds float64 from components
 */
#define FPL_MAKE_FLOAT_64(sign, exp, mant)                                  \
    (                                                                       \
     (((FPL_float64) sign) << FPL_SIGN_OFFSET_64) |                         \
     (((FPL_float64) (exp) + FPL_EXP_BIAS_64) << FPL_EXP_OFFSET_64) |       \
     (((FPL_float64) mant) << FPL_MANTISSA_OFFSET_64)                       \
    )   
    
/**
 * @name Special values
 */
///@{
#define FPL_POSITIVE_ZERO_64    0x0000000000000000uL
#define FPL_NEGATIVE_ZERO_64    0x8000000000000000uL
#define FPL_POSITIVE_INF_64     0x7FF0000000000000uL
#define FPL_NEGATIVE_INF_64     0xFFF0000000000000uL
///@}

/**
 * @name Extreme values 
 * Extreme values of 64-bit floating point numbers in various categories
 */
///@{

/**
 * Maximal positive value of float64
 *  - sign      = 0
 *  - exp       = 7FE
 *  - mantissa  = FFFFFFFFFFFFF
 */
#define FPL_MAX_FLOAT_64        0x7FEFFFFFFFFFFFFFuL

/**
 * Minimal value of positive normalized float64
 *  - sign     = 0
 *  - exp      = 001
 *  - mantissa = 0
 */
#define FPL_MIN_FLOAT_64        0x0010000000000000uL

/**
 * Minimal value of positive subnormal float64
 *  - sign     = 0
 *  - exp      = 0
 *  - mantissa = 1
 */
#define FPL_MIN_SUB_FLOAT_64    0x0000000000000001uL

/**
 * Maximal value of positive subnormal float64
 *  - sign     = 0
 *  - exp      = 0
 *  - mantissa = FFFFFFFFFFFFF
 */
#define FPL_MAX_SUB_FLOAT_64    0x000FFFFFFFFFFFFFuL

///@}

/**
 * Structure containing all the parts of binary represenation of 64-bit
 * floating point number as defined by ieee 754.
 */
typedef struct FPL_ieee754_64
{
    int8_t sign;
    int16_t exponent;
    uint64_t mantissa;
}
FPL_ieee754_64;

/**
 * @name Conversions between FPL_float64 and FPL_ieee754_64 structure
 */
///@{
/**
 * Converts FPL_float64 to FPL_ieee765_64 structure
 *
 * @param[in]  ieee emulated FPL_float64 value
 * @param[out] p pointer to output structure
 */
void FPL_float64_to_ieee(FPL_float64 x, FPL_ieee754_64* ieee);

/**
 * Converts FPL_ieee754_64 structure to FPL_float64
 * 
 * @param[in] ieee structure with components
 * @return float64 having components as specified by @ref ieee
 */
FPL_float64 FPL_ieee_to_float64(FPL_ieee754_64* ieee);
///@}

/**
 * @name Float properties
 * Tests for various special properties of @ref FPL_float64 values
 * (e.g. being NaN).
 */
///@{
/**
 * Checks for NaN value - exponent is FPL_MAX_EXP_64, mantissa != 0.
 *
 * @return true if x is NaN, false otherwise
 */
bool FPL_is_nan_64(FPL_float64 x);

/**
 * Checks for infinite value - exponent is MAX_EXP_64, mantissa == 0.
 * Does not distinguish between negative and positive infinity.
 *
 * @return true if x represents infinite value, false otherwise
 */
bool FPL_is_inf_64(FPL_float64 x);

/**
 * Checks for subnormal values - exponent is MIN_EXP_64, mantissa != 0.
 *
 * @return true if x is subnormal number, false otherwise
 */
bool FPL_is_subnormal_64(FPL_float64 x);

/**
 * @return true if value is normal (not 0, NaN, inf, subnormal),
 *     false otherwise
 */
bool FPL_is_normal_64(FPL_float64 x);
///@}

#ifdef __cplusplus
}
#endif

#endif // FPL_FLOAT_H

