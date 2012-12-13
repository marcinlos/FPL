#include "config.h"
#include <FPL/float.h>
#include <FPL/functions.h>
#include <FPL/interoperability.h>
#include <FPL/exptable.h>
#include <math.h>

#define LOGN2 4604418534313441775
#define SQRT2 4609047870845172685

FPL_float64 polynomial(double x){
	return 0.999999999999999999999623540967 + 0.99999999999999763152470487429*x + 0.500000000000002763221626745899*x*x + 0.166666676600773574707689190153*x*x*x + 0.0416666650109821193350748697127*x*x*x*x;
}



FPL_float64 FPL_exponent_64(double expr){
	//if (FPL_is_nan_64(exp)){
	//	return exp;
	//}
	//if(FPL_is_inf_64(exp)){
	//	return FPL_GET_SIGN_64(exp)?1:FPL_POSITIVE_ZERO_64:exp;
	//}

	int n = round(expr/log(2.0));
	double y = expr - n * log(2.0);
	int i = round(y*512);
	double z = y - i/512.0;
	int n2 = 1;
	printf("\nn= %d\n",n);
	for(n;n>0;n--){
		n2 = n2*2;
	}
	printf("\nn2= %d\n",n2);
	printf("\nz= %f\n",polynomial(z));
	printf("\ni= %d\n",i);

	i += 177; //transforming to table indexing
	printf("\nzlo= %e\n",TABLE[i] + TABLE[i]*polynomial(z));
	printf("\ntablei= %f\n",TABLE[i]);
	printf("\ndiff= %e\n",polynomial(z)-exp(z));
	return n2 * (TABLE[i]*polynomial(z));


}
