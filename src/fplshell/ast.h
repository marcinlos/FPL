/**
 * @file ast.h
 */
#ifndef FPSHELL_AST_H
#define FPSHELL_AST_H


typedef enum
{
    EXP_PLUS,
    EXP_MINUS,
    EXP_TIMES,
    EXP_DIV,
    EXP_FLOAT,
    EXP_NATIVE,
    EXP_INT,
    EXP_ASSIGN,
    EXP_UNMINUS,
    EXP_ID,
    EXP_CALL
} exp_type;


typedef struct expr
{
    struct expr* left;
    struct expr* right;
    struct expr_list* args;
    exp_type type;
    char* text;
} expr;


typedef struct expr_list
{
    expr* expression;
    struct expr_list* next;
} expr_list;


expr* make_binop(exp_type type, expr* left, expr* right);

expr* make_unop(exp_type type, expr* operand);

expr* make_float_expr(char* value);

expr* make_native_expr(char* value);

expr* make_int_expr(char* value);

expr* make_id_expr(char* name);

expr* make_call(expr* fun, expr_list* args);

expr* make_assignment(expr* var, expr* value);

expr_list* make_expr_singleton(expr* e);

expr_list* append_expr(expr_list* list, expr* e);

void free_expr(expr* e);

void free_expr_list(expr_list* list);


#endif // FPSHELL_AST_H
