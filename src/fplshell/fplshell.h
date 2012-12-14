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
    VAL_VOID
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


void evaluate(expr* expression);

#endif // FPLSHELL_H
