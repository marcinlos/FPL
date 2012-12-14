/**
 * @file value.c
 */
#include "config.h"
#include "fplshell.h"
#include <stdio.h>


value_object make_int(int x)
{
    value_object val;
    val.type = VAL_INT;
    val.int_value = x;
    return val;
}

value_object make_float(double x)
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


void insitu_cast_value(value_object* val, value_type target)
{
    if (val->type == VAL_NULL)
    {
        fprintf(stderr, "Error: trying to cast null value\n");
        *val = make_null();
    }
    if (val->type != target)
    {
        if (val->type == VAL_FLOAT)
            *val = make_int(val->float_value);
        if (val->type == VAL_INT)
            *val = make_float(val->int_value);
    }
}


value_object cast_value(value_object val, value_type target)
{
    value_object copy = val;
    insitu_cast_value(&copy, target);
    return copy;
}


