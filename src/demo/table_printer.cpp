#include <FPL/float.h>
//#include <FPL/macros.h>
//#include <FPL/interoperability.h>

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>



using namespace std;

int main(){
	double x;
	int i,j;
	double tab1[192];
	for(i=0;i<192;i++){
		tab1[j] = 0.75+i/256.0+1/512.0;
	}
	//for(i=0;i<192;i++){
	//	tab1[i] = cos(tab1[i]);
	//}

	ofstream outputFile;
	outputFile.open("logtable1.h");
	outputFile << "double TABLEX[] = {";
	for (i=0;i<184;i++){
		//outputFile <<  FLOAT64(tab1[i]) << "," << endl;
		FPL_addition_64(5uL,5uL);
	}
//	outputFile <<  FLOAT64(tab1[184]) << "};";
	outputFile.close();
}
