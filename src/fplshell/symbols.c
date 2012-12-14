/**
 * @file symbols.c
 */
#include "config.h"
#include "symbols.h"
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 4


typedef struct hash_chain
{
    symbol_def* symbol;
    struct hash_chain* next;
} hash_chain;

// Bucket table
static hash_chain* hash_table[HASH_SIZE];


static void clear_chain_item(hash_chain* chain)
{
    chain->symbol = NULL;
    chain->next = NULL;
}

static void free_chain(hash_chain* chain)
{
    while (chain != NULL)
    {
        hash_chain* tmp = chain;
        chain = chain->next;
        if (tmp->symbol)
            free_symbol(tmp->symbol);
        free(tmp);
    }
}

static hash_chain* make_chain(symbol_def* symbol)
{
    hash_chain* chain = malloc(sizeof(hash_chain));
    chain->next = NULL;
    chain->symbol = symbol;
    return chain;
}

static hash_chain* append_symbol(hash_chain* chain, symbol_def* symbol)
{
    hash_chain* new_chain = make_chain(symbol);
    hash_chain* head = chain, *prev = NULL;
    while (head != NULL)
    {
        prev = head;
        head = head->next;
    }
    if (prev == NULL)
        chain = new_chain;
    else
        prev->next = new_chain;
    return chain;
}

static symbol_def* find_in_chain(hash_chain* chain, const char* name)
{
    while (chain != NULL)
    {
        symbol_def* symbol = chain->symbol;
        if (strcmp(symbol->name, name) == 0)
            return symbol;
        chain = chain->next;
    }
    return NULL;
}

static symbol_def* remove_from_chain(hash_chain** chain, const char* name)
{
    hash_chain* head = *chain, *prev = NULL;
    while (head != NULL)
    {
        symbol_def* symbol = head->symbol;
        if (strcmp(symbol->name, name) == 0)
        {
            if (prev == NULL)
                *chain = head->next;
            else
                prev->next = head->next;
            clear_chain_item(head);
            free_chain(head);
            return symbol;
        }
        head = head->next;
    }
    return NULL;
}

static unsigned int hash_function(const char* string)
{
    unsigned int k = 0;

    return k;
}

static int chain_index(const char* string)
{
    int idx = hash_function(string);
    return idx % HASH_SIZE;
}


symbol_def* make_variable(const char* name, value_object value)
{
    symbol_def* symbol = malloc(sizeof(symbol_def));
    symbol->name = strdup(name);
    symbol->type = SYM_VARIABLE;
    symbol->value = value;
    return symbol;
}

symbol_def* make_function(const char* name, fpl_function function)
{
    symbol_def* symbol = malloc(sizeof(symbol_def));
    symbol->name = strdup(name);
    symbol->type = SYM_FUNCTION;
    symbol->function = function;
    return symbol;
}

void free_symbol(symbol_def* symbol)
{
    free(symbol->name);
    free(symbol);
}

void create_symbol_table(void)
{
    //hash_table = calloc(HASH_SIZE, sizeof(hash_chain*));
}

void clear_symbol_table(void)
{
    int i;
    for (i = 0; i < HASH_SIZE; ++ i)
    {
        if (hash_table[i] != NULL)
            free_chain(hash_table[i]);
        hash_table[i] = NULL;
    }
}

void free_symbol_table(void)
{
    clear_symbol_table();
    //free(hash_table);
}

symbol_def* find_symbol(const char* name)
{
    int i = chain_index(name);
    return find_in_chain(hash_table[i], name);
}

symbol_def* insert_symbol(symbol_def* symbol)
{
    const char* name = symbol->name;
    int i = chain_index(name);
    symbol_def* prev = remove_from_chain(&hash_table[i], name);
    hash_table[i] = append_symbol(hash_table[i], symbol);
    return prev;

}

symbol_def* remove_symbol(const char* name)
{
    int i = chain_index(name);
    symbol_def* prev = remove_from_chain(&hash_table[i], name);
    return prev;
}


