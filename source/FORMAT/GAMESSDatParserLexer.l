%{
#include <BALL/FORMAT/GAMESSDatFile.h>
#include <BALL/COMMON/parserDefinitions.h>

using namespace BALL;


#include "GAMESSDatParserParser.h"
#include <string.h>

#define YY_INPUT(buf,result,max_size) \
{ \
	((std::istream*)GAMESSDatFile::state.current_parser)->read(buf, max_size);\
	result = GAMESSDatFile::state.current_parser->gcount();\
}

%}

%s in_block
%s in_data_block
%s in_zmat_block
%s in_vec_block
%s in_coord_line

%option noyywrap
%option nounput

COMMENT	!
BLOCK_END		[[:space:]]*$[eE][nN][dD]
BLOCK_START [[:space:]]*$[a-zA-Z]*
DATA_BLOCK_START [[:space:]]*$[dD][aA][tT][aA]
ZMAT_BLOCK_START [[:space:]]*$[zZ][mM][aA][tT]
VEC_BLOCK_START  [[:space:]]*$[vV][eE][cC]([[:digit:]]*)

INT		[+-]?[0-9]+
FLOAT	[+-]?[0-9]+"."[0-9]+

ATOM_START ^[[:blank:]]*[[:alnum:]]+
ATOM_LINE	[[:blank:]]+{FLOAT}[[:blank:]]+{FLOAT}[[:blank:]]+{FLOAT}[[:blank:]]+{FLOAT}[[:space:]]*

VALID_START	[^$][^eE][^nN][^dD]
VALID_END		[[:blank:]]+.*\n|\n

ZMAT_BOND_START	 					[[:blank:]]*1[[:space:]]*,
ZMAT_BOND_ANGLE_START 		[[:blank:]]*2[[:space:]]*,
ZMAT_DIHEDRAL_ANGLE_START [[:blank:]]*3[[:space:]]*,
ZMAT_ATOM_PLANE 					[[:blank:]]*4[[:space:]]*,
ZMAT_LINEAR_BEND					[[:blank:]]*5[[:space:]]*,
ZMAT_DIHEDRAL_PLANE				[[:blank:]]*6[[:space:]]*,
ZMAT_GHOST_TORSION				[[:blank:]]*7[[:space:]]*,

ZMAT_TWO_NUM		[[:space:]]*({INT}|{FLOAT})[[:space:]]*,[[:space:]]*({INT}|{FLOAT})
ZMAT_THREE_NUM	([[:space:]]*({INT}|{FLOAT})[[:space:]]*,){2}[[:space:]]*({INT}|{FLOAT})
ZMAT_FOUR_NUM	  ([[:space:]]*({INT}|{FLOAT})[[:space:]]*,){3}[[:space:]]*({INT}|{FLOAT})
ZMAT_FIVE_NUM		([[:space:]]*({INT}|{FLOAT})[[:space:]]*,){4}[[:space:]]*({INT}|{FLOAT})
ZMAT_SIX_NUM	  ([[:space:]]*({INT}|{FLOAT})[[:space:]]*,){5}[[:space:]]*({INT}|{FLOAT})

%%

{COMMENT}+.*$					  {GAMESSDatParserlval.text = yytext; return(TK_COMMENT);}
{BLOCK_END}/{VALID_END}	{BEGIN(INITIAL); return(TK_BLOCK_END);}
{DATA_BLOCK_START}/{VALID_END}			{BEGIN(in_data_block); return(TK_DATA_BLOCK_START);}
{ZMAT_BLOCK_START}/{VALID_END}		{BEGIN(in_zmat_block); return(TK_ZMAT_BLOCK_START);}
{VEC_BLOCK_START}/{VALID_END}		{BEGIN(in_vec_block); return(TK_VEC_BLOCK_START);}
{BLOCK_START}/{VALID_END}					{GAMESSDatParserlval.text = yytext+1; BEGIN(in_block); return(TK_BLOCK_START);}
<in_block>[[:alnum:]\.()+\-]*	{GAMESSDatParserlval.text = strdup(yytext); return(TK_DATA);}
<in_block>=							{return(TK_EQUALS);}
<in_block>^[^=\n]*$				{GAMESSDatParserlval.text = yytext; return(TK_LINE);}
<in_data_block>{ATOM_START}/{ATOM_LINE} {BEGIN(in_coord_line);GAMESSDatParserlval.text = strdup(yytext); return TK_ATOM;}
<in_coord_line>{FLOAT}$	{GAMESSDatParserlval.value	= atof(yytext); BEGIN(in_data_block); return(TK_FLOAT);}
<in_coord_line>{FLOAT}	{GAMESSDatParserlval.value	= atof(yytext); return(TK_FLOAT);}

<in_zmat_block>[[:space:]]*[a-zA-Z][[:alnum:][:blank:]()]*/=	{GAMESSDatParserlval.text = yytext; return(TK_GROUP);}

<in_zmat_block>{ZMAT_BOND_START}{ZMAT_TWO_NUM}	{
	GAMESSDatParserlval.text = yytext;
	return(TK_BOND);
}

<in_zmat_block>{ZMAT_BOND_ANGLE_START}{ZMAT_THREE_NUM}	{
	GAMESSDatParserlval.text = yytext;
	return(TK_LINE);
}

<in_zmat_block>{ZMAT_DIHEDRAL_ANGLE_START}{ZMAT_FOUR_NUM}	{
	GAMESSDatParserlval.text = yytext;
	return(TK_LINE);
}

<in_zmat_block>{ZMAT_ATOM_PLANE}{ZMAT_FOUR_NUM}	{
	GAMESSDatParserlval.text = yytext;
	return(TK_LINE);
}

<in_zmat_block>{ZMAT_LINEAR_BEND}{ZMAT_THREE_NUM}	{
	GAMESSDatParserlval.text = yytext;
	return(TK_LINE);
}

<in_zmat_block>{ZMAT_DIHEDRAL_PLANE}{ZMAT_FIVE_NUM}	{
	GAMESSDatParserlval.text = yytext;
	return(TK_LINE);
}

<in_zmat_block>{ZMAT_GHOST_TORSION}{ZMAT_SIX_NUM}	{
	GAMESSDatParserlval.text = yytext;
	return(TK_LINE);
}

<in_zmat_block>,|\n	{}

<in_data_block,in_vec_block,INITIAL>^.*$			{GAMESSDatParserlval.text = yytext; return(TK_LINE);}
<*>.|\n										{}
%%

/**YY_BUFFER_STATE SmilesParser_buffer;
void SmilesParser_initBuffer(const char* buf)
{
	SmilesParser_buffer = SmilesParser_scan_string(buf);
}

void SmilesParser_delBuffer()
{
	SmilesParser_delete_buffer(SmilesParser_buffer);
}
**/
