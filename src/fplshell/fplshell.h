/*
 * @file fplshell.h
 */
#ifndef FPLSHELL_H
#define FPLSHELL_H

#include "ast.h"

typedef enum
{
    VAL_INT,
    VAL_FLOAT,
    VAL_NULL
} value_type;


typedef struct
{
    value_type type;
    union
    {
        int int_value;
        double float_value;
    };
} value_object;


value_object make_int(int x);
value_object make_float(double x);
value_object make_null(void);

void insitu_cast_value(value_object* val, value_type target);
value_object cast_value(value_object val, value_type target);


void evaluate(expr* expression);

#endif // FPLSHELL_H
