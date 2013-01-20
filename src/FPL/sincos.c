#include "config.h"
#include <FPL/float.h>
#include <FPL/interoperability.h>
#include <FPL/functions.h>
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

FPL_float64 poly9(FPL_float64 x){
	return FPL_poly_eval_64(x,polytable9,10);
}

FPL_float64 poly8(FPL_float64 x){
	return FPL_poly_eval_64(x,polytable8,9);
}

reducted normalize(FPL_float64 x){
	if (x<DIV(PI,FLOAT64(4)) && FPL_compare(x, DIV(PI,FLOAT64(4))) >= 0){
		reducted n;
		n.k = 0;
		n.x = x;
		return n;
	}
	long int k = ROUND(MUL(x,DIV(FLOAT64(2),PI)));
	x = SUB(x,MUL(FLOAT64(k),DIV(PI,FLOAT64(2))));
	reducted n;
	n.k = k;
	n.x = x;
	return n;
}

FPL_float64 true_sin(FPL_float64 y){ //oblicza dla znormalizowanego y
	if (y < 0x3fc4c00000000000){ // y < 41.5/256
		return poly9(y);
	}
	long int i = ROUND(MUL(0x4070000000000000,y)); //w zakresie od 16 do 201, round(256*y)
	if (i > 200) i = 200;
	else if (i < 16) i = 16;

	FPL_float64 z = SUB(y,DIV(FLOAT64(i),0x4070000000000000)); //z = y-i/256.0; //|z|<1/512
	i = i-16; // tab[0] zwraca wartości dla i=16
	return ADD(
			MUL(F[i],poly8(z)),
			MUL(G[i],poly9(z))
			);//(F[i]*poly8(z)+G[i]*poly9(z));
	//sin(Xi)*cos(z)+cos(Xi)*sin(z)

}
FPL_float64 true_cos (FPL_float64 y){ //liczy dla znormalizowanego y
	if(y < 0x3fbf800000000000){ //y < 31.5 / 256
		return poly8(y);
	}
	long int i = ROUND(MUL(0x4070000000000000,y)); //w zakresie od 16 do 201
	if (i > 200) i = 200;
	else if (i < 16) i = 16;
	FPL_float64 z = SUB(y,DIV(FLOAT64(i),0x4070000000000000)); //|z|<1/512
	i = i-16; // tab[0] zwraca wartości dla i=16
	FPL_float64 first = MUL(G[i],poly8(z));
	FPL_float64 second = MUL(F[i],poly9(z));
	FPL_float64 wynik = SUB(first,second);
	return wynik; //G[i]*poly8(z) - F[i]*poly9(z);

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
			case 1: return true_cos(ABS(n.x)); //adjusted
			case 2: return true_sin(NEG(n.x));
			case 3: return NEG(true_cos(ABS(n.x)));
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
	reducted n = normalize(x);

	if (FPL_is_sign_minus_64(n.x)){
			switch (n.k%4){
				case 0: return true_cos(ABS(n.x));
				case 1: return true_sin(NEG(n.x));
				case 2: return NEG(true_cos(ABS(n.x)));
				case 3: return NEG(true_sin(NEG(n.x)));
			}
				}
	switch (n.k%4){
			case 0: return true_cos(ABS(n.x));
			case 1: return NEG(true_sin(n.x));
			case 2: return NEG(true_cos(ABS(n.x)));
			case 3: return true_sin(n.x);
		}

}
