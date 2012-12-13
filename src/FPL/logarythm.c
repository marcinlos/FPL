#include "config.h"
#include <FPL/float.h>
#include <FPL/interoperability.h>
#include <FPL/logtable.h>
#include <math.h>

#define LOGN2 4604418534313441775;

double static polynomial(double z){
	return 0;
}

double logarithm (double argument){
	int n = 0;
	int n2=1;
	double y;
	while (argument/n2 > 0.75 && argument/n2 < 1.5){
		n2 = n2*2;
		n++;
	}
    y = argument/n2;
	int i = round((y-3/4.0-1/512.0)*256);
	double z = (y - TABLEX[i]) * TABLEG[i]; // <|1/384|
	double wynik = TABLEF[i] + polynomial(z);
	return wynik + n * log(2);
}

double FPL_logarithm_64 (double base,double argument){
	//if arg <=0 or NaN or inf it's bad
	/*if (FPL_is_sign_minus_64(argument) || !FPL_is_normal_64(argument)){
		return FPL_POSITIVE_NAN_64;
	}
	else{*/
	return logarithm(argument) / logarithm(base);
	//}
}
