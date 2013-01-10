#include "config.h"
#include <FPL/float.h>
#include <FPL/interoperability.h>
#include <FPL/functions.h>
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <FPL/macros.h>
#include <FPL/tantable.h>
#include <FPL/cotantable.h>


static FPL_float64 polytable11[] = {
		0,
		0x3ff0000000000000,
		0,
		0x3fd555555555555d,
		0,
		0x3fc111111114ab1a,
		0,
		0x3faba1ba987c7ca0,
		0,
		0x3f9666800da57954,
		0,
		0x3f82b4580a1580e7
};

static FPL_float64 polytable5[] = {
		0,
		0x3ff0000000000000,
		0,
		0x3fd55555555524fa,
		0,
		0x3fc1111416c1ce10
};

double oldpoly11(double x){
	return 1.00000000000000000000190024972*x+
			 0.333333333333333784780958329232*x*x*x+
			 0.133333333339884790301540013415*x*x*x*x*x+
			 0.0539682685032343322547213288727*x*x*x*x*x*x*x+
			 0.0218753822640505734535277170774*x*x*x*x*x*x*x*x*x+
			 0.00913304119848175097402765532662*x*x*x*x*x*x*x*x*x*x*x;
}

FPL_float64 poly11(FPL_float64 x){
	return FPL_poly_eval_64(x,polytable11,12);
}

double oldpoly5(double x){
	return 1.0000000000000000003276708763192724919140769142401*x+
	 0.33333333333264615783250028073185745312972880220438*x*x*x+
	 0.13333369361099346665006899524872832007098748411090*x*x*x*x*x;
}

FPL_float64 poly5(FPL_float64 x){
	return FPL_poly_eval_64(x,polytable5,6);
}

typedef struct  {
	int k;
	FPL_float64 x;
} reducted;

/*reducted normalizer(double x){ //TODO
	if (x<M_PI/4.0 && x>-1*M_PI/4.0){
		reducted n;
		n.k = 0;
		n.x = x;
		//printf("d");
		return n;
	}
	long int k = round(x*2.0/M_PI);
	printf("\nnorm k = %ld\n",k);
	x = x-(k*M_PI/2.0);
	printf("\nnorm x = %e\n",x);
	reducted n;
	n.k = k;
	n.x = x;
	return n;
}*/
reducted normalizer(FPL_float64 x){ //TODO
	if (x<DIV(PI,FLOAT64(4)) && FPL_compare(x, DIV(PI,FLOAT64(4))) >= 0){
		reducted n;
		n.k = 0;
		n.x = x;
		//printf("d");
		return n;
	}
	long int k = ROUND(MUL(x,DIV(FLOAT64(2),PI)));
	//printf("\nnorm k = %ld\n",k);
	x = SUB(x,MUL(FLOAT64(k),DIV(PI,FLOAT64(2))));
	//printf("\nnorm x = %e\n",FPL_float64_to_double(x));
	reducted n;
	n.k = k;
	n.x = x;
	return n;
}
FPL_float64 truetan(FPL_float64 y){
	//printf("\ny = %e\n",FPL_float64_to_double(y));
	if(y<0x3faf000000000000){
		//printf("\ny = %e\n",y);
		return poly11(y);
	}
	long int i = ROUND(MUL(0x4070000000000000,y)); //byc moze, nie bylo napisane :/
	//printf("\ni= %ld\n", i);
	if (i > 200) i = 200;
	else if (i < 16) i = 16;
	FPL_float64 z = SUB(y,DIV(FLOAT64(i),0x4070000000000000));  //|z|<1/512
	i = i-16;//rzutowanie na tabelkę
	//printf("\nz = %e\n",FPL_float64_to_double(z));
	//printf("\nF[i] = %e\n",FPL_float64_to_double(F[i]));
	/*printf("\nrightside = %e\n",FPL_float64_to_double(
			MUL(
					poly5(z),
					DIV(
							ADD(
							G[i],F[i]
							),
							SUB(
							G[i],poly5(z)
							)
					)
			)
			));*/
	FPL_float64 wynik = ADD(F[i],
			MUL(
					poly5(z),
					DIV(
							ADD(
							G[i],F[i]
							),
							SUB(
							G[i],poly5(z)
							)
					)
			)
			);
	//printf("\nwynik = %e\n",FPL_float64_to_double(wynik));
	return wynik;
	/*return F[i] + (poly5(z) * (
			(G[i]+F[i])
			/
			(G[i]-poly5(z))
			));*/
}
FPL_float64 truecotan(FPL_float64 y){
	//printf("\ncoy = %e\n",y);
	return DIV(ONE,truetan(y));
}

FPL_float64 FPL_tan_64(FPL_float64 x){
	if(FPL_is_sign_minus_64(x)) return NEG(FPL_tan_64(NEG(x)));
	//printf("\nx = %e",x);
	reducted n = normalizer(x);
	//printf("\nxnormal = %e\n",n.x);
	if (FPL_is_sign_minus_64(n.x)){
		switch(n.k % 2){
			case 0: return NEG(truetan(NEG(n.x)));
			case 1: return truecotan(NEG(n.x));
		}
	}
	switch(n.k%2){
		case 0: return truetan(n.x);
		case 1: return NEG(truecotan(n.x));
	}
}

FPL_float64 FPL_cotan_64(FPL_float64 x){
	//możnaby pewnie zrobić 1/FPL=tan_64, ale wtedy jedno dzielenie więcej w pewnych przypadkach. Chyba.
	if(FPL_is_sign_minus_64(x)) return NEG(FPL_cotan_64(NEG(x)));
	reducted n = normalizer(x);
	if (FPL_is_sign_minus_64(n.x)){
		switch(n.k % 2){
			case 0: return NEG(truecotan(NEG(n.x)));
			case 1: return truetan(NEG(n.x));
		}
	}
	switch(n.k%2){
		case 0: return truecotan(n.x);
		case 1: return NEG(truetan(n.x));
	}
}
