%{
#include <BALL/FORMAT/GAMESSLogFile.h>

using namespace BALL;

#include "GAMESSLogParserParser.h"
#include <string.h>

#define YY_INPUT(buf,result,max_size) \
{ \
	((std::istream*)GAMESSLogFile::state.current_parser)->read(buf, max_size);\
	result = GAMESSLogFile::state.current_parser->gcount();\
}

%}

%x in_coord_block
%x in_title_block
%x in_charge_block
%x in_population_block
%s in_zmat_block

%option noyywrap
%option case-insensitive

INT		[+-]?[0-9]+
FLOAT	[+-]?[0-9]+"."[0-9]+

COORDINATE_ANGS_LINE ^.*COORDINATES[[:blank:]]OF[[:blank:]]ALL[[:blank:]]ATOMS[[:blank:]]ARE[[:blank:]]\(ANGS\).*\n.*\n.*\n	
ATOM_LINE ^[[:blank:]]*[a-zA-Z][[:alnum:]]*([[:blank:]]+{FLOAT}){4}.*\n
ZMAT_LINE	^.*ENCODED[[:blank:]]Z[[:blank:]]MATRIX.*\n.*\n
CHARGE_LINE ^.*NET[[:blank:]]CHARGES.*\n.*\n.*\n.*\n
POPULATION_LINE ^.*ATOMIC[[:blank:]]POPULATIONS\n.*\n

ZMAT_BOND	 					[[:blank:]]*[0-9]+[[:blank:]]+1([[:blank:]]+{INT}){2}.*\n
ZMAT_OTHER 					[[:blank:]]*[0-9]+[[:blank:]]+[2-7].*\n

%%

^.*RUN[[:blank:]]TITLE.*\n.*\n	{
	BEGIN(in_title_block);
	return(TK_TITLE);
}

<in_title_block>^.*\n	{
	BEGIN(INITIAL);
	GAMESSLogParserlval.text = yytext;
	return(TK_LINE);
}

^.*ATOM[[:blank:]]*ATOMIC[[:blank:]]*COORDINATES[[:blank:]]\(BOHR\).*\n.*\n	{
	BEGIN(in_coord_block);
	return(TK_COORDS_BOHR);
}

{COORDINATE_ANGS_LINE} {
	BEGIN(in_coord_block);
	return(TK_COORDS_ANGSTROM);
}

<in_coord_block>{ATOM_LINE}	{
	GAMESSLogParserlval.text = yytext;
	return(TK_ATOM);
}

<in_coord_block>^[[:blank:]]*\n	{
	BEGIN(INITIAL);
	return(TK_END_MOL);
}

{ZMAT_LINE}	{
	BEGIN(in_zmat_block);
	return(TK_ZMAT_START);
}

<in_zmat_block>{ZMAT_BOND}	{
	GAMESSLogParserlval.text = yytext;
	return(TK_BOND);
}

<in_zmat_block>{ZMAT_OTHER}	{
	GAMESSLogParserlval.text = yytext;
	return(TK_OTHER);
}

<in_zmat_block>^.*\n	{
	BEGIN(INITIAL);
	return(TK_ZMAT_END);
}

{CHARGE_LINE}	{
	BEGIN(in_charge_block);
	return(TK_CHARGE_START);
}

<in_charge_block>^[[:blank:]]*--.*\n	{
	BEGIN(INITIAL);
	return(TK_CHARGE_END);
}

<in_charge_block>^.*\n	{
	GAMESSLogParserlval.text = yytext;
	return(TK_CHARGE);
}

^.*\n	{
	GAMESSLogParserlval.text = yytext;
	return(TK_LINE);
}

{POPULATION_LINE}	{
	BEGIN(in_population_block);
	return(TK_POPULATION_START);
}

<in_population_block>^[[:blank:]]*[0-9]+.*\n	{
	GAMESSLogParserlval.text = yytext;
	return(TK_POPULATION);
}

<in_population_block>^.*\n	{
	BEGIN(INITIAL);
	return(TK_POPULATION_END);
}

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
