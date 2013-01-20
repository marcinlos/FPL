#include "config.h"
#include <FPL/float.h>
#include <FPL/functions.h>
#include <FPL/interoperability.h>
#include <FPL/exptable.h>
#include<FPL/macros.h>

static FPL_float64 polytable[] = {
		0x3ff0000000000000,
		0x3fefffffffffffeb,
		0x3fe0000000000019,
		0x3fc555556aaaaa97,
		0x3fa55555471c71cb
};

FPL_float64 static polynomial(FPL_float64 z){

	return FPL_poly_eval_64(z,polytable,5);
    }



FPL_float64 FPL_exponent_64(FPL_float64 expr){
	if (FPL_is_nan_64(expr)){
		return expr;
	}
	if(FPL_is_inf_64(expr)){
		return FPL_GET_SIGN_64(expr)?FPL_POSITIVE_ZERO_64:expr;
	}
	int n = FPL_round(
        FPL_division_64(expr, LOGN2)
    );

	FPL_float64 y = FPL_subtraction_64(
        expr,
        FPL_multiplication_64(
            FPL_to_float64(n),
            LOGN2
        )
    );
	int i = FPL_round(
        FPL_multiplication_64(
            y,
            FPL_to_float64(512)
        )
    );
	FPL_float64 z = FPL_subtraction_64(y, DIV(FPL_to_float64(i),FPL_to_float64(512)));
	int n2 = 1;
	FPL_float64 newn2 = FPL_to_float64(n2);
	FPL_unpacked64 unpacked;
	FPL_UNPACK_64(newn2,unpacked);
	unpacked.e += n;
	newn2 = FPL_PACK_64(unpacked);


	i += 177; //transforming to table indexing

	return FPL_multiplication_64(newn2, FPL_multiplication_64(TABLE[i],polynomial(z)));
}
