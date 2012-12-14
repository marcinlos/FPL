/**
 * @file builtins.c
 */
#include "config.h"
#include "symbols.h"
#include "value_list.h"
#include <stdio.h>
#include <stddef.h>
#include <math.h>

value_object sum(value_list* args)
{
    double sum = 0.0;
    while (args != NULL)
    {
        value_object val = args->value;
        if (val.type == VAL_FLOAT)
        {
            sum += val.float_value;
        }
        else
        {
            fprintf(stderr, "sum: Invalid argument type");
        }
        args = args->next;
    }
    return make_float(sum);
}



#define FUNCTION_WRAPPER(function)                                      \
    value_object function##_wrapper(value_list* args)                   \
    {                                                                   \
        if (value_list_length(args) != 1)                               \
        {                                                               \
            fprintf(stderr, #function "exp: Expected one argument\n");  \
            return make_null();                                         \
        }                                                               \
        value_object val = args->value;                                 \
        insitu_cast_value(&val, VAL_FLOAT);                             \
        return make_float(function(val.float_value));                   \
    }

FUNCTION_WRAPPER(log)
FUNCTION_WRAPPER(exp)
FUNCTION_WRAPPER(sin)
FUNCTION_WRAPPER(cos)
FUNCTION_WRAPPER(tan)

#undef FUNCTION_WRAPPER

void register_builtins(void)
{
    insert_symbol(make_function("sum", sum));

#define REG(name) insert_symbol(make_function(#name, name##_wrapper))

    REG(exp);
    REG(log);
    REG(sin);
    REG(cos);
    REG(tan);

#undef REG
}


