#include "config.h"
#include <FPL/float.h>
#include <FPL/interoperability.h>
#include <stdio.h>
#include <math.h>
#include <FPL/sintable.h>
#include <FPL/costable.h>

#define pi 3.141592653589793238462

double poly9(double x){ // for sin
	return 0.999999999999999999393795829034*x -
	0.166666666666676221746125759480*x*x*x +
	0.00833333332487845206504547225678*x*x*x*x*x-
	0.000198413628378777807248883955724*x*x*x*x*x*x*x+
	0.00000274625991149636194057744682828*x*x*x*x*x*x*x*x*x;
}
double poly8(double x){ //for cos
	x = x*x;
	return 1.0-
	0.499999999999999817660187301143*x+
	0.0416666666676681381422709755368*x*x-
	0.00138888832785533543419979701513*x*x*x+
	0.0000248458867469448388393995277595*x*x*x*x;

}
double normalize(double x){ //TODO
	return 0.4;
}

double FPL_sin_64 (double x){
	if (x<0){
		return -1.0*FPL_sin_64(-1.0*x); //mamy jakieś funkcje do tych odwróceń znaków chyba
	}
	double y = normalize(x);
	if (y < 41.5/256.0){ //hax
		return poly9(y); //w zasadzie poly9(u), ale co to u
	}
	int i = round(256.0*y); //w zakresie od 16 do 201 ponoć
	double z = y-i/256.0; //|z|<1/512
	i = i-16; // tab[0] zwraca wartości dla i=16
	printf("%d\n", i);
	printf("%f, %f\n", F[i], G[i]);

	return (F[i]*poly8(z)+G[i]*poly9(z)); //poly powinny być niższe, ale po ch...
	//kolejność może mieć znaczenie, jest rozpisane śmieszniej gdyby było trzeba
	//oby nie
	//sin(Xi)*cos(z)+cos(Xi)*sin(z)

}

double FPL_cos_64 (double x){
	if (x<0){
			return FPL_cos_64(-1.0*x); //mamy jakieś funkcje do tych odwróceń znaków chyba
		}
	double y = normalize(x);
	if(y < 31.5/256.0){
		return poly8(y);
	}
	int i = round(256.0*y); //w zakresie od 16 do 201 ponoć
	double z = y-i/256.0; //|z|<1/512
	i = i-16; // tab[0] zwraca wartości dla i=16
	return G[i]*poly8(z) - F[i]*poly9(z);
	//cos(Xi)*cos(z)-sin(Xi)*sin(z)

}
