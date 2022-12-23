/* Definitions */
%{
  #include<stdio.h>
  #include "y.tab.h"
  void yyerror(char *);
%}

digit       [0-9]+
operator    [-+()*/]
whitespace  [\t]
endline     [\n]
  
/* Rule */
%%

{digit}     { 
              yylval.num=atoi(yytext);
              return NUMBER;
            }

{operator}    { return *yytext; } 

{whitespace}  { ; }
  
{endline}     {return 0;}
    
%%

/* Program */
int yywrap() {
  return 1;
}