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
#include <math.h>
#include <stdlib.h>


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
        printf("%lx\n", val.float_value);
    }
    return make_null();
}

static value_object poly_eval(value_list* args)
{
    int length = value_list_length(args);
    int degree = length - 2;
    if (length < 2)
    {
        fprintf(stderr, "poly_eval: Expected at least 2 arguments\n");
        return make_null();
    }
    insitu_cast_value(&args->value, VAL_FLOAT);
    FPL_float64 x = args->value.float_value;
    args = args->next;
    FPL_float64* coeffs = calloc(degree + 1, sizeof(FPL_float64));
    int i = degree;
    while (args != NULL)
    {
        insitu_cast_value(&args->value, VAL_FLOAT);
        coeffs[i --] = args->value.float_value;
        args = args->next;
    }
    FPL_float64 result = FPL_poly_eval_64(x, coeffs, degree);
    free(coeffs);
    return make_float(result);
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

// Wrappers for one-argument functions from math.h

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

static value_object pow_wrapper(value_list* args)
{
    if (value_list_length(args) != 2)
    {
        fprintf(stderr, "pow: Expected two arguments\n");
        return make_null();
    }
    value_object base = get_nth_value(args, 0);
    value_object exponent = get_nth_value(args, 1);
    insitu_cast_value(&base, VAL_NATIVE);
    insitu_cast_value(&exponent, VAL_NATIVE);
    return make_native(pow(base.native_value, exponent.native_value));
}


void register_builtins(void)
{
    insert_symbol(make_function("hex", hex));
    insert_symbol(make_function("pow", pow_wrapper));
    insert_symbol(make_function("poly_eval", poly_eval));

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

