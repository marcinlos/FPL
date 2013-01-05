/**
 * @file symbols.h
 */

#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "fplshell.h"
#include "value_list.h"

typedef value_object (*fpl_function)(value_list*);

typedef enum
{
    SYM_FUNCTION,
    SYM_VARIABLE
} symbol_type;


typedef struct symbol_def
{
    char* name;
    symbol_type type;
    union
    {
        value_object value;
        fpl_function function;
    };
} symbol_def;


symbol_def* make_variable(const char* name, value_object value);
symbol_def* make_function(const char* name, fpl_function function);
void free_symbol(symbol_def* symbol);

void create_symbol_table(void);
void clear_symbol_table(void);
void free_symbol_table(void);

symbol_def* find_symbol(const char* name);
symbol_def* insert_symbol(symbol_def* symbol);
symbol_def* remove_symbol(const char* name);

void register_builtins(void);

void dbg_print_hashtable(void);

#endif // SYMBOLS_H
