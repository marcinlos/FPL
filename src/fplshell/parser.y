
%{
#include <stdio.h>
#include "ast.h"
#include "fplshell.h"

#define  YYERROR_VERBOSE
%}

%union {
    expr* node;
    expr_list* list;
}

%token <node> ID INTEGER FLOAT
%type <node> stmt expr add_expr mult_expr postfix_expr atomic_expr
%type <list> expr_list

%locations

%destructor { free_expr($$); } <node>

%%

program
    :
    | stmt              { evaluate($1); }
    | program stmt      { evaluate($2); }
    | error             { yyclearin; yyerrok; }
    | program error     { yyclearin; yyerrok; } 
    ;
    
stmt
    : expr ';'
    ;
    
expr: add_expr
    ;
    
add_expr
    : add_expr '+' mult_expr        { $$ = make_binop(EXP_PLUS, $1, $3); }
    | add_expr '-' mult_expr        { $$ = make_binop(EXP_MINUS, $1, $3); }
    | mult_expr                         
    ;
    
mult_expr
    : mult_expr '*' postfix_expr    { $$ = make_binop(EXP_TIMES, $1, $3); }
    | mult_expr '/' postfix_expr    { $$ = make_binop(EXP_DIV, $1, $3); }
    | postfix_expr
    ;
    
postfix_expr
    : ID '(' expr_list ')'          { $$ = make_call($1, $3); }
    | ID '(' ')'                    { $$ = make_call($1, NULL); }
    | atomic_expr
    ;
    
atomic_expr
    : ID
    | INTEGER   
    | FLOAT
    | '(' expr ')'                  { $$ = $2; }    
    ;

expr_list
    : expr                          { $$ = make_expr_singleton($1); }
    | expr_list ',' expr            { $$ = append_expr($1, $3); }
    ;

%%

void yyerror(const char* msg, ...) 
{
    fprintf(stderr, "(%d, %d): %s\n", yylloc.first_line, 
        yylloc.first_column, msg);
}

