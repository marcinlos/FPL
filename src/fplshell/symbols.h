/**
 * @file symbols.h
 */

#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "fplshell.h"

typedef value_object (*fpl_function)(value_list*);

typedef enum
{
    SYM_FUNCTION,
    SYM_VARIABLE
} symbol_type;


typedef struct symbol_def
{
    const char* name;
    symbol_type type;
    union
    {
        value_object value;
        fpl_function function;
    };
} symbol_def;



#endif // SYMBOLS_H
