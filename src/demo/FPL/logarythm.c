#include "config.h"
#include <FPL/float.h>
#include <FPL/interoperability.h>

#define LOG2 4604418534313441775;
#define SQRT2 4609047870845172685;

FPL_float64 FPL_logarythm_64 (FPL_float64 base, FPL_float64 argument){
	//if arg <=0 or NaN or inf it's bad
	if (FPL_is_sign_minus_64(argument) || !FPL_is_normal_64(argument)){
		return FPL_POSITIVE_NAN_64;
	}
	else{
		FPL_unpacked64 number;
		FPL_UNPACK_64(argument,number);
		//number.m = number.m & (~FPL_IMPLICIT_ONE_64);

	}
}
