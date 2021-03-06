#ifndef FPL_MACROS_H
#define FPL_MACROS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <FPL/float.h>

/*
 * For internal use of implementors
 */

#define FLOAT64(x) (FPL_double_to_float64((double) (x)))

#define ADD(x, y) (FPL_addition_64((x), (y)))

#define SUB(x, y) (FPL_subtraction_64((x), (y)))

#define MUL(x, y) (FPL_multiplication_64((x), (y)))

#define DIV(x, y) (FPL_division_64((x), (y)))

#define TRUNC(x) (FPL_to_integer_64((x)))

#define ROUND(x) (FPL_round((x)))

#define NEG(x) (FPL_negate_64((x)))

#define ABS(x) (FPL_abs_64((x)))

#define LT(x, y)    (FPL_lt_64((x), (y)))
#define LTE(x, y)   (FPL_lte_64((x), (y)))
#define GT(x, y)    (FPL_gt_64((x), (y)))
#define GTE(x, y)   (FPL_gte_64((x), (y)))

#ifdef __cplusplus
}
#endif

#endif // FPL_MACROS_H
