#include <FPL/float.h>
#include <FPL/macros.h>
#include <FPL/interoperability.h>
#include <FPL/functions.h>
#include "generators.h"
#include "function_test.h"
#include <iostream>
#include <cstdio>
#include <map>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <fstream>

using namespace std;
using namespace fpl::gen;

using namespace std;
using namespace fpl::gen;

void table_gen() {
	double x;
	int i, j;
	double tab1[185];
	j = 0;
	for (i = 16; i < 202; i++) {
		tab1[j] = i / 256.0;
		j++;
	}
	for (i = 0; i < 185; i++) {
		tab1[i] = tan(tab1[i]);
	}

	ofstream outputFile;
	outputFile.open("tantable.h");
	outputFile << "static FPL_float64 F[] = {";
	for (i = 0; i < 184; i++) {
		outputFile << FLOAT64(tab1[i]) << "," << endl;
	}
	outputFile << FLOAT64(tab1[184]) << "};";
	outputFile.close();
}

template<class Generator>
void test_truncation(const Generator& rand) {
	for (int i = 0; i < 1000; ++i) {
		double r = (rand() / double(RAND_MAX) - .5) * 1000000000;
		int res = FPL_to_integer(FPL_double_to_float64(r));
		int actual = static_cast<int>(r);
		if (res != actual) {
			std::cerr << res << " =/= " << actual << std::endl;
		}
	}
}

template<class Generator>
void test_round(const Generator& rand) {
	for (int i = 0; i < 1000; ++i) {
		double r = rand();
		int res = FPL_round(FPL_double_to_float64(r));
		int actual = round(r);
		if (res != actual) {
			std::cerr << res << " =/= " << actual << std::endl;
		}
	}
}

void compare_exp() {
	uniform_random_double rand(30);
	for (int i = 0; i < 1000; ++i) {
		double r = rand();
		double res = FPL_float64_to_double(
				FPL_exponent_64(FPL_double_to_float64(r)));
		double actual = std::exp(r);
		double diff = std::fabs(actual - res);
		std::cout << diff;
		if (diff > 1) {
			std::cout << "(" << r << ", " << actual << " vs " << res << ")";
		}
		std::cout << std::endl;
	}
}

template<typename Operation>
void test() {
	srand(1234);
	Operation op;
	std::map<int64_t, int> errors;
	bitwise_random_double rand;
	for (int i = 0; i < 1000000; ++i) {
		if (i % 1000000 == 0)
			std::cout << '.' << std::flush;
		double x = rand();
		double y = rand();

		FPL_float64 a = FPL_double_to_float64(x);
		FPL_float64 b = FPL_double_to_float64(y);
		FPL_float64 res = op(a, b);
		FPL_float64 actual = FPL_double_to_float64(op(x, y));
		std::map<int64_t, int>::iterator j = errors.find(res - actual);
		int64_t diff = res - actual;

		if (FPL_is_nan_64(res) && FPL_is_nan_64(actual))
			continue;

		if (j != errors.end())
			++j->second;
		else
			errors[diff] = 1;

		if (FPL_is_nan_64(res) && FPL_is_nan_64(actual))
			continue;
		if (diff > 1000 || diff < -1000) {
			printf("- - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
			printf("x = %lx\ny = %lx\n", a, b);
			std::cout << "x = " << x << "\ny = " << y << std::endl;
			std::cout << "res = " << FPL_float64_to_double(res) << std::endl;
			double act = op(x, y);
			std::cout << "ok =  " << act << std::endl;
			printf("res = %lx\nok =  %lx\n", res, actual);
		}
	}
	std::cout << endl;
	for (std::map<int64_t, int>::iterator i = errors.begin(); i != errors.end();
			++i) {

		std::cout << i->first << ": " << std::dec << i->second << std::endl;

	}
}

struct add {
	FPL_float64 operator ()(FPL_float64 x, FPL_float64 y) {
		return FPL_addition_64(x, y);
	}

	double operator ()(double x, double y) {
		return x + y;
	}
};

struct mul {
	FPL_float64 operator ()(FPL_float64 x, FPL_float64 y) {
		return FPL_multiplication_64(x, y);
	}

	double operator ()(double x, double y) {
		return x * y;
	}
};

struct division {
	FPL_float64 operator ()(FPL_float64 x, FPL_float64 y) {
		return FPL_division_64(x, y);
	}

	double operator ()(double x, double y) {
		return x / y;
	}
};

void constPrinter() {
	FPL_float64 log2 = FPL_double_to_float64(
			0.69314718055994530941723212145817656807550);
	std::cout << log2;
}
void exprPrinter() {
	double x = 9.0;
	double res = FPL_exponent_64(x);
	cout << "Hexowo: " << std::hex << *(uint64_t*) (&res) << std::endl;
	//double FPL_exponent_64(double x);
	cout << FPL_float64_to_double(FPL_exponent_64(x)) << endl;
	cout << exp(9) << std::endl;
}
void randomlogPrinter() {
	srand(4321);
	int i;
	int good = 0;
	int bad = 0;
	double table[10];
	int j = 0;
	for (i = 0; i < 1000; i++) {
		double f = (double) rand() / RAND_MAX;
		double x = 0.0001 + f * (10000 - 0.0001);
		if (abs(FPL_logarithm_E_64(x) - log(x)) > 0.0000000000001) {
			bad++;
			table[j] = x;
			j++;
		} else
			good++;
	}
	for (i = 0; i < 10; i++)
		cout << "bad number: " << table[i] << endl;
	cout << "good: " << good << endl;
	cout << "bad: " << bad << endl;
	//cout << "mineee "<< setprecision(40) << logarithm(x) << endl;
	//cout << "theirs " << setprecision(40)  << log(x) << endl;
}

void logPrinter() {

	double x = FPL_double_to_float64(9.0);
	//double res = FPL_exponent_64(x);
	//cout << "Hexowo: " << std::hex << *(uint64_t*)(&res) << std::endl;
	cout << FPL_float64_to_double(FPL_logarithm_E_64(x)) << endl;
	cout << log(9) << std::endl;
}

double eexp(double x) {
	return std::exp(x);
}
double esin(double x) {
	return std::sin(x);
}
double ecos(double x) {
	return std::cos(x);
}
double elog(double x) {
	return std::log(x);
}
double etan(double x) {
	return std::tan(x);
}
double ecotan(double x) {
	return 1.0 / (std::tan(x));
}
double earctan(double x) {
	return std::atan(x);
}

double sinPrinter() {

	double x = -7.4784; //7.2253, 7.17732 //6.766616e-01, 6.286816e-01 //-2.246160e-04 8.803840e-04

	FPL_float64 y = FLOAT64(x);
	cout << "mineee " << setprecision(40)
			<< FPL_float64_to_double(FPL_cos_64(y)) << endl;
	cout << "theirs " << setprecision(40) << cos(x) << endl;
}

double tanPrinter() {
	double x = -4.15137;
	FPL_float64 y = FLOAT64(x);
	cout << "mineee " << setprecision(40)
			<< FPL_float64_to_double(FPL_cotan_64(y)) << endl;
	cout << "theirs " << setprecision(40) << 1.0 / tan(x) << endl;
}
void test_arithmetic() {
	std::cout << "Multiplication";
	test<mul>();
	std::cout << "Addition";
	test<add>();
	std::cout << "Division";
	test<division>();
	std::cout << "Division";
}

int main() {
	fpl::test::histogram_collector hist;

	//test_round();
	//exprPrinter();
	//table_gen();
	//logPrinter();
	//compare_exp();
	//test_truncation();
	//test<mul>();
	//randomlogPrinter();
	//sinPrinter();

	//test_arithmetic();
	//double (*atan_p)(double) = std::atan;
	cout << "Eponent:" << endl;
	fpl::test::function_test(FPL_exponent_64, eexp, 100000,
			uniform_random_double(20.0), hist);
	hist.print();
	cout << endl;
	cout << "Logarithm:" << endl;
	hist.clear();
	fpl::test::function_test(FPL_logarithm_E_64, elog, 100000,
			uniform_random_double(0.0, 100.0), hist);
	hist.print();
	cout << endl;
	cout << "Sinus" << endl;
	hist.clear();
	fpl::test::function_test(FPL_sin_64, esin, 100000,
			uniform_random_double(50.0), hist);
	hist.print();
	cout << endl;
	cout << "Cosinus" << endl;
	hist.clear();
	fpl::test::function_test(FPL_cos_64, ecos, 100000,
			uniform_random_double(50.0), hist);
	hist.print();
	cout << endl;
	cout << "Tangent" << endl;
	hist.clear();
	fpl::test::function_test(FPL_tan_64, etan, 100000,
			uniform_random_double(50.0), hist);
	hist.print();
	cout << endl;
	cout << "Cotangent" << endl;
	hist.clear();
	fpl::test::function_test(FPL_cotan_64, ecotan, 100000,
			uniform_random_double(50.0), hist);
	hist.print();
	cout << "Arcus Tangent" << endl;
	hist.clear();
	fpl::test::function_test(FPL_arctan_64, earctan, 100000,
			uniform_random_double(50.0), hist);
	hist.print();
	//tanPrinter();
	return 0;
}

