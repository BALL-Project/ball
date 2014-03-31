%{

#include <BALL/MOLMEC/AMBER/GAFFCESParser.h>
#include "GAFFCESParserParser.h"
#include <BALL/COMMON/parserDefinitions.h>

using namespace BALL;


#include <string.h>

#define YY_INPUT(buf,result,max_size) \
{ \
	result = GAFFCESParser::state.current_parser->read(buf, max_size);\
}


%}

%option noyywrap
%option nounput

%x APS

/*definition */
INTEGER 			[1-4]
SYMBOL  			[A-Z]+[A-Za-z]?
START_PROPERTY "["
END_PROPERTY   "]"

/*rules*/
%%
{INTEGER} {GAFFCESParserlval.number = atoi(yytext); return OPTIONAL_NUMBER;}
{SYMBOL} 	{GAFFCESParserlval.text = yytext; return ATOMSTRING;}

{START_PROPERTY} {BEGIN(APS); return TK_APS_START;}

<APS>","  {return TK_APS_AND_TERM;}
<APS>"."  {return TK_APS_OR_TERM;}

<APS>"DL" {return TK_APS_DELOCALIZED;}
<APS>"SB" {return TK_APS_PURE_SINGLE_BOND;}
<APS>"DB" {return TK_APS_PURE_DOUBLE_BOND;}
<APS>"TB" {return TK_APS_TRIPLE_BOND;}
<APS>"AB" {return TK_APS_AROMATIC_BOND;}
<APS>"sb" {return TK_APS_ALL_SINGLE_BONDS;}
<APS>"db" {return TK_APS_ALL_DOUBLE_BONDS;}
<APS>"tb" {return TK_APS_TRIPLE_BOND;}
<APS>"''" {return TK_APS_NOT_BOUND_TO_PARENT;}
<APS>"'"  {return TK_APS_BOUND_TO_PARENT;}

<APS>"AR1" {return TK_APS_PURE_AROMATIC_RING;}
<APS>"AR2" {return TK_APS_PLANAR_RING;}
<APS>"AR3" {return TK_APS_PLANAR_RING_WITH_DB;}
<APS>"AR4" {return TK_APS_OTHER_RING_ATOM;}
<APS>"AR5" {return TK_APS_PURE_ALIPHATIC_RING;}

<APS>"RG3" {return TK_APS_THREE_RING;}
<APS>"RG4" {return TK_APS_FOUR_RING;}
<APS>"RG5" {return TK_APS_FIVE_RING;}
<APS>"RG6" {return TK_APS_SIX_RING;}
<APS>"RG7" {return TK_APS_SEVEN_RING;}
<APS>"RG8" {return TK_APS_EIGHT_RING;}
<APS>"RG9" {return TK_APS_NINE_RING;}
<APS>"RG"	 {return TK_APS_RING;}
<APS>"NR"	 {return TK_APS_NON_RING;}
<APS>"*"	 {return TK_APS_TRUE;}

<APS>[0-9]+ {GAFFCESParserlval.number = atoi(yytext); return TK_APS_NUMBER;}

<APS>{END_PROPERTY}	 {BEGIN(INITIAL); return TK_APS_END;}

"("				{return TK_OPEN;}
")"				{return TK_END;}
","				{return TK_AND;}
"*"				{return TK_MATCH_TRUE;}


%%
/*user defined routines*/

void GAFFCESParser_destroy()
{
#ifdef BALL_HAS_YYLEX_DESTROY
	GAFFCESParserlex_destroy();
#endif
}
