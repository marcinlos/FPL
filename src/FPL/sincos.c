#include "config.h"
#include <FPL/float.h>
#include <FPL/interoperability.h>
#include <FPL/functions.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <FPL/sintable.h>
#include <FPL/costable.h>
#include <FPL/macros.h>


static FPL_float64 polytable9[] = {
		0,
		0x3ff0000000000000,
		0,
		0xbfc55555555556ae,
		0,
		0x3f81111110c6b25e,
		0,
		0xbf2a01a81705fc8a,
		0,
		0x3ec7098c5a509ed1
};
static FPL_float64 polytable8[] = {
		0x3ff0000000000000,
		0,
		0xbfdffffffffffffd,
		0,
		0x3fa555555557891d,
		0,
		0xbf56c16b7c8a3214,
		0,
		0x3efa0d8456173af6
};


typedef struct  {
	int k;
	FPL_float64 x;
} reducted;

#define pi 3.141592653589793238462

double oldpoly9(double x){ // for sin
	return 0.999999999999999999393795829034*x -
	0.166666666666676221746125759480*x*x*x +
	0.00833333332487845206504547225678*x*x*x*x*x-
	0.000198413628378777807248883955724*x*x*x*x*x*x*x+
	0.00000274625991149636194057744682828*x*x*x*x*x*x*x*x*x;
}
FPL_float64 poly9(FPL_float64 x){
	return FPL_poly_eval_64(x,polytable9,10);
}
double oldpoly8(double x){ //for cos
	x = x*x;
	return 1.0-
	0.499999999999999817660187301143*x+
	0.0416666666676681381422709755368*x*x-
	0.00138888832785533543419979701513*x*x*x+
	0.0000248458867469448388393995277595*x*x*x*x;

}
FPL_float64 poly8(FPL_float64 x){
	return FPL_poly_eval_64(x,polytable8,9);
}

reducted normalize(FPL_float64 x){ //TODO
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
/*
	long int k = round(x * 2/M_PI);
	printf("\nk= %d\n", k);
	x = x - k*M_PI/2;
	printf("\nx= %.18f\n", x);
	reducted n;
	n.k = k;
	n.x = x;
	return n;*/
}

FPL_float64 true_sin(FPL_float64 y){ //oblicza dla znormalizowanego y
	//printf("\ntrue_sin= %e\n", FPL_float64_to_double(y));
	//printf("\ny = %e\n", FPL_float64_to_double(y));
	if (y < 0x3fc4c00000000000){ //hax, y < 41.5/256
		return poly9(y); //w zasadzie poly9(u), ale co to u
	}
	long int i = ROUND(MUL(0x4070000000000000,y)); //w zakresie od 16 do 201 ponoć, round(256*y)
	if (i > 201 || i<16){
		printf("\nindexOutOfBoundsExcetion y= %e, i= %ld\n",FPL_float64_to_double(y),i);
		exit(-1);
	}

	FPL_float64 z = SUB(y,DIV(FLOAT64(i),0x4070000000000000)); //z = y-i/256.0; //|z|<1/512
	//printf("\nz= %e\n", FPL_float64_to_double(z));
	i = i-16; // tab[0] zwraca wartości dla i=16
	//printf("\ni %d\n", i);
	//printf("\nFi = %e",FPL_float64_to_double(F[i]));
	//printf("\nGi = %e\n",FPL_float64_to_double(G[i]));
	return ADD(
			MUL(F[i],poly8(z)),
			MUL(G[i],poly9(z))
			);//(F[i]*poly8(z)+G[i]*poly9(z)); //poly powinny być niższe, ale po ch...
	//kolejność może mieć znaczenie, jest rozpisane śmieszniej gdyby było trzeba
	//oby nie
	//sin(Xi)*cos(z)+cos(Xi)*sin(z)

}
FPL_float64 true_cos (FPL_float64 y){ //liczy dla znormalizowanego y
	printf("\ntrue_cos= %e\n", FPL_float64_to_double(y));
	if(y < 0x3fbf800000000000){ //y < 31.5 / 256
		return poly8(y);
	}
	long int i = ROUND(MUL(0x4070000000000000,y)); //w zakresie od 16 do 201 ponoć
	if (i > 201 || i<16){
			printf("\nindexOutOfBoundsExcetion y= %e, i= %ld\n",FPL_float64_to_double(y),i);
			exit(-1);
		}
	FPL_float64 z = SUB(y,DIV(FLOAT64(i),0x4070000000000000)); //|z|<1/512
	printf("\nz= %e\n", FPL_float64_to_double(z));
	i = i-16; // tab[0] zwraca wartości dla i=16
	return SUB(MUL(G[i],poly8(z)),MUL(F[i],poly9(z))); //G[i]*poly8(z) - F[i]*poly9(z);
	//cos(Xi)*cos(z)-sin(Xi)*sin(z)
}

FPL_float64 FPL_sin_64 (FPL_float64 x){
	if (FPL_is_sign_minus_64(x)){
		return NEG(FPL_sin_64(NEG(x)));
	}
	reducted n = normalize(x);
	if (FPL_is_sign_minus_64(n.x)){
			switch (n.k%4){
			case 0: return NEG(true_sin(NEG(n.x)));
			case 1: printf("\ndupa\n");return true_cos(ABS(n.x)); //adjusted
			case 2: return true_sin(NEG(n.x));
			case 3: return true_cos(ABS(n.x));
			}
		}
	switch (n.k%4){
		case 0: return true_sin(n.x);
		case 1: return true_cos(ABS(n.x));
		case 2: return NEG(true_sin(n.x));
		case 3: return NEG(true_cos(ABS(n.x)));
	}

}



FPL_float64 FPL_cos_64 (FPL_float64 x){
	if (FPL_is_sign_minus_64(x)){
				return FPL_cos_64(NEG(x));
			}
	reducted n = normalize(n.x);
	if (FPL_is_sign_minus_64(n.x)){
			switch (n.k%4){
				case 0: return true_cos(ABS(n.x));
				case 1: return (true_sin(NEG(n.x)));
				case 2: return NEG(true_cos(ABS(n.x)));
				case 3: return NEG(true_sin(NEG(x)));
			}
				}
	switch (n.k%4){
			case 0: return true_cos(ABS(n.x));
			case 1: return NEG(true_sin(n.x));
			case 2: return NEG(true_cos(ABS(n.x)));
			case 3: return true_sin(n.x);
		}

}
