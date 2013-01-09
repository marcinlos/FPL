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


static value_object dbg_print_hashtable_wrapper(value_list* args)
{
    if (value_list_length(args) > 0)
    {
        fprintf(stderr, "dbg_print_hashtable: Expected no arguments\n");
        return make_null();
    }
    dbg_print_hashtable();
    return make_null();
}


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

static value_object dec(value_list* args)
{
    if (value_list_length(args) != 1)
    {
        fprintf(stderr, "dec: Expected one argument\n");
        return make_null();
    }
    value_object val = args->value;
    insitu_cast_value(&val, VAL_FLOAT);
    if (val.type == VAL_FLOAT)
    {
        printf("%lu\n", val.float_value);
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


static value_object compare_wrapper(value_list* args)
{
    int length = value_list_length(args);
    if (length != 2)
    {
        fprintf(stderr, "compare: expected exactly 2 arguments\n");
        return make_null();
    }
    value_object left = get_nth_value(args, 0);
    value_object right = get_nth_value(args, 1);
    insitu_cast_value(&left, VAL_FLOAT);
    insitu_cast_value(&right, VAL_FLOAT);
    int result = FPL_compare_64(left.float_value, right.float_value);
    return make_int(result);
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
FPL_FUNCTION_WRAPPER(FPL_sqrt_64, fpl_sqrt)
FPL_FUNCTION_WRAPPER(FPL_logarithm_E_64, fpl_log)
FPL_FUNCTION_WRAPPER(FPL_sin_64,fpl_sin)
FPL_FUNCTION_WRAPPER(FPL_arctan_64, fpl_atan)
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
FUNCTION_WRAPPER(atan)

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
#define PUT(name, fun) insert_symbol(make_function(name, fun))

	PUT("dec", dec);
    PUT("hex", hex);
    PUT("pow", pow_wrapper);
    PUT("poly_eval", poly_eval);
    PUT("compare", compare_wrapper);
    PUT("dbg_print_hashtable", dbg_print_hashtable_wrapper);

#undef PUT

#define REG(name) insert_symbol(make_function(#name, name##_wrapper))

    REG(exp);
    REG(log);
    REG(sin);
    REG(cos);
    REG(tan);
    REG(sqrt);
    REG(atan);

    REG(fpl_exp);
    REG(fpl_sqrt);
	REG(fpl_log);
	REG(fpl_sin);
    REG(fpl_atan);

#undef REG
}


