/**
 * @file main.c
 */
#include "config.h"
#include "fplshell.h"
#include "parser.h"
#include <stdio.h>


static void greeting(void)
{
    printf("FPL shell, v1.0\n");
}

int main(int argc, char* argv[])
{
    create_symbol_table();
    register_builtins();
    greeting();
    yyparse();
    free_symbol_table();
    printf("\n");
    return 0;
}
