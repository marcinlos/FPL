#include "config.h"
#include <FPL/float.h>
#include <FPL/utils.h>

struct uint128
{
    uint32_t b[4];
};


// TODO: Zrob to dobrze jelopie

int FPL_compare(FPL_float64 x, FPL_float64 y)
{
	FPL_float64 res = FPL_subtraction_64(x, y);
	if (res == 0) return 0;
	else return 1 - 2*(res & FPL_SIGN_MASK_64 != 0);

}





#define INCREMENT_FLOAT(up) do {                                            \
        if ((up).m == FPL_MAX_MANTISSA_64)                                  \
        {                                                                   \
            (up).m = FPL_IMPLICIT_ONE_64;                                   \
            ++ (up).e;                                                      \
        }                                                                   \
        else ++ (up).m;                                                     \
    } while (0)

#define HIGH64(val) ((((uint64_t) val.b[3]) << 32) | val.b[2])
#define LOW64(val)  ((((uint64_t) val.b[1]) << 32) | val.b[0])

#define GET_BIT128(val, n)                                                  \
    ((n) > 63 ?                                                             \
        (HIGH64(val) & (1 << ((n) - 64))) != 0                              \
        : ((LOW64(val) & (1 << (n))) != 0))


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
        while (s >= 64)                                                     \
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
        while (s >= 64)                                                     \
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


#define INCREMENT128(num) do {                                              \
        int i = 0;                                                          \
        while (i < 4 && ++ num.b[i++] == 0);                                \
    } while (0)


#define AND_128(x, y) do {                                                  \
        int i = 4;                                                          \
        while (-- i) x.b[i] &= y.b[i];                                      \
    } while (0)


#define ADD_128(x, y) do {                                                  \
        uint64_t acc = 0;                                                   \
        int i = 0;                                                          \
        for (i = 0; i < 4; ++ i)                                            \
        {                                                                   \
            acc = acc + x.b[i] + y.b[i];                                    \
            x.b[i] = LOW32(acc);                                            \
            acc >>= 32;                                                     \
        }                                                                   \
    } while (0)


#define SUB_128(x, y) do {                                                  \
        uint64_t acc = 1;                                                   \
        int i = 0;                                                          \
        for (i = 0; i < 4; ++ i)                                            \
        {                                                                   \
            acc = acc + x.b[i] + ~y.b[i];                                   \
            x.b[i] = LOW32(acc);                                            \
            acc = HIGH32(acc);                                              \
        }                                                                   \
    } while (0)



#define LT_128(x, y) (HIGH64(x) < HIGH64(y) ||                              \
                     ((HIGH64(x) == HIGH64(y)) && LOW64(x) < LOW64(y)))


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




#define NAN_GUARD(x) do { if (FPL_is_nan_64(x)) return (x); } while (0)


#define FPL_ADJUST_EXPONENT_64(up) do {                         \
        if ((up).e < FPL_MIN_NORMAL_EXP_64)                     \
        {                                                       \
            int shift = FPL_MIN_NORMAL_EXP_64 - (up).e;         \
            (up).e = FPL_ZERO_EXP_64;                           \
            if (shift > 63) (up).m = 0;                         \
            else (up).m >>= shift;                              \
        }                                                       \
        else if ((up).e >= FPL_INF_EXP_64)                      \
        {                                                       \
            (up).m = FPL_IMPLICIT_ONE_64;                       \
            (up).e = FPL_INF_EXP_64;                            \
        }                                                       \
    } while (0)



#define FPL_RENORMALIZE_64(result, mantissa, expected_shift) do {           \
        int pos = FPL_highest_nonzero_bit(HIGH64(mantissa)) + 64;           \
        if (pos < 64) pos = FPL_highest_nonzero_bit(LOW64(mantissa));       \
        int total_shift = 0;                                                \
        bool fixed = false;                                                 \
        if (pos >= 0)                                                       \
        {                                                                   \
            total_shift += pos - FPL_MANTISSA_SIZE_64;                      \
            result.e += total_shift - expected_shift;                       \
        }                                                                   \
        else result.e = FPL_ZERO_EXP_64;                                    \
        if (result.e < FPL_MIN_NORMAL_EXP_64)                               \
        {                                                                   \
            total_shift += FPL_MIN_NORMAL_EXP_64 - result.e;                \
            result.e = FPL_ZERO_EXP_64;                                     \
        }                                                                   \
        else if (res.e >= FPL_INF_EXP_64)                                   \
        {                                                                   \
            result.m = FPL_IMPLICIT_ONE_64;                                 \
            result.e = FPL_INF_EXP_64;                                      \
            fixed = true;                                                   \
        }                                                                   \
        if (! fixed)                                                        \
        {                                                                   \
            int b = GET_BIT128(mantissa, total_shift - 1);                  \
            SHIFT_128(mantissa, total_shift);                               \
            result.m = LOW64(mantissa);                                     \
        }                                                                   \
        else result.m = 0;                                                  \
    } while (0)


#define FPL_ALIGN_MANTISSAS(x, y) do {                                      \
        int diff = (x).e - (y).e;                                           \
        if (diff < 0)                                                       \
        {                                                                   \
            SHIFT_LEFT_128(mx, -diff);                                      \
            (x).m >>= (- diff);                                             \
            (x).e = (y).e;                                                  \
        }                                                                   \
        else if ((y).e < (x).e)                                             \
        {                                                                   \
            SHIFT_LEFT_128(my, diff);                                       \
            (y).m >>= diff;                                                 \
            (y).e = (x).e;                                                  \
        }                                                                   \
    } while (0)


FPL_float64 add_same_sign(FPL_unpacked64* x, FPL_unpacked64* y)
{
    FPL_unpacked64 res;
    struct uint128 mx, my;
    FPL_float64 sign = x->s ? FPL_SIGN_MASK_64 : 0;
    if (x->e == FPL_INF_EXP_64)
        return FPL_PACK_64(*x);
    if (y->e == FPL_INF_EXP_64)
        return FPL_PACK_64(*y);
    MAKE_UINT128(x->m, 0uL, mx);
    MAKE_UINT128(y->m, 0uL, my);
    FPL_ALIGN_MANTISSAS(*x, *y);
    res.e = x->e;
    res.s = x->s;
    ADD_128(mx, my);
    FPL_RENORMALIZE_64(res, mx, 64);
    return FPL_PACK_64(res);
}


FPL_float64 sub_same_sign(FPL_unpacked64* x, FPL_unpacked64* y)
{
    FPL_unpacked64 res;
    struct uint128 mx, my;
    if (x->e == FPL_INF_EXP_64)
    {
        if (y->e == FPL_INF_EXP_64)
            return FPL_POSITIVE_NAN_64;
        else
            return FPL_PACK_64(*x);
    }
    else if (y->e == FPL_INF_EXP_64)
    {
        y->s = 1 - y->s;
        return FPL_PACK_64(*y);
    }
    MAKE_UINT128(x->m, 0uL, mx);
    MAKE_UINT128(y->m, 0uL, my);
    FPL_ALIGN_MANTISSAS(*x, *y);
    res.e = x->e;
    if (LT_128(mx, my))
    {
        res.s = 1 - x->s;
        SUB_128(my, mx);
        res.m = y->m - x->m;
        mx = my;
    }
    else
    {
        res.s = x->s;
        res.m = x->m - y->m;
        SUB_128(mx, my);
    }
    FPL_RENORMALIZE_64(res, mx, 64);
    return FPL_PACK_64(res);
}


FPL_float64 FPL_addition_64(FPL_float64 x, FPL_float64 y)
{
    FPL_unpacked64 ux, uy;
    NAN_GUARD(x);
    NAN_GUARD(y);
    FPL_UNPACK_64(x, ux);
    FPL_UNPACK_64(y, uy);
    FPL_float64 res;
    if ((ux.s ^ uy.s) == 0)
        res = add_same_sign(&ux, &uy);
    else
    {
        uy.s = 1 - uy.s;
        res = sub_same_sign(&ux, &uy);
    }
    return res;
}


FPL_float64 FPL_subtraction_64(FPL_float64 x, FPL_float64 y)
{
    FPL_unpacked64 ux, uy;
    NAN_GUARD(x);
    NAN_GUARD(y);
    FPL_UNPACK_64(x, ux);
    FPL_UNPACK_64(y, uy);
    FPL_float64 res;
    if ((ux.s ^ uy.s) == 0)
        res = sub_same_sign(&ux, &uy);
    else
    {
    	uy.s = 1 - uy.s;
        res = add_same_sign(&ux, &uy);
    }
    return res;
}


FPL_float64 FPL_multiplication_64(FPL_float64 x, FPL_float64 y)
{
    FPL_unpacked64 ux, uy, res;
    NAN_GUARD(x);
    NAN_GUARD(y);
    FPL_UNPACK_64(x, ux);
    FPL_UNPACK_64(y, uy);
    res.s = ux.s ^ uy.s;
    res.e = ux.e + uy.e;
    FPL_float64 sign = res.s ? FPL_SIGN_MASK_64 : 0;
    if (ux.e == FPL_INF_EXP_64)
    {
        if (FPL_is_zero_64(y)) return FPL_POSITIVE_NAN_64 | sign;
        else return FPL_POSITIVE_INF_64 | sign;
    }
    if (uy.e == FPL_INF_EXP_64)
    {
        if (FPL_is_zero_64(x)) return FPL_POSITIVE_NAN_64 | sign;
        else return FPL_POSITIVE_INF_64 | sign;
    }
    struct uint128 r;
    FPL_MULT(ux.m, uy.m, r);
    FPL_RENORMALIZE_64(res, r, 52);
    return FPL_PACK_64(res);
}


FPL_float64 FPL_division_64(FPL_float64 x, FPL_float64 y)
{
    FPL_unpacked64 ux, uy, res;
    NAN_GUARD(x);
    NAN_GUARD(y);
    FPL_UNPACK_64(x, ux);
    FPL_UNPACK_64(y, uy);

    //struct uint128 mx, my;
    //MAKE_UINT128(ux.m, 0uL, mx);
    //MAKE_UINT128(uy.m, 0uL, my);

    res.s = ux.s ^ uy.s;
    res.e = ux.e - uy.e;
    uint64_t N = ux.m;
    uint64_t D = uy.m;
    if (uy.m == 0)
    {
        res.e = FPL_INF_EXP_64;
        if (N != 0)
            res.m = 0;
        else
            res.m = FPL_IMPLICIT_ONE_64 >> 1;
        return FPL_PACK_64(res);
    }
    if (ux.e == FPL_INF_EXP_64)
    {
        res.e = FPL_INF_EXP_64;
        if (uy.e != FPL_INF_EXP_64)
            res.m = 0;
        else
            res.m = FPL_IMPLICIT_ONE_64 >> 1;
        return FPL_PACK_64(res);
    }
    if (uy.e == FPL_INF_EXP_64)
    {
        res.e = FPL_ZERO_EXP_64;
        res.m = 0;
        return FPL_PACK_64(res);
    }

    while (N > D)
    {
        res.e += 1;
        D <<= 1;
    }
    uint64_t q = 0;
    int i;
    for (i = 0; i < 53; ++i)
    {
        N <<= 1;
        q <<= 1;
        if (N >= D)
        {
            q |= 1;
            N -= D;
        }
    }
    // One extra bit in the above loop
    -- res.e;
    res.m = q;
    FPL_NORMALIZE_MANTISSA_64(res);
    FPL_ADJUST_EXPONENT_64(res);
    return FPL_PACK_64(res);
}
