
%{
#include <stdio.h>
#include "ast.h"
#include "parser.h"
#include <readline/readline.h>


#define YY_INPUT(buf,result,max_size) result = get_input(buf, max_size);

static int get_input(char *buf, int size)
{
    char *line;
    if (feof(yyin))
        return YY_NULL;
    line = readline("fplsh: ");
    if (!line)
        return YY_NULL;
    if (strlen(line) > size - 2) {
    yyerror("input line too long");
        return YY_NULL;
    }
    strcpy(buf, line);
    free(line);
    add_history(buf);

    return strlen(buf);
}


#define BUFFER_SIZE 256

static void update_loc(const char* text)
{
    yylloc.first_line = yylloc.last_line;
    yylloc.first_column = yylloc.last_column;
    char c;
    while (c = *text++)
    {
        if (c == '\n') 
        {
            ++ yylloc.last_line;
            yylloc.last_column = 0;
        }
        ++ yylloc.last_column;
    }
}


void print_error(const char* msg) 
{
    fprintf(stderr, "(%d, %d): %s\n", yylloc.first_line, 
        yylloc.first_column, msg);
}


#define YY_USER_ACTION update_loc(yytext);

%}

%option noyywrap

DIGIT           [0-9]

%%


[+\-*/(),;]                 return yytext[0];

[a-zA-Z_][a-zA-Z0-9_]*      yylval.node = make_id(yytext); return ID;

{DIGIT}+                    yylval.node = make_int(yytext); return INTEGER;

([1-9][0-9]*)?\.[0-9]*       yylval.node = make_float(yytext); return FLOAT;


[ \t\n] ;


<*>. { 
    char msg[BUFFER_SIZE];
    sprintf(msg, "Unexpected character: `%s'", yytext);
    print_error(msg);
}


%%

