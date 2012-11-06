/**
 * @file
 */
#ifndef FPL_UTILS_H
#define FPL_UTILS_H

#include <FPL/float.h>


#ifdef __cplusplus
extern "C" {
#endif


uint64_t FPL_add_implicit_one(uint64_t mantissa);

int FPL_highest_nonzero_bit(uint64_t x);


#ifdef __cplusplus
}
#endif

#endif // FPL_UTILS_H
