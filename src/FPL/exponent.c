#include "config.h"
#include <FPL/float.h>
#include <FPL/functions.h>
#include <FPL/interoperability.h>
#include <FPL/exptable.h>
#include <math.h>


double static polynomial(FPL_float64 z){
    double x = FPL_float64_to_double(z);
	return (0.999999999999999999999623540967 + 0.99999999999999763152470487429*x + 0.500000000000002763221626745899*x*x + 0.166666676600773574707689190153*x*x*x + 0.0416666650109821193350748697127*x*x*x*x);
}



FPL_float64 FPL_exponent_64(FPL_float64 expr){
	if (FPL_is_nan_64(expr)){
		return exp;
	}
	if(FPL_is_inf_64(expr)){
		return FPL_GET_SIGN_64(expr)?FPL_POSITIVE_ZERO_64:expr;
	}

	int n = FPL_round(
        FPL_division_64(expr, LOGN2)
    );

	FPL_float64 y = FPL_subtraction_64(
        expr,
        FPL_multiplication_64(
            FPL_double_to_float64((double)n),
            LOGN2
        )
    );
	int i = FPL_round(
        FPL_multiplication_64(
            y,
            FPL_double_to_float64((double)512)
        )
    ); //not sure czy to ucina, powinien być odpowiednik round(całość) w miejscu to_integer()
	FPL_float64 z = FPL_subtraction_64(y, FPL_double_to_float64(i/512.0));
	int n2 = 1;
	//printf("\nn= %d\n",n);
	for(n;n>0;n--){
		n2 = n2*2;
	}
//	printf("\nn2= %d\n",n2);
//	printf("\nz= %f\n",FPL_float64_to_double(z));
//	printf("\ni= %d\n",i);

	i += 177; //transforming to table indexing
	//printf("\nzlo= %e\n",TABLE[i] + TABLE[i]*polynomial(z));
	//printf("\ntablei= %f\n",TABLE[i]);
	//printf("\ndiff= %e\n",polynomial(z)-exp(z));
	return FPL_multiplication_64(FPL_double_to_float64((double)n2), FPL_multiplication_64(TABLE[i],polynomial(z)));

/*
	int n = round(FPL_float64_to_double(expr)/log(2.0));
    double y = FPL_float64_to_double(expr) - n * log(2.0);
    int i = round(y*512);
    double z = y - i/512.0;
    int n2 = 1;
    //printf("\nn= %d\n",n);
    for(n;n>0;n--){
        n2 = n2*2;
        printf("\nn2= %d\n",n2);
    }
    printf("\nn2= %d\n",n2);
    printf("\nz= %f\n",polynomial(z));
    printf("\ni= %d\n",i);
    printf("\niii = %f\n", round(y*512));
    printf("\ny = %f\n", y);

    i += 177; //transforming to table indexing
//    printf("\nzlo= %e\n",TABLE[i] + TABLE[i]*polynomial(z));
//    printf("\ntablei= %f\n",TABLE[i]);
//    printf("\ndiff= %e\n",polynomial(z)-exp(z));
    return FPL_double_to_float64(n2 * (FPL_float64_to_double(TABLE[i])*polynomial(z)));
*/
}
