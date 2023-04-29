%option noyywrap
/* Output with lexer symbols: */
%option header-file="read_units.lex.h"

%{
    
#include "read_units.h"
#include "read_units.tab.h"
    
%}


%%
 /* single character ops */
"+" |
"-" |
"*" |
"/" |
"^" |
"(" |
")"        { return yytext[0]; }
[0-9]+	   { yylval.d = atoi(yytext); return T_number; }
[a-zA-Z]+  { yylval.s = strdup(yytext); return T_symbol; }
[ \t]      { /* ignore white space */ }
.	       { printf("Illegal character: '%c'\n", *yytext); }
%%

