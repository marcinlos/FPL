#ifndef FPL_MACROS_H
#define FPL_MACROS_H

#include <FPL/float.h>

/*
 * For internal use of implementors
 */

#define ADD(x, y) (FPL_addition_64((x), (y)))

#define SUB(x, y) (FPL_subtraction_64((x), (y)))

#define MUL(x, y) (FPL_multiplication_64((x), (y)))

#define DIV(x, y) (FPL_division_64((x), (y)))

#define TRUNC(x) (FPL_to_integer_64((x)))

#define ROUND(x) (FPL_round_64((x)))


#endif // FPL_MACROS_H
