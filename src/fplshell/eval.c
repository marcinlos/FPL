/**
 * @file eval.c
 */
#include "config.h"
#include "fplshell.h"
#include "symbols.h"
#include <FPL/interoperability.h>
#include <stdio.h>
#include <stdlib.h>

static value_object eval_aux(expr* node);

static void print_type(value_type type)
{
    switch (type)
    {
    case VAL_FLOAT:
        printf("fpl");
        break;
    case VAL_NATIVE:
        printf("native");
        break;
    case VAL_INT:
        printf("int");
        break;
    case VAL_NULL:
        printf("-");
        break;
    }
}

static void print_value(value_object value)
{
    switch (value.type)
    {
    case VAL_FLOAT:
        printf("%f", FPL_float64_to_double(value.float_value));
        break;
    case VAL_NATIVE:
        printf("%f", value.native_value);
        break;
    case VAL_INT:
        printf("%d", value.int_value);
        break;
    case VAL_NULL:
        printf("<null>");
        break;
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
    else if (x->type == VAL_NATIVE)
    {
        insitu_cast_value(y, VAL_NATIVE);
        return VAL_NATIVE;
    }
    else if (y->type == VAL_NATIVE)
    {
        insitu_cast_value(x, VAL_NATIVE);
        return VAL_NATIVE;
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
        return make_float(FPL_addition_64(x.float_value, y.float_value));
    else if (common == VAL_NATIVE)
        return make_native(x.native_value + y.native_value);
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
        return make_float(FPL_subtraction_64(x.float_value, y.float_value));
    else if (common == VAL_NATIVE)
        return make_native(x.native_value - y.native_value);
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
        return make_float(FPL_multiplication_64(x.float_value, y.float_value));
    else if (common == VAL_NATIVE)
        return make_native(x.native_value * y.native_value);
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
        return make_float(FPL_division_64(x.float_value, y.float_value));
    else if (common == VAL_NATIVE)
        return make_native(x.native_value / y.native_value);
    else
        return make_int(x.int_value / y.int_value);
}


static value_object negate_value(value_object x)
{
    if (x.type == VAL_FLOAT)
        return make_float(FPL_negate_64(x.float_value));
    else if (x.type == VAL_NATIVE)
        return make_native(- x.native_value);
    else if (x.type == VAL_INT)
        return make_int(- x.int_value);
    else
    {
        fprintf(stderr, "Error: trying to negate null\n");
        return make_null();
    }
}

static value_object assign_value(expr* left, expr* right)
{
    value_object rhs = eval_aux(right);
    symbol_def* symbol = make_variable(left->text, rhs);
    insert_symbol(symbol);
    return rhs;
}

static value_object get_variable_value(const char* name)
{
    symbol_def* symbol = find_symbol(name);
    if (symbol == NULL)
    {
        fprintf(stderr, "Error: symbol `%s' not found\n", name);
        return make_null();
    }
    if (symbol->type != SYM_VARIABLE)
    {
        fprintf(stderr, "Error: `%s' is not a variable\n", name);
        return make_null();
    }
    return symbol->value;
}

static FPL_float64 to_float64(const char* text)
{
    return FPL_double_to_float64(atof(text));
}


static value_object eval_aux(expr* node)
{
    if (node == NULL)
        return make_null();
    if (node->type == EXP_ASSIGN)
        return assign_value(node->left, node->right);

    value_object left = eval_aux(node->left);
    value_object right = eval_aux(node->right);

    switch (node->type)
    {
    case EXP_PLUS:      return add_values(left, right);
    case EXP_MINUS:     return sub_values(left, right);
    case EXP_TIMES:     return mul_values(left, right);
    case EXP_DIV:       return div_values(left, right);
    case EXP_INT:       return make_int(atoi(node->text));
    case EXP_FLOAT:     return make_float(to_float64(node->text));
    case EXP_NATIVE:    return make_native(atof(node->text));
    case EXP_CALL:      return eval_function(node->text, node->args);
    case EXP_ID:        return get_variable_value(node->text);
    case EXP_UNMINUS:   return negate_value(left);
    }
}

void evaluate(expr* expression)
{
    value_object val = eval_aux(expression);
    if (val.type != VAL_NULL)
    {
        printf(" [");
        print_type(val.type);
        printf("]");
        printf(" = ");
        print_value(val);
    }
    printf("\n");
    free_expr(expression);
}

