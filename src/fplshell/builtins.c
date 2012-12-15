/**
 * @file builtins.c
 */
#include "config.h"
#include "symbols.h"
#include "value_list.h"
#include <FPL/float.h>
#include <FPL/interoperability.h>
#include <FPL/functions.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>


static value_object hex(value_list* args)
{
    if (value_list_length(args) != 1)
    {
        fprintf(stderr, "hex: Expected one argument\n");
        return make_null();
    }
    value_object val = args->value;
    insitu_cast_value(&val, VAL_FLOAT);
    if (val.type == VAL_FLOAT)
    {
        double fv = val.float_value;
        FPL_float64 a = FPL_double_to_float64(val.float_value);
        printf("%lx\n", a);
    }
    return make_null();
}

#define FPL_FUNCTION_WRAPPER(function, name)                            \
    static value_object name##_wrapper(value_list* args)                \
    {                                                                   \
        if (value_list_length(args) != 1)                               \
        {                                                               \
            fprintf(stderr, #name ": Expected one argument\n");         \
            return make_null();                                         \
        }                                                               \
        value_object val = args->value;                                 \
        insitu_cast_value(&val, VAL_FLOAT);                             \
        return make_float(function(val.float_value));                   \
    }


FPL_FUNCTION_WRAPPER(FPL_exponent_64, fpl_exp)


#define FUNCTION_WRAPPER(function)                                      \
    static value_object function##_wrapper(value_list* args)            \
    {                                                                   \
        if (value_list_length(args) != 1)                               \
        {                                                               \
            fprintf(stderr, #function ": Expected one argument\n");     \
            return make_null();                                         \
        }                                                               \
        value_object val = args->value;                                 \
        insitu_cast_value(&val, VAL_NATIVE);                            \
        return make_native(function(val.native_value));                 \
    }

FUNCTION_WRAPPER(log)
FUNCTION_WRAPPER(exp)
FUNCTION_WRAPPER(sin)
FUNCTION_WRAPPER(cos)
FUNCTION_WRAPPER(tan)
FUNCTION_WRAPPER(sqrt)

#undef FUNCTION_WRAPPER

void register_builtins(void)
{
    insert_symbol(make_function("hex", hex));

#define REG(name) insert_symbol(make_function(#name, name##_wrapper))

    REG(exp);
    REG(log);
    REG(sin);
    REG(cos);
    REG(tan);
    REG(sqrt);

    REG(fpl_exp);

#undef REG
}


