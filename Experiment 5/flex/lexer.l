%{
#include <stdio.h>

int i = 0, e = 0, c = 0, a = 0, s = 0;
%}

%%

"include"|"void"|"main"|"int"|"float"|"double"|"scanf"|"char" {
    printf("Keyword: %s\n", yytext);
    i++;
}

[A-Za-z_][A-Za-z0-9_]* {
    printf("Identifier: %s\n", yytext);
    a++;
}

[0-9]+ {
    printf("Digit: %s\n", yytext);
    c++;
}

"="|"+"|"-"|"*"|"/" {
    printf("Operator: %s\n", yytext);
    e++;
}

[;|,|(|)|{|}|\[|\]] {
    printf("Delimiter: %s\n", yytext);
    s++;
}

["#|<|>|%|^|&|!|~|?|/"] {
    printf("Symbol: %s\n", yytext);
    s++;
}

[ \t\n]+ {
    // Ignore whitespaces and newlines
}

%%

int main(void) {
    yyin = fopen("lexinput.txt", "r");
    yylex();
    printf("\nIdentifiers: %d\n", a);
    printf("Digits: %d\n", c);
    printf("Operators: %d\n", e);
    printf("Keywords: %d\n", i);
    printf("Delimiters: %d\n", s);
    return 0;
}

int yywrap() {
    return 1;
}
