#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

int main(){
	double x;
	int i,j;
	double tab1[201-16];
	j=0;
	for(i=16;i<202;i++){
		tab1[j] = i/256.0;
		j++;
	}
	for(i=0;i<185;i++){
		tab1[i] = cos(tab1[i]);
	}

	ofstream outputFile;
	outputFile.open("costable.h");
	outputFile << "double F[] = {";
	for (i=0;i<184;i++){
		outputFile << setprecision(40) << tab1[i] << "," << endl;
	}
	outputFile << setprecision(40) << tab1[184] << "};";
	outputFile.close();
}
