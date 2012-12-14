/**
 * @file eval.c
 */
#include "config.h"
#include "fplshell.h"
#include "symbols.h"
#include <stdio.h>
#include <stdlib.h>

static value_object eval_aux(expr* node);

static void print_value(value_object value)
{
    switch (value.type)
    {
    case VAL_FLOAT: printf("%f", value.float_value); break;
    case VAL_INT: printf("%d", value.int_value); break;
    case VAL_NULL: printf("<null>"); break;
    }
}


static value_object eval_function(char* name, expr_list* args)
{
    expr_list* head = args;
    value_list* arg_vals = NULL;
    while (head != NULL)
    {
        value_object v = eval_aux(head->expression);
        arg_vals = append_value(arg_vals, v);
        head = head->next;
    }
    fpl_function fun;
    symbol_def* symbol = find_symbol(name);
    if (symbol == NULL)
    {
        fprintf(stderr, "Error: function `%s' does not exist\n", name);
        return make_null();
    }
    if (symbol->type != SYM_FUNCTION)
    {
        fprintf(stderr, "Error: `%s' is not a function\n", name);
        return make_null();
    }
    value_object val = (symbol->function)(arg_vals);
    return val;
}


static value_type coerce(value_object* x, value_object* y)
{
    if (x->type == VAL_NULL || y->type == VAL_NULL)
        return VAL_NULL;
    if (x->type == VAL_FLOAT)
    {
        insitu_cast_value(y, VAL_FLOAT);
        return VAL_FLOAT;
    }
    else if (y->type == VAL_FLOAT)
    {
        insitu_cast_value(x, VAL_FLOAT);
        return VAL_FLOAT;
    }
    else
        return VAL_INT;
}


static value_object add_values(value_object x, value_object y)
{
    value_type common = coerce(&x, &y);
    if (common == VAL_NULL)
    {
        fprintf(stderr, "Error: trying to add null objects\n");
        return make_null();
    }
    else if (common == VAL_FLOAT)
        return make_float(x.float_value + y.float_value);
    else
        return make_int(x.int_value + y.int_value);
}

static value_object sub_values(value_object x, value_object y)
{
    value_type common = coerce(&x, &y);
    if (common == VAL_NULL)
    {
        fprintf(stderr, "Error: trying to subtract null objects\n");
        return make_null();
    }
    else if (common == VAL_FLOAT)
        return make_float(x.float_value - y.float_value);
    else
        return make_int(x.int_value - y.int_value);
}

static value_object mul_values(value_object x, value_object y)
{
    value_type common = coerce(&x, &y);
    if (common == VAL_NULL)
    {
        fprintf(stderr, "Error: trying to multiply null objects\n");
        return make_null();
    }
    else if (common == VAL_FLOAT)
        return make_float(x.float_value * y.float_value);
    else
        return make_int(x.int_value * y.int_value);
}

static value_object div_values(value_object x, value_object y)
{
    value_type common = coerce(&x, &y);
    if (common == VAL_NULL)
    {
        fprintf(stderr, "Error: trying to divide null objects\n");
        return make_null();
    }
    else if (common == VAL_FLOAT)
        return make_float(x.float_value / y.float_value);
    else
        return make_int(x.int_value / y.int_value);
}


static value_object eval_aux(expr* node)
{
    if (node == NULL)
        return make_null();
    value_object left = eval_aux(node->left);
    value_object right = eval_aux(node->right);

    switch (node->type)
    {
    case EXP_PLUS:      return add_values(left, right);
    case EXP_MINUS:     return sub_values(left, right);
    case EXP_TIMES:     return mul_values(left, right);
    case EXP_DIV:       return div_values(left, right);
    case EXP_INT:       return make_int(atoi(node->text));
    case EXP_FLOAT:     return make_float(atof(node->text));
    case EXP_CALL:      return eval_function(node->text, node->args);
    }
}

void evaluate(expr* expression)
{
    value_object val = eval_aux(expression);
    printf("val = ");
    print_value(val);
    printf("\n");
    free_expr(expression);
}

