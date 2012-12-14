/**
 * @name function_test.h
 */

#ifndef FUNCTION_TEST_H
#define FUNCTION_TEST_H

#include <map>
#include <cmath>

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
        double diff = std::fabs(actual, app);
        collector(diff, app, actual);
    }
};


struct histogram_collector
{
    std::map<double, int> count;
};


} // namespace test
} // namespace fpl


#endif /* FUNCTION_TEST_H_ */
