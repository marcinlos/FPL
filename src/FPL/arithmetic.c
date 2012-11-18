#include "config.h"
#include <FPL/float.h>
#include <FPL/utils.h>


#define FPL_ALIGN_MANTISSAS_64(x, y) do {                       \
        int diff = (x).e - (y).e;                               \
        if (diff < 0)                                           \
        {                                                       \
            (x).m >>= (-diff);                                  \
            (x).e = (y).e;                                      \
        }                                                       \
        else if ((y).e < (x).e)                                 \
        {                                                       \
            (y).m >>= diff;                                     \
            (y).e = (x).e;                                      \
        }                                                       \
    } while (0)


#define FPL_ADJUST_EXPONENT_64(up) do {                         \
        if ((up).e < FPL_MIN_NORMAL_EXP_64)                     \
        {                                                       \
            int shift = FPL_MIN_NORMAL_EXP_64 - (up).e;         \
            (up).e = FPL_ZERO_EXP_64;                           \
            (up).m >>= shift;                                   \
        }                                                       \
        else if ((up).e >= FPL_INF_EXP_64)                      \
        {                                                       \
            (up).m = FPL_IMPLICIT_ONE_64;                       \
            (up).e = FPL_INF_EXP_64;                            \
        }                                                       \
    } while (0)


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


FPL_unpacked64 add_same_sign(FPL_unpacked64* x, FPL_unpacked64* y)
{
    FPL_ALIGN_MANTISSAS_64(*x, *y);
    int16_t e = x->e;
    uint64_t m = x->m + y->m;
    FPL_unpacked64 res = {x->s, e, m};
    FPL_NORMALIZE_MANTISSA_64(res);
    FPL_ADJUST_EXPONENT_64(res);
    return res;
}


FPL_unpacked64 sub_same_sign(FPL_unpacked64* x, FPL_unpacked64* y)
{
    FPL_unpacked64 res;
    FPL_ALIGN_MANTISSAS_64(*x, *y);
    res.e = x->e;
    if (x->m < y->m)
    {
        res.s = 1;
        res.m = y->m - x->m;
    }
    else
    {
        res.s = 0;
        res.m = x->m - y->m;
    }
    FPL_NORMALIZE_MANTISSA_64(res);
    FPL_ADJUST_EXPONENT_64(res);
    return res;
}


FPL_float64 FPL_addition_64(FPL_float64 x, FPL_float64 y)
{
    FPL_unpacked64 ux, uy;
    FPL_UNPACK_64(x, ux);
    FPL_UNPACK_64(y, uy);
    FPL_unpacked64 res;
    if ((ux.s ^ uy.s) == 0)
        res = add_same_sign(&ux, &uy);
    else
    {
        ux.s = 1 - ux.s;
        res = sub_same_sign(&ux, &uy);
    }
    return FPL_PACK_64(res);
}


FPL_float64 FPL_subtraction_64(FPL_float64 x, FPL_float64 y)
{
    FPL_unpacked64 ux, uy;
    FPL_UNPACK_64(x, ux);
    FPL_UNPACK_64(y, uy);
    FPL_unpacked64 res;
    if ((ux.s ^ uy.s) == 0)
        res = sub_same_sign(&ux, &uy);
    else
    {
        ux.s = 1 - ux.s;
        res = add_same_sign(&ux, &uy);
    }
    return FPL_PACK_64(res);
}


struct uint128
{
    uint32_t b[4];
};

#define HIGH64(val) ((((uint64_t) val.b[3]) << 32) | val.b[2])
#define LOW64(val)  ((((uint64_t) val.b[1]) << 32) | val.b[0])


#define HIGH32(val) (val >> 32)
#define LOW32(val)  (val & 0xffffffff)


#define MAKE_UINT128(high, low, num) do {                                   \
        num.b[0] = LOW32(low);                                              \
        num.b[1] = HIGH32(low);                                             \
        num.b[2] = LOW32(high);                                             \
        num.b[3] = HIGH32(high);                                            \
    } while (0)


#define SHIFT_LEFT_128(num, sh) do {                                        \
        uint64_t high = HIGH64(num), low = LOW64(num);                      \
        int s = sh;                                                         \
        if (sh >= 64)                                                       \
        {                                                                   \
            low = high;                                                     \
            high = 0;                                                       \
            s -= 64;                                                        \
        }                                                                   \
        low = (high << (64 - s)) | (low >> s);                              \
        high >>= s;                                                         \
        MAKE_UINT128(high, low, num);                                       \
    } while (0)


#define SHIFT_RIGHT_128(num, sh) do {                                       \
        uint64_t high = HIGH64(num), low = LOW64(num);                      \
        int s = sh;                                                         \
        if (sh >= 64)                                                       \
        {                                                                   \
            high = low;                                                     \
            low = 0;                                                        \
            s -= 64;                                                        \
        }                                                                   \
        high = (high << s) | (low >> (64 - s));                             \
        low <<= s;                                                          \
        MAKE_UINT128(high, low, num);                                       \
    } while (0)


#define SHIFT_128(num, sh) do {                                             \
        if ((sh) > 0) SHIFT_LEFT_128(num, sh);                              \
        else SHIFT_RIGHT_128(num, (-(sh)));                                 \
    } while (0)



#define FPL_MULT(x, y, r) do {                                              \
        uint64_t xl = LOW32(x);                                             \
        uint64_t xh = HIGH32(x);                                            \
        uint64_t yl = LOW32(y);                                             \
        uint64_t yh = HIGH32(y);                                            \
                                                                            \
        uint32_t carry = 0;                                                 \
        uint64_t tmp;                                                       \
                                                                            \
        tmp = yl * xl;                                                      \
        carry = HIGH32(tmp);                                                \
        uint32_t x1 = LOW32(tmp);                                           \
        tmp = yl * xh + carry;                                              \
        uint32_t x2 = LOW32(tmp);                                           \
        uint32_t x3 = HIGH32(tmp);                                          \
                                                                            \
        tmp = yh * xl;                                                      \
        carry = HIGH32(tmp);                                                \
        uint32_t y1 = LOW32(tmp);                                           \
        tmp = yh * xh + carry;                                              \
        uint32_t y2 = LOW32(tmp);                                           \
        uint32_t y3 = HIGH32(tmp);                                          \
                                                                            \
        r.b[0] = x1;                                                        \
        tmp = (uint64_t) x2 + y1;                                           \
        carry = HIGH32(tmp);                                                \
        r.b[1] = LOW32(tmp);                                                \
        tmp = (uint64_t) x3 + y2 + carry;                                   \
        carry = HIGH32(tmp);                                                \
        r.b[2] = LOW32(tmp);                                                \
        r.b[3] = y3 + carry;                                                \
    } while (0)




FPL_float64 FPL_multiplication_64(FPL_float64 x, FPL_float64 y)
{
    FPL_unpacked64 ux, uy, res;
    FPL_UNPACK_64(x, ux);
    FPL_UNPACK_64(y, uy);
    res.s = ux.s ^ uy.s;
    res.e = ux.e + uy.e;

    struct uint128 r;
    FPL_MULT(ux.m, uy.m, r);

    // Manual renormalization due to 128 bit precision of mantissa
    int pos = FPL_highest_nonzero_bit(HIGH64(r)) + 64;
    if (pos < 64) pos = FPL_highest_nonzero_bit(LOW64(r));

    int total_shift = 0;
    if (pos >= 0)
    {
        total_shift += pos - FPL_MANTISSA_SIZE_64;
        res.e += total_shift - 52;
    }
    else res.e = FPL_ZERO_EXP_64;
    if (res.e < FPL_MIN_NORMAL_EXP_64)
    {
        total_shift += FPL_MIN_NORMAL_EXP_64 - res.e;
        res.e = FPL_ZERO_EXP_64;
    }
    else if (res.e >= FPL_INF_EXP_64)
    {
        res.m = FPL_IMPLICIT_ONE_64;
        res.e = FPL_INF_EXP_64;
    }
    if (res.e != FPL_INF_EXP_64) {
        SHIFT_128(r, total_shift);
        res.m = LOW64(r);
    }
    return FPL_PACK_64(res);
}


FPL_float64 FPL_division_64(FPL_float64 x, FPL_float64 y)
{
}
