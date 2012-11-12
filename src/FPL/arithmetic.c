#include "config.h"
#include <FPL/float.h>
#include <FPL/utils.h>

static int16_t align_mantissas(FPL_unpacked64* x, FPL_unpacked64* y)
{
	int diff = x->e - y->e;
	if (diff < 0)
	{
		x->m >>= (- diff);
		return (x->e = y->e);
	}
	else if (y->e < x->e)
	{
		y->m >>= diff;
		return (y->e = x->e);
	}
	else return x->e;
}

void normalize_mantissa(FPL_unpacked64* x)
{
	int pos = FPL_highest_nonzero_bit(x->m);
	if (pos >= 0)
	{
		int diff = pos - FPL_MANTISSA_SIZE_64;
		if (diff > 0)
			x->m >>= diff;
		else
			x->m <<= (-diff);
		x->e += diff;
	}
	else
	{
		x->e = FPL_ZERO_EXP_64;
	}
}

FPL_unpacked64 add_same_sign(FPL_unpacked64* x, FPL_unpacked64* y)
{
	int16_t e = align_mantissas(x, y);
	uint64_t m = x->m + y->m;
	FPL_unpacked64 res = {x->s, e, m};
	normalize_mantissa(&res);
	return res;
}


FPL_unpacked64 sub_same_sign(FPL_unpacked64* x, FPL_unpacked64* y)
{
    FPL_unpacked64 res;
    res.e = align_mantissas(x, y);
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
    normalize_mantissa(&res);
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

}




#define FPL_NORMALIZE_64(up) do {                           \
        if (up.e < FPL_MIN_NORMAL_EXP_64)                   \
        {                                                   \
            int diff = -FPL_EXP_BIAS_64 - up.e + 1;         \
            up.e = FPL_MIN_NORMAL_EXP_64 - 1;               \
            up.m >>= diff;                                  \
        }                                                   \
        else                                                \
        {                                                   \
            if ((up.m & (FPL_IMPLICIT_ONE_64 << 1)) != 0)   \
            {                                               \
                ++ up.e;                                    \
                up.m >>= 1;                                 \
            }                                               \
            if (up.e > FPL_MAX_NORMAL_EXP_64)               \
            {                                               \
                up.e = FPL_EXP_BIAS_64 + 1;                 \
                up.m = 0;                                   \
            }                                               \
        }                                                   \
    } while(0)

FPL_float64 FPL_multiplication_64(FPL_float64 x, FPL_float64 y)
{
    FPL_unpacked64 ux, uy, res;
    FPL_UNPACK_64(x, ux);
    FPL_UNPACK_64(y, uy);
    res.s = ux.s ^ uy.s;
    int shift = FPL_MANTISSA_SIZE_64 / 2;
    res.m = (ux.m >> shift) * (uy.m >> shift);
    res.e = ux.e + uy.e;
    FPL_NORMALIZE_64(res);
    return FPL_PACK_64(res);
}


FPL_float64 FPL_division_64(FPL_float64 x, FPL_float64 y)
{
}
