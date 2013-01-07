#ifndef GENERATORS_H
#define GENERATORS_H

#include <FPL/float.h>
#include <FPL/interoperability.h>
#include <cstdlib>

namespace fpl
{
namespace gen
{

struct bitwise_random_double
{
    double operator() () const
    {
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
};


struct uniform_random_double
{
    double max;

    uniform_random_double(double max)
    : max(max)
    { }

    double operator() () const
    {
        return (rand() / double(RAND_MAX) - .5) * 2 * max;
    }
};

} // namespace gen
} // namespace fpl

#endif // GENERATORS_H
