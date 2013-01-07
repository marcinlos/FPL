/**
 * @file value.c
 */
#include "config.h"
#include "fplshell.h"
#include <FPL/interoperability.h>
#include <FPL/float.h>
#include <stdio.h>


value_object make_int(int x)
{
    value_object val;
    val.type = VAL_INT;
    val.int_value = x;
    return val;
}

value_object make_native(double x)
{
    value_object val;
    val.type = VAL_NATIVE;
    val.native_value = x;
    return val;
}

value_object make_float(FPL_float64 x)
{
    value_object val;
    val.type = VAL_FLOAT;
    val.float_value = x;
    return val;
}

value_object make_null(void)
{
    value_object val;
    val.type = VAL_NULL;
    return val;
}


static void to_int(value_object* val)
{
    int n;
    switch (val->type)
    {
    case VAL_NATIVE:
        n = (int) val->float_value;
        break;
    case VAL_FLOAT:
        n = FPL_to_integer(val->float_value);
        break;
    case VAL_INT:
        n = val->int_value;
        break;
    }
    *val = make_int(n);
}

static void to_native(value_object* val)
{
    double n;
    switch (val->type)
    {
    case VAL_NATIVE:
        n = val->native_value;
        break;
    case VAL_FLOAT:
        n = FPL_float64_to_double(val->float_value);
        break;
    case VAL_INT:
        n = val->int_value;
        break;
    }
    *val = make_native(n);
}

static void to_float(value_object* val)
{
    FPL_float64 n;
    switch (val->type)
    {
    case VAL_NATIVE:
        n = FPL_double_to_float64(val->native_value);
        break;
    case VAL_FLOAT:
        n = val->float_value;
        break;
    case VAL_INT:
        n = FPL_to_float64(val->int_value);
        break;
    }
    *val = make_float(n);
}


void insitu_cast_value(value_object* val, value_type target)
{
    if (val->type == VAL_NULL)
    {
        fprintf(stderr, "Error: trying to cast null value\n");
        *val = make_null();
    }
    if (val->type != target)
    {
        if (target == VAL_FLOAT)
            to_float(val);
        if (target == VAL_NATIVE)
            to_native(val);
        if (val->type == VAL_INT)
            to_int(val);
    }
}


value_object cast_value(value_object val, value_type target)
{
    value_object copy = val;
    insitu_cast_value(&copy, target);
    return copy;
}


