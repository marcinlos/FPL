#include "config.h"
#include <FPL/float.h>
#include <FPL/interoperability.h>
//#include <FPL/logtable.h>
#include <FPL/logtable1.h>
#include <FPL/logtable2.h>
#include <FPL/logtable3.h>
#include <FPL/functions.h>
#include <FPL/macros.h>
//#include <math.h>
//#include <stdio.h>
static FPL_float64 polytable[] = {
		0,
		0x3ff0000000000000,
		0xbfe0000000000000,
		0x3fd5555555555556,
		0xbfd0000000000000,
		0x3fc999999994dbf5,
		0xbfc555555551110e,
		0x3fc24932cb3474bf,
		0xbfc0000cccd3e949
};

/*double static oldpolynomial(double x){
	return 1.4045029064076594981467548241576029709745070016382549949011e-96 + 0.9999999999999999999999917374202996065681905866839136611725*x
			- 0.4999999999999999999999925636765477056221792963438333048451*x*x + 0.3333333333333333495782192703860337840598850693074962264487*x*x*x
			- 0.2500000000000000146204006157773413568585527113102554985955*x*x*x*x + 0.1999999999913765484599888779513938442741120088779883813959*x*x*x*x*x
			- 0.1666666666589055587058662155957302248503575444427867117456*x*x*x*x*x*x + 0.1428588382922608524316678538001521754672181631046527236984*x*x*x*x*x*x*x
			- 0.125001525891841565968060649430009547354266601487839728337*x*x*x*x*x*x*x*x;
}*/

FPL_float64 static polynomial(FPL_float64 x){
	return FPL_poly_eval_64(x,polytable,9);
}

FPL_float64 FPL_logarithm_E_64 (FPL_float64 argument){
	if (FPL_is_sign_minus_64(argument) || !FPL_is_normal_64(argument)){
		return FPL_POSITIVE_NAN_64;
	}
	else if (FPL_is_zero_64(argument)){
		return FPL_NEGATIVE_INF_64;
	}
	else{
		int n = 0;
		int n2=1;
		FPL_float64 y;

		/*double oldy;
		double oldargument = FPL_float64_to_double(argument);
		int oldn = 0;
		int oldn2 = 1;
		while (!(oldargument/oldn2 >= 0.75 && oldargument/oldn2 < 1.5)){
				oldn2 = oldn2*2;
				oldn++;
			}
		oldy = oldargument/oldn2;
		int oldi = round((oldy-3/4.0-1/512.0)*256);
		if(oldi<0) oldi=0;
		else if (oldi>191) oldi=191;
		double oldz = (oldy - oldTABLEX[oldi]) * oldTABLEG[oldi];
		double oldwynik = oldTABLEF[oldi] + oldpolynomial(oldz);
		double oldwynik2 = oldwynik + oldn * log(2);*/


		if(argument < 0x3fe8000000000000)
		{
			FPL_float64 n3 = 0x3ff0000000000000; //=1
			FPL_unpacked64 unpacked;
			while (!(DIV(argument,n3) >= 0x3fe8000000000000 && DIV(argument,n3) < 0x3ff8000000000000))
			{
				FPL_UNPACK_64(n3,unpacked);
				unpacked.e -= 1;
				n3 = FPL_PACK_64(unpacked);
				n--;
				//printf("\n%e",FPL_float64_to_double(DIV(argument,FLOAT64(n2))));
			}
			y = DIV(argument,n3);
		}
		else{
			while (!(DIV(argument,FLOAT64(n2)) >= 0x3fe8000000000000 && DIV(argument,FLOAT64(n2)) < 0x3ff8000000000000)){
				n2 = n2*2;
				n++;
				//printf("\n%e",FPL_float64_to_double(DIV(argument,FLOAT64(n2))));
			}
			y = DIV(argument,FLOAT64(n2));
		}
		//jest pomiedzy 0.75 a 1.5

		//printf("\ny= %e\n", FPL_float64_to_double(y));
		//printf("\nn2= %d\n", n2);
		//printf("\nn= %d\n",n);
		//printf("\noldn= %d\n",n);
		//int i = round((y-3/4.0-1/512.0)*256);
		int i = ROUND(MUL(SUB(SUB(y,0x3fe8000000000000),0x3f5fffffffffffff),FLOAT64(256)));
		if(i<0) i=0;
		else if (i>191) i=191;
		//printf("\ni= %d\n",i);
		//printf("\noldi= %d\n",oldi);
		//printf("\ntabx= %f\n",FPL_float64_to_double(TABLEX[i]));
		//printf("\noldtabx= %f\n",oldTABLEX[oldi]);
		//printf("\ntabx20= %f\n",TABLEX[i+20]);
		//printf("\ntabx40= %f\n",TABLEX[i+40]);
		//printf("\ntabg= %f\n",FPL_float64_to_double(TABLEG[i]));
		//printf("\noldtabg= %f\n",oldTABLEG[oldi]);
		//FPL_float64 z = (y - TABLEX[i]) * TABLEG[i]; // <|1/384|
		FPL_float64 z = MUL(SUB(y,TABLEX[i]),TABLEG[i]);
		//printf("\nz= %e\n",FPL_float64_to_double(z));
		//printf("\noldz= %e\n",oldz);
		//printf("\ntabf= %e\n",TABLEF[i]);
		//printf("\ntabf= %e\n",TABLEF[i+60]);
		FPL_float64 wynik = ADD(TABLEF[i], polynomial(z));
		//printf("\nwynik = %e",FPL_float64_to_double(wynik));
		//printf("\noldwynik = %e",oldwynik);
		//printf("\n\n\n");
		//int j;
		//for(j=0;j<191;j++){
		//	printf("\ntabf= %e\n",TABLEF[j]);
		//}
		if(argument < 0x3fe8000000000000){
			//printf("\nwynik_zla= %e\n",FPL_float64_to_double(ADD(wynik, MUL(FLOAT64(n), LOGN2))));
		}
		return ADD(wynik, MUL(FLOAT64(n), LOGN2));
	}
}

FPL_float64 FPL_logarithm_64 (FPL_float64 base,FPL_float64 argument){
	//if arg <=0 or NaN or inf it's bad
	if (FPL_is_sign_minus_64(argument) || !FPL_is_normal_64(argument)){
		return FPL_POSITIVE_NAN_64;
	}
	else if (FPL_is_zero_64(argument)){
			return FPL_NEGATIVE_INF_64;
		}
	else{
		return DIV(FPL_logarithm_E_64(argument),FPL_logarithm_E_64(base));
	}
}
