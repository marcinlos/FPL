/**
 * @file value_list.h
 */
#ifndef VALUE_LIST_H
#define VALUE_LIST_H

#include "fplshell.h"


typedef struct value_list
{
    value_object value;
    struct value_list* next;
} value_list;


value_list* make_value_singleton(value_object value);

value_list* append_value(value_list* list, value_object value);

value_object get_nth_value(value_list* list, int n);

void free_value_list(value_list* list);

int value_list_length(value_list* list);


#endif // VALUE_LIST_H
