#include <FPL/float.h>
#include <FPL/interoperability.h>
#include <iostream>
#include <cstdio>
#include <map>
#include <cstdlib>

using namespace std;

double random_double()
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

template <typename Operation>
void test()
{
    srand(1234);
    Operation op;
    std::map<int64_t, int> errors;
    for (int i = 0; i < 10000000; ++ i)
    {
        if (i % 1000000 == 0) std::cout << '.' << std::flush;
        double x = random_double();
        double y = random_double();

        FPL_float64 a = FPL_double_to_float64(x);
        FPL_float64 b = FPL_double_to_float64(y);
        FPL_float64 res = op(a, b);
        FPL_float64 actual = FPL_double_to_float64(op(x, y));
        std::map<int64_t, int>::iterator j = errors.find(res - actual);
        int64_t diff = res - actual;
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
        std::cout << std::hex << i->first << ": " << std::dec << i->second << std::endl;
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

int main()
{
    std::cout << 1. / 0. << std::endl;
    test<division>();
    return 0;
}


