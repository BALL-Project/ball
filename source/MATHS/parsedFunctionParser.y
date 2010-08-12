%{
#include <math.h>
#include <BALL/MATHS/parsedFunction.h>

using namespace BALL;
using namespace std;

extern int ParsedFunctionlex();
extern void ParsedFunctionerror(char const* s);

double ParsedFunctionResult;
%}

%union {
	double *var;
	double(*func)(double);
	double val;
}

%token <val>  NUM
%token <func> FNCT 
%token <var>  VAR
%type  <val>  exp

%right '='
%left  '-' '+'
%left  '*' '/'
%left  NEG
%right '^'

%%
input:   /* empty */
       | input line
;

line:    '\n'
			 | exp ';' 					 { ParsedFunctionResult = $1;				}
			 | exp   '\n'        { ParsedFunctionResult = $1;       }
			 | error '\n'        { yyerrok;                         }
;

exp:     NUM               { $$ = $1;                         			        			}
			 | VAR               { $$ = *($1);																					}
			 | VAR '=' exp       { $$ = $3; *($1) = $3;                                 }
			 | FNCT '(' exp ')'  { $$ = (*($1))($3);																		}
			 | exp '+' exp       { $$ = $1 + $3;                    										}
			 | exp '-' exp       { $$ = $1 - $3;                    										}
			 | exp '*' exp       { $$ = $1 * $3;                    										}
			 | exp '/' exp       { $$ = $1 / $3;                    										}
			 | '-' exp %prec NEG { $$ = -$2;                        										}
			 | exp '^' exp       { $$ = pow($1, $3);                										}
			 | '(' exp ')'       { $$ = $2;                         										}
;

%%

void ParsedFunctionerror(char const* s)
{
	//throw Exception::ParseError(__FILE__, 0,
	cerr << "Parse Error! (" << String(s) << ")" << endl;
}
