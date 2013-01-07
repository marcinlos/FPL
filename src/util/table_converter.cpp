/**
 * @file
 */
#include "config.h"
#include <FPL/float.h>
#include <FPL/interoperability.h>
#include <FPL/arctan_table.h>

#include <iostream>


template <std::size_t N>
void output_table(const double (&tab)[N])
{
    std::cout << "{" << std::endl;
    for (int i = 0; i < N; ++ i)
    {
        std::cout << std::hex << std::showbase << FPL_double_to_float64(tab[i])
                << (i != N - 1 ? "," : "");
        std::cout << std::endl;
    }
    std::cout << "}" << std::endl;
}

int main()
{
    std::cout << "FPL_float64 BASE[] = " << std::endl;
    output_table(BASE);
    std::cout << ";" << std::endl;

    std::cout << "FPL_float64 MAIN[][6] = \n{" << std::endl;

    const int ROWS = sizeof(MAIN)/sizeof(MAIN[0]);
    for (int i = 0; i < ROWS; ++ i)
    {
        output_table(MAIN[i]);
        std::cout << (i != ROWS - 1 ? "," : "") << std::endl;
    }

    std::cout << "};" << std::endl;
}




