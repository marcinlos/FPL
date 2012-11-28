#include <FPL/float.h>
#include <FPL/interoperability.h>
#include <iostream>
#include <cstdio>
#include <map>
#include <cstdlib>

using namespace std;

template <typename Operation>
void test()
{
    Operation op;
    double scale = 1000024;
    std::map<int64_t, int> errors;
    for (int i = 0; i < 1000000; ++ i)
    {
        double x = rand() / double(RAND_MAX) - 0.5;
        double y = rand() / double(RAND_MAX) - 0.5;
        x *= scale;
        y *= scale;
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
        if (diff == 1024)
        {
            printf("- - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
            printf("x = %lx\ny = %lx\n", a, b);
            std::cout << "x = " << x << ", y = " << y << std::endl;
            std::cout << FPL_float64_to_double(res) << std::endl;
            std::cout << x + y << std::endl;
            printf("%lx\n%lx\n", res, actual);
        }
    }
    for (std::map<int64_t, int>::iterator i = errors.begin();
            i != errors.end(); ++ i)
    {
        std::cout << i->first << ": " << i->second << std::endl;
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
    test<mul>();
    return 0;
}


