#include "config.h"
#include <FPL/float.h>
#include <FPL/interoperability.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <FPL/sintable.h>
#include <FPL/costable.h>



typedef struct  {
	int k;
	double x;
} reducted;

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

reducted normalize(double x){ //TODO
	if (x<M_PI/4 && x>-1.0*M_PI/4){
		reducted n;
		n.k = 0;
		n.x = x;
		return n;
	}
	long int k = round(x * 2/M_PI);
	printf("\nk= %d\n", k);
	x = x - k*M_PI/2;
	printf("\nx= %e\n", x);
	reducted n;
	n.k = k;
	n.x = x;
	return n;
}

double true_sin(double y){ //oblicza dla znormalizowanego y
	printf("\ntrue_sin= %e\n", y);
	if (y < 41.5/256.0){ //hax
		return poly9(y); //w zasadzie poly9(u), ale co to u
	}
	long int i = round(256.0*y); //w zakresie od 16 do 201 ponoć
	if (i > 201 || i<16){
		printf("\nindexOutOfBoundsExcetion y= %e, i= %d\n",y,i);
		exit(-1);
	}
	//printf("\ni %d\n", i);
	double z = y-i/256.0; //|z|<1/512
	//printf("\nz= %e\n", z);
	i = i-16; // tab[0] zwraca wartości dla i=16
	return (F[i]*poly8(z)+G[i]*poly9(z)); //poly powinny być niższe, ale po ch...
	//kolejność może mieć znaczenie, jest rozpisane śmieszniej gdyby było trzeba
	//oby nie
	//sin(Xi)*cos(z)+cos(Xi)*sin(z)
}
double true_cos (double y){ //liczy dla znormalizowanego y
	//printf("\ntrue_cos= %e\n", y);
	if(y < 31.5/256.0){
		return poly8(y);
	}
	long int i = round(256.0*y); //w zakresie od 16 do 201 ponoć
	if (i > 201 || i<16){
			printf("\nindexOutOfBoundsExcetion y= %e, i= %d\n",y,i);
			exit(-1);
		}
	double z = y-i/256.0; //|z|<1/512
	i = i-16; // tab[0] zwraca wartości dla i=16
	return G[i]*poly8(z) - F[i]*poly9(z);
	//cos(Xi)*cos(z)-sin(Xi)*sin(z)
}

double FPL_sin_64 (double x){
	if (x<0){
		return -1.0*FPL_sin_64(-1.0*x); //mamy jakieś funkcje do tych odwróceń znaków chyba
	}
	reducted n = normalize(x);
	switch (n.k%4){
		case 0: return true_sin(n.x);
		case 1: return true_cos(n.x);
		case 2: return -1.0 * true_sin(n.x);
		case 3: return -1.0 * true_cos(n.x);
	}

}



double FPL_cos_64 (double x){
	if (x<0){
			return FPL_cos_64(-1.0*x); //mamy jakieś funkcje do tych odwróceń znaków chyba
		}
	reducted n = normalize(x);
	switch (n.k%4){
			case 0: return true_cos(n.x);
			case 1: return -1.0 * true_sin(n.x);
			case 2: return -1.0 * true_cos(n.x);
			case 3: return true_sin(n.x);
		}

}
