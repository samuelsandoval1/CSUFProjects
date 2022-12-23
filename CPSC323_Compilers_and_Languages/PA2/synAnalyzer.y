/* Definitions */

%{
  #include<stdio.h>
  int flag=0;
%}

%union {
  int num;
}

%type <num> E T F

%token <num> NUMBER
  
%left '+' '-'
  
%left '*' '/' 
  
%left '(' ')'

%%

/* Grammar */
S : E               { printf("Result is = %i\n", $1); 
                      return 0;                           }
  ;

E : E '+' T         { $$ = $1 + $3; }
  | E '-' T         { $$ = $1 - $3; }
  | T               { $$ = $1; }
  ;

T : T '*' F         { $$ = $1 * $3; }
  | T '/' F         { if ($3 == 0) { // Zero division case
                        yyerror();
                        exit(0);
                      }
                      else {
                        $$ = $1 / $3;
                      }
                    }
  | F               { $$ = $1; }
  ;

F :'(' E ')'         { $$ = $2; }
  | '-' F            { $$ = -$2; }
  | NUMBER           { $$ = $1; }
  ;

%%

/* Program */
int yyerror(void) 
{
   printf("The entered arithmetic expression is invalid! \n");
   flag = 1;
}

int main(void)
{
    printf("Enter the expression:\n");

    yyparse();

    if(flag == 0) 
      printf("The entered arithmetic expression is valid \n");
}