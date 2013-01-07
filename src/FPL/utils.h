/**
 * @file
 */
#ifndef FPL_UTILS_H
#define FPL_UTILS_H

#include <FPL/float.h>


#ifdef __cplusplus
extern "C" {
#endif


int FPL_highest_nonzero_bit(uint64_t x);


#define FPL_NORMALIZE_MANTISSA_64(up) do {                      \
        int pos = FPL_highest_nonzero_bit((up).m);              \
        if (pos >= 0)                                           \
        {                                                       \
            int shift = pos - FPL_MANTISSA_SIZE_64;             \
            if (shift > 0) (up).m >>= shift;                    \
            else (up).m <<= (-shift);                           \
            (up).e += shift;                                    \
        }                                                       \
        else (up).e = FPL_ZERO_EXP_64;                          \
    } while (0)






#ifdef __cplusplus
}
#endif

#endif // FPL_UTILS_H
