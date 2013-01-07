/*
 * @file fplshell.h
 */
#ifndef FPLSHELL_H
#define FPLSHELL_H

#include "ast.h"
#include <FPL/float.h>


typedef enum
{
    VAL_INT,
    VAL_FLOAT,
    VAL_NATIVE,
    VAL_NULL
} value_type;


typedef struct
{
    value_type type;
    union
    {
        int int_value;
        double native_value;
        FPL_float64 float_value;
    };
} value_object;


value_object make_int(int x);
value_object make_native(double x);
value_object make_float(FPL_float64 x);
value_object make_null(void);

void insitu_cast_value(value_object* val, value_type target);
value_object cast_value(value_object val, value_type target);


void evaluate(expr* expression);

#endif // FPLSHELL_H
