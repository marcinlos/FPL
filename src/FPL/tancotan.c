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



double poly11(double x){
	return 1.00000000000000000000190024972*x+
			 0.333333333333333784780958329232*x*x*x+
			 0.133333333339884790301540013415*x*x*x*x*x+
			 0.0539682685032343322547213288727*x*x*x*x*x*x*x+
			 0.0218753822640505734535277170774*x*x*x*x*x*x*x*x*x+
			 0.00913304119848175097402765532662*x*x*x*x*x*x*x*x*x*x*x;
}

double poly5(double x){
	return 1.0000000000000000003276708763192724919140769142401*x+
	 0.33333333333264615783250028073185745312972880220438*x*x*x+
	 0.13333369361099346665006899524872832007098748411090*x*x*x*x*x;
}

typedef struct  {
	int k;
	double x;
} reducted;

reducted normalizer(double x){ //TODO
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
}

double truetan(double y){
	printf("\ny = %e\n",y);
	if(y<15.5/256.0){
		//printf("\ny = %e\n",y);
		return poly11(y);
	}
	long int i = round(256*y); //byc moze, nie bylo napisane :/
	printf("\ni= %ld\n", i);
	if (i > 200) i = 200;
	else if (i < 16) i = 16;
	double z = y - (i/256.0); //|z|<1/512
	i = i-16;//rzutowanie na tabelkę
	printf("\nz = %e\n",z);
	printf("\nF[i] = %e\n",F[i]);
	printf("\nrightside = %e\n",poly5(z) * (
			(G[i]+F[i])
			/
			(G[i]-poly5(z))
			));
	return F[i] + (poly5(z) * (
			(G[i]+F[i])
			/
			(G[i]-poly5(z))
			));
}
double truecotan(double y){
	printf("\ncoy = %e\n",y);
	return 1.0/truetan(y);
}

double FPL_tan_64(double x){
	if(x<0) return -1*FPL_tan_64(-1*x);
	printf("\nx = %e",x);
	reducted n = normalizer(x);
	printf("\nxnormal = %e\n",n.x);
	if(n.x < 0){
		switch(n.k % 2){
			case 0: return -1*truetan(-1*n.x);
			case 1: return truecotan(-1*n.x);
		}
	}
	switch(n.k%2){
		case 0: return truetan(n.x);
		case 1: return -1*truecotan(n.x);
	}
}

double FPL_cotan_64(double x){
	if(x<0) return -1*FPL_cotan_64(-1*x);
	reducted n = normalizer(x);
	if(n.x < 0){
		switch(n.k % 2){
			case 0: return -1*truecotan(-1*n.x);
			case 1: return truetan(-1*n.x);
		}
	}
	switch(n.k%2){
		case 0: return truecotan(n.x);
		case 1: return -1*truetan(n.x);
	}
}
