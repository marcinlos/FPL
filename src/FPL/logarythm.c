#include "config.h"
#include <FPL/float.h>
#include <FPL/interoperability.h>
#include <FPL/logtable.h>
#include <math.h>

#define LOGN2 4604418534313441775;

double static polynomial(double x){
	return 1.4045029064076594981467548241576029709745070016382549949011e-96 + 0.9999999999999999999999917374202996065681905866839136611725*x
			- 0.4999999999999999999999925636765477056221792963438333048451*x*x + 0.3333333333333333495782192703860337840598850693074962264487*x*x*x
			- 0.2500000000000000146204006157773413568585527113102554985955*x*x*x*x + 0.1999999999913765484599888779513938442741120088779883813959*x*x*x*x*x
			- 0.1666666666589055587058662155957302248503575444427867117456*x*x*x*x*x*x + 0.1428588382922608524316678538001521754672181631046527236984*x*x*x*x*x*x*x
			- 0.125001525891841565968060649430009547354266601487839728337*x*x*x*x*x*x*x*x;
}

double logarithm (double argument){
	int n = 0;
	int n2=1;
	double y;
	while (!(argument/n2 > 0.75 && argument/n2 < 1.5)){
		n2 = n2*2;
		n++;
	}
    y = argument/n2;
    printf("\ny= %e\n", y);
    printf("\nn= %d\n",n);
	int i = round((y-3/4.0-1/512.0)*256);
	printf("\ni= %d\n",i);
	printf("\ntabx= %e\n",TABLEX[i]);
	printf("\ntabg= %e\n",TABLEG[i]);
	double z = (y - TABLEX[i]) * TABLEG[i]; // <|1/384|
	printf("\nz= %e\n",z);
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
