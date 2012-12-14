/**
 * @file eval.c
 */
#include "config.h"
#include "fplshell.h"
#include <stdlib.h>

static double eval_aux(expr* node);


static double eval_function(char* name, expr_list* args)
{
    expr_list* head = args;
    while (head != NULL)
    {
        printf("Arg: %f\n", eval_aux(head->expression));
        head = head->next;
    }
    return 0.0;
}


static double eval_aux(expr* node)
{
    if (node == NULL)
        return 0.0;
    double left = eval_aux(node->left);
    double right = eval_aux(node->right);

    switch (node->type)
    {
    case EXP_PLUS:      return left + right;
    case EXP_MINUS:     return left - right;
    case EXP_TIMES:     return left * right;
    case EXP_DIV:       return left / right;
    case EXP_INT:       return atof(node->text);
    case EXP_FLOAT:     return atof(node->text);
    case EXP_CALL:      return eval_function(node->text, node->args);
    }
}

void evaluate(expr* expression)
{
    double value = eval_aux(expression);
    printf("val = %f\n", value);
    free_expr(expression);
}

