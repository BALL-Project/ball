%{
// $Id: parsedFunctionLexer.l,v 1.2 2002/12/12 10:22:09 oliver Exp $
#include <BALL/MATHS/parsedFunction.h>
#include <BALL/COMMON/parserDefinitions.h>

using namespace BALL;

#include "parsedFunctionParser.h"
%}

%option noyywrap
%option nounput

LETTER   [a-zA-Z]
SIGN     "+"|"-"
DIGIT            [0-9]
INT      {SIGN}?{DIGIT}+
REAL     {SIGN}?({DIGIT}*)((\.({DIGIT}*))?)((E|e)({SIGN}?){DIGIT}{1,3})?
ID       {LETTER}(({LETTER}|{DIGIT})*)

%%
[ \t] /* Good bye white space...*/
"\n"    {return '\n';}
";"     {return ';';}
"\+"    {return  '+';}
"\-"    {return  '-';}
"\*"    {return  '*';}
"\/"    {return  '/';}
"\^"    {return  '^';}
"("     {return  '(';}
")"     {return  ')';}
"="     {return  '=';}
{INT}   {ParsedFunctionlval.val=atoi(yytext); return NUM;}
{REAL}  {ParsedFunctionlval.val=atof(yytext); return NUM;}
{ID}    {if (ParsedFunctionFunctions->has(yytext)) 
				 {
					 double(*func)(double)=(*ParsedFunctionFunctions)[yytext];
					 ParsedFunctionlval.func = func;
					 return FNCT;
					}
					else
					{ if (ParsedFunctionConstants->has(yytext))
					  {
							ParsedFunctionlval.var = (*ParsedFunctionConstants)[yytext];
							return VAR;
						}
						else
						{
							double *v = new double;
							(*ParsedFunctionConstants)[yytext] = v;
							ParsedFunctionlval.var = v;
							return VAR;
						}
					}
				}

%%
YY_BUFFER_STATE ParsedFunction_buffer;
void ParsedFunction_initBuffer(const char* buf)
{
	ParsedFunction_buffer = ParsedFunction_scan_string(buf);
}

void ParsedFunction_delBuffer()
{
	ParsedFunction_delete_buffer(ParsedFunction_buffer);
}
