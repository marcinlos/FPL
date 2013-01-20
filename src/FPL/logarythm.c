#include "config.h"
#include <FPL/float.h>
#include <FPL/interoperability.h>
#include <FPL/logtable1.h>
#include <FPL/logtable2.h>
#include <FPL/logtable3.h>
#include <FPL/functions.h>
#include <FPL/macros.h>
static FPL_float64 polytable[] = {
		0,
		0x3ff0000000000000,
		0xbfe0000000000000,
		0x3fd5555555555556,
		0xbfd0000000000000,
		0x3fc999999994dbf5,
		0xbfc555555551110e,
		0x3fc24932cb3474bf,
		0xbfc0000cccd3e949
};

FPL_float64 static polynomial(FPL_float64 x){
	return FPL_poly_eval_64(x,polytable,9);
}

FPL_float64 FPL_logarithm_E_64 (FPL_float64 argument){
	if (FPL_is_sign_minus_64(argument) || !FPL_is_normal_64(argument)){
		return FPL_POSITIVE_NAN_64;
	}
	else if (FPL_is_zero_64(argument)){
		return FPL_NEGATIVE_INF_64;
	}
	else{
		int n = 0;
		int n2=1;
		FPL_float64 y;


		if(argument < 0x3fe8000000000000)
		{
			FPL_float64 n3 = 0x3ff0000000000000; //=1
			FPL_unpacked64 unpacked;
			while (!(DIV(argument,n3) >= 0x3fe8000000000000 && DIV(argument,n3) < 0x3ff8000000000000))
			{
				FPL_UNPACK_64(n3,unpacked);
				unpacked.e -= 1;
				n3 = FPL_PACK_64(unpacked);
				n--;
			}
			y = DIV(argument,n3);
		}
		else{
			while (!(DIV(argument,FLOAT64(n2)) >= 0x3fe8000000000000 && DIV(argument,FLOAT64(n2)) < 0x3ff8000000000000)){
				n2 = n2*2;
				n++;
			}
			y = DIV(argument,FLOAT64(n2));
		}
		//jest pomiedzy 0.75 a 1.5

		//int i = round((y-3/4.0-1/512.0)*256);
		int i = ROUND(MUL(SUB(SUB(y,0x3fe8000000000000),0x3f5fffffffffffff),FLOAT64(256)));
		if(i<0) i=0;
		else if (i>191) i=191;
		//FPL_float64 z = (y - TABLEX[i]) * TABLEG[i]; // <|1/384|
		FPL_float64 z = MUL(SUB(y,TABLEX[i]),TABLEG[i]);
		FPL_float64 wynik = ADD(TABLEF[i], polynomial(z));
		return ADD(wynik, MUL(FLOAT64(n), LOGN2));
	}
}

FPL_float64 FPL_logarithm_64 (FPL_float64 base,FPL_float64 argument){
	//if arg <=0 or NaN or inf it's bad
	if (FPL_is_sign_minus_64(argument) || !FPL_is_normal_64(argument)){
		return FPL_POSITIVE_NAN_64;
	}
	else if (FPL_is_zero_64(argument)){
			return FPL_NEGATIVE_INF_64;
		}
	else{
		return DIV(FPL_logarithm_E_64(argument),FPL_logarithm_E_64(base));
	}
}
