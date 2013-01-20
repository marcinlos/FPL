#include "config.h"
#include <FPL/float.h>
#include <FPL/interoperability.h>
#include <FPL/functions.h>
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

FPL_float64 poly11(FPL_float64 x){
	return FPL_poly_eval_64(x,polytable11,12);
}


FPL_float64 poly5(FPL_float64 x){
	return FPL_poly_eval_64(x,polytable5,6);
}

typedef struct  {
	int k;
	FPL_float64 x;
} reducted;

reducted normalizer(FPL_float64 x){
	if (x<DIV(PI,FLOAT64(4)) && FPL_compare(x, DIV(PI,FLOAT64(4))) >= 0){
		reducted n;
		n.k = 0;
		n.x = x;
		//printf("d");
		return n;
	}
	long int k = ROUND(MUL(x,DIV(FLOAT64(2),PI)));
	x = SUB(x,MUL(FLOAT64(k),DIV(PI,FLOAT64(2))));
	reducted n;
	n.k = k;
	n.x = x;
	return n;
}
FPL_float64 truetan(FPL_float64 y){
	if(y<0x3faf000000000000){
		return poly11(y);
	}
	long int i = ROUND(MUL(0x4070000000000000,y));
	if (i > 200) i = 200;
	else if (i < 16) i = 16;
	FPL_float64 z = SUB(y,DIV(FLOAT64(i),0x4070000000000000));  //|z|<1/512
	i = i-16;//rzutowanie na tabelkę
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
	return wynik;
}
FPL_float64 truecotan(FPL_float64 y){
	return DIV(ONE,truetan(y));
}

FPL_float64 FPL_tan_64(FPL_float64 x){
	if(FPL_is_sign_minus_64(x)) return NEG(FPL_tan_64(NEG(x)));
	reducted n = normalizer(x);
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
	//możnaby pewnie zrobić 1/FPL=tan_64, ale wtedy jedno dzielenie więcej w pewnych przypadkach.
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
