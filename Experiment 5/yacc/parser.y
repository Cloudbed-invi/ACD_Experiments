%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define YYSTYPE double

void yyerror(const char *s);
int yylex(void);
%}

%token num
%left '+' '-'
%left '*' '/'

%%
st : st expr '\n' { printf("VALID\n"); }
   | st '\n'
   | error '\n' { printf("INVALID\n"); yyerrok; }
   ;

expr : num
     | expr '+' expr
     | expr '-' expr
     | expr '*' expr
     | expr '/' expr
     ;

%%

int main() {
    printf("ENTER AN EXPRESSION TO VALIDATE:\n");
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    printf("Error: %s\n", s);
}
