#include <FPL/float.h>
#include <FPL/macros.h>
#include <FPL/interoperability.h>

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>



using namespace std;

int main(){
	double x;
	int i,j;
	double tab1[185];
	int j=0;
	for(i=16;i<201;i++){
		tab1[j] = 0.75+i/256.0+1/512.0;
		j++;
	}
	for(i=0;i<192;i++){
		tab1[i] = cos(tab1[i]);
	}

	ofstream outputFile;
	outputFile.open("sintable.h");
	outputFile << "FPL_float64 F[] = {";
	for (i=0;i<184;i++){
		outputFile <<  FLOAT64(tab1[i]) << "," << endl;
	}
//	outputFile <<  FLOAT64(tab1[184]) << "};";
	outputFile.close();
}
