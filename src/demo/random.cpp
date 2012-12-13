#include <FPL/float.h>
#include <FPL/interoperability.h>
#include <FPL/functions.h>
#include <iostream>
#include <cstdio>
#include <map>
#include <cstdlib>
#include <cmath>
#include <iomanip>

using namespace std;



double bitwise_random_double()
{
    //return (rand() / double(RAND_MAX)) * 100000;
    int res = rand() % 1000;
    if (res == 0) return 0.0;
    if (res == 1) return 1. / 0.;
    if (res == 2) return -1. / 0.;
    if (res == 3) return FPL_float64_to_double(FPL_POSITIVE_NAN_64);
    uint64_t x = 0;
    for (int i = 0; i < 4; ++ i)
    {
        x <<= 16;
        x |= rand() & 0xffff;
    }
    return FPL_float64_to_double(x);
}

double random_double(double max)
{
    return (rand() / double(RAND_MAX) - .5) * 2 * max;
}

void test_truncation()
{
    for (int i = 0; i < 1000; ++ i)
    {
        double r = (rand() / double(RAND_MAX) - .5) * 1000000000;
        int res = FPL_to_integer(FPL_double_to_float64(r));
        int actual = static_cast<int>(r);
        if (res != actual)
        {
            std::cerr << res << " =/= " << actual << std::endl;
        }
    }
}

void test_round()
{
    for (int i = 0; i < 1000; ++ i)
    {
        double r = (rand() / double(RAND_MAX) - .5) * 1000000000;
        int res = FPL_round(FPL_double_to_float64(r));
        int actual = round(r);
        if (res != actual)
        {
            std::cerr << res << " =/= " << actual << std::endl;
        }
    }
}

void compare_exp()
{
    for (int i = 0; i < 1000; ++ i)
    {
        double r = random_double(30);
        double res = FPL_float64_to_double(FPL_exponent_64(FPL_double_to_float64(r)));
        double actual = std::exp(r);
        double diff = std::fabs(actual - res);
        std::cout << diff;
        if (diff > 1)
        {
            std::cout << "(" << r << ", " << actual << " vs " << res << ")";
        }
        std::cout << std::endl;
    }
}


template <typename Operation>
void test()
{
    srand(1234);
    Operation op;
    std::map<int64_t, int> errors;
    for (int i = 0; i < 10000000; ++ i)
    {
        if (i % 1000000 == 0) std::cout << '.' << std::flush;
        double x = bitwise_random_double();
        double y = bitwise_random_double();

        FPL_float64 a = FPL_double_to_float64(x);
        FPL_float64 b = FPL_double_to_float64(y);
        FPL_float64 res = op(a, b);
        FPL_float64 actual = FPL_double_to_float64(op(x, y));
        std::map<int64_t, int>::iterator j = errors.find(res - actual);
        int64_t diff = res - actual;

        if (FPL_is_nan_64(res) && FPL_is_nan_64(actual))
            continue;

        if (j != errors.end())
            ++ j->second;
        else
            errors[diff] = 1;

        /*if (diff == 0x8000000000000000)
                {
                    printf("- - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
                    printf("x = %lx\ny = %lx\n", a, b);
                    std::cout << "x = " << x << "\ny = " << y << std::endl;
                    std::cout << "res = " << FPL_float64_to_double(res) << std::endl;
                    std::cout << "ok =  " << op(x, y) << endl;
                    printf("res = %lx\nok =  %lx\n", res, actual);
                }*/


        if (FPL_is_nan_64(res) && FPL_is_nan_64(actual))
            continue;
        if (diff > 1000 || diff < -1000)
        {
            printf("- - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
            printf("x = %lx\ny = %lx\n", a, b);
            std::cout << "x = " << x << "\ny = " << y << std::endl;
            std::cout << "res = " << FPL_float64_to_double(res) << std::endl;
            std::cout << "ok =  " << op(x, y) << std::endl;
            printf("res = %lx\nok =  %lx\n", res, actual);
        }
    }
    std::cout << endl;
    for (std::map<int64_t, int>::iterator i = errors.begin();
            i != errors.end(); ++ i)
    {
        //if (i->first < 100 && i->first > -1000)
        std::cout << i->first << ": " << std::dec << i->second << std::endl;
        //std::cout << FPL_float64_to_double(i->first) << std::endl;
    }
}

struct add
{
    FPL_float64 operator () (FPL_float64 x, FPL_float64 y)
    {
        return FPL_addition_64(x, y);
    }

    double operator () (double x, double y)
    {
        return x + y;
    }
};


struct mul
{
    FPL_float64 operator () (FPL_float64 x, FPL_float64 y)
    {
        return FPL_multiplication_64(x, y);
    }

    double operator () (double x, double y)
    {
        return x * y;
    }
};

struct division
{
    FPL_float64 operator () (FPL_float64 x, FPL_float64 y)
    {
        return FPL_division_64(x, y);
    }

    double operator () (double x, double y)
    {
        return x / y;
    }
};

void constPrinter(){
	FPL_float64 log2 = FPL_double_to_float64(0.69314718055994530941723212145817656807550);
	std::cout << log2;
}
void exprPrinter(){
	double x = 9.0;
	double res = FPL_exponent_64(x);
	cout << "Hexowo: " << std::hex << *(uint64_t*)(&res) << std::endl;
	//double FPL_exponent_64(double x);
	cout << FPL_float64_to_double(FPL_exponent_64(x)) << endl;
	cout << exp(9) << std::endl;
}
void randomlogPrinter(){
	srand(4321);
	int i;
	int good=0;
	int bad = 0;
	double table[10];
	int j = 0;
	for(i=0;i<1000;i++){
		double f = (double)rand() / RAND_MAX;
		double x = 0.0001 + f * (10000 - 0.0001);
		if(abs(logarithm(x)-log(x))>0.0000000000001){
			bad++;
			table[j] = x;
			j++;
		}
		else good++;
	}
	for(i=0;i<10;i++)
		cout << "bad number: " << table[i] << endl;
	cout << "good: " << good << endl;
	cout << "bad: " << bad << endl;
	//cout << "mineee "<< setprecision(40) << logarithm(x) << endl;
	//cout << "theirs " << setprecision(40)  << log(x) << endl;
}

void logPrinter(){

	double x = 6142;
	cout << "mineee "<< setprecision(40) << logarithm(x) << endl;
	cout << "theirs " << setprecision(40)  << log(x) << endl;
}

int main()
{
    //test_round();
    //exprPrinter();
    /*std::cout << "Multiplication";
    test<mul>();
    std::cout << "Addition";
    test<add>();*/
    //logPrinter();
    //compare_exp();
    //test_truncation();
    //test<mul>();
	randomlogPrinter();
    return 0;
}


