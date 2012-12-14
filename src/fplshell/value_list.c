/**
 * @file value_list.c
 */
#include "config.h"
#include "value_list.h"
#include <stdlib.h>

static value_list* make_empty_list()
{
    value_list* list = malloc(sizeof(value_list));
    list->next = NULL;
    return list;
}


value_list* make_value_singleton(value_object value)
{
    value_list* list = make_empty_list();
    list->value = value;
    return list;
}

value_list* append_value(value_list* list, value_object value)
{
    value_list* elem = make_value_singleton(value);
    value_list* head = list, *prev = NULL;
    while (head != NULL)
    {
        prev = head;
        head = head->next;
    }
    if (prev == NULL)
        list = elem;
    else
        prev->next = elem;
    return list;
}

void free_value_list(value_list* list)
{
    while (list != NULL)
    {
        value_list* tmp = list;
        list = list->next;
        free(tmp);
    }
}


