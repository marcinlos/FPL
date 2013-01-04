/**
 * @name function_test.h
 */

#ifndef FUNCTION_TEST_H
#define FUNCTION_TEST_H

#include <iostream>
#include <iomanip>
#include <map>
#include <cmath>
#include <limits>

namespace fpl
{
namespace test
{

template <class Collector, typename Approx, typename Target, class Generator>
void function_test(const Approx& approx, const Target& target,
        int n, const Generator& random, Collector& collector)
{
    for (int i = 0; i < n; ++ i)
    {
        double x = random();
        FPL_float64 xf = FPL_double_to_float64(x);
        double actual = target(x);
        double app = FPL_float64_to_double(approx(xf));
        double diff = std::fabs(actual - app);
        if(diff > 2) { std::cout << "x = " << x << std::endl; }
        collector(diff, app, actual);
    }
};


struct histogram_collector
{
    std::map<int, int> count;
    typedef std::map<int, int>::iterator iter;

    void operator () (double diff, double approx, double target)
    {
        int log = std::log10(diff);
        iter i = count.find(log);
        if (i != count.end())
            ++ i->second;
        else
        {
            count[log] = 1;
        }
    }

    void diagram()
    {
        int max = 0, min = std::numeric_limits<int>::max();
        for (iter i = count.begin(); i != count.end(); ++ i)
        {
            if (i->first > max) max = i->first;
            if (i->first < min) min = i->first;
        }
        min = std::max(-40, min);
        for (int j = min; j <= 10; ++ j)
        {
            std::cout << std::setw(4) << j << ": ";
            iter it = count.find(j);
            if (it != count.end())
            {
                std::string line(it->second, '*');
                std::cout << line;
            }
            std::cout << std::endl;
        }
    }

    void print()
    {
        for (iter i = count.begin(); i != count.end(); ++ i)
        {
            std::cout << std::setw(5) << i->first << ": "
                    << i->second << std::endl;
        }
    }

    void clear()
    {
        count.clear();
    }
};


} // namespace test
} // namespace fpl


#endif /* FUNCTION_TEST_H_ */
