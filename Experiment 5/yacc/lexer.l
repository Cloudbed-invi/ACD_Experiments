%{
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
%}

%%
[0-9]+("."[0-9]+)? { yylval = atof(yytext); return num; }  /* Recognizing integer and decimal numbers */
[+\-*/]            { return yytext[0]; }                   /* Recognizing operators */
[\n]               { return '\n'; }                        /* Recognizing newline */
[ \t]              { /* Ignore spaces and tabs */ }
.                  { printf("Unknown character: %s\n", yytext); }
%%

int yywrap() {
    return 1;
}
