/**
 * @file ast.c
 */
#include "config.h"
#include "ast.h"
#include <stdlib.h>
#include <string.h>

static void clean_expr(expr* e)
{
    e->left = NULL;
    e->right = NULL;
    e->text = NULL;
    e->args = NULL;
}

expr* make_empty_expr()
{
    expr* e = malloc(sizeof(expr));
    clean_expr(e);
    return e;
}

expr* make_binop(exp_type type, expr* left, expr* right)
{
    expr* e = make_empty_expr();
    e->left = left;
    e->right = right;
    e->type = type;
    return e;
}

expr* make_unop(exp_type type, expr* operand)
{
    expr* e = make_empty_expr();
    e->left = operand;
    e->type = type;
    return e;
}

expr* make_float(char* value)
{
    expr* e = make_empty_expr();
    e->type = EXP_FLOAT;
    e->text = strdup(value);
    return e;
}

expr* make_int(char* value)
{
    expr* e = make_empty_expr();
    e->type = EXP_INT;
    e->text = strdup(value);
    return e;
}

expr* make_id(char* name)
{
    expr* e = make_empty_expr();
    e->type = EXP_ID;
    e->text = strdup(name);
    return e;
}

expr* make_call(char* name, expr_list* args)
{
    expr* e = make_empty_expr();
    e->type = EXP_CALL;
    e->text = strdup(name);
    e->args = args;
    return e;
}

expr_list* make_empty_list()
{
    expr_list* list = malloc(sizeof(expr_list));
    list->expression = NULL;
    list->next = NULL;
    return list;
}

expr_list* make_expr_singleton(expr* e)
{
    expr_list* list = make_empty_list();
    list->expression = e;
    return list;
}

expr_list* append_expr(expr_list* list, expr* e)
{
    expr_list* elem = make_expr_singleton(e);
    expr_list* head = list, *prev = NULL;
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

void free_expr(expr* e)
{
    if (e->left != NULL)
        free_expr(e->left);
    if (e->right != NULL)
        free_expr(e->right);
    if (e->args != NULL)
        free_expr_list(e->args);
    if (e->text != NULL)
        free(e->text);
    clean_expr(e);
}

void free_expr_list(expr_list* list)
{
    while (list != NULL)
    {
        if (list->expression)
            free_expr(list->expression);
        expr_list* tmp = list;
        list = list->next;
        free(tmp);
    }
}

