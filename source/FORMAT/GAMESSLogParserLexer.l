%{
#include <BALL/FORMAT/GAMESSLogFile.h>
#include <BALL/COMMON/parserDefinitions.h>

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
%x in_eigenvector_block
%x in_basis_options_block
%s in_zmat_block
%s in_bond_block

%option noyywrap
%option nounput
%option case-insensitive

INT		[+-]?[0-9]+
FLOAT	[+-]?[0-9]+"."[0-9]+

COORDINATE_ANGS_LINE ^.*COORDINATES[[:blank:]]OF[[:blank:]]ALL[[:blank:]]ATOMS[[:blank:]]ARE[[:blank:]]\(ANGS\).*\n.*\n.*\n	
ATOM_LINE ^[[:blank:]]*[a-zA-Z][[:alnum:]]*([[:blank:]]+{FLOAT}){4}.*\n
ZMAT_LINE	^.*ENCODED[[:blank:]]Z[[:blank:]]MATRIX.*\n.*\n
CHARGE_LINE ^.*NET[[:blank:]]CHARGES.*\n.*\n.*\n.*\n
POPULATION_LINE ^.*ATOMIC[[:blank:]]POPULATIONS\n.*\n
COEFFICIENT_LINE ^[[:blank:]]*({FLOAT}[[:blank:]]+)+{FLOAT}[[:blank:]]*\n
ZMAT_BOND	 					[[:blank:]]*[0-9]+[[:blank:]]+1([[:blank:]]+{INT}){2}.*\n
ZMAT_OTHER 					[[:blank:]]*[0-9]+[[:blank:]]+[2-7].*\n
BOHR_LINE ^.*ATOM[[:blank:]]*ATOMIC[[:blank:]]*COORDINATES[[:blank:]]\(BOHR\).*\n.*\n
BASIS_OPTIONS_LINE ^[[:blank:]]*BASIS[[:blank:]]OPTIONS[[:blank:]]*\n.*\n
BASIS_OPTION	[[:blank:]]*[[:alnum:]]+=[[:blank:]]*[[:alnum:]]+[[:blank:]]*\n?
BOND_BLOCK ^[[:blank:]]*BOND[[:blank:]]ORDER.*ANALYSIS.*\n.*\n
BOND_ORDER_LINE ^([[:blank:]]*ATOM[[:blank:]]+PAIR[[:blank:]]+DIST[[:blank:]]+ORDER).*\n
BOND_DATA	[[:blank:]]*{INT}[[:blank:]]+{INT}[[:blank:]]+{FLOAT}[[:blank:]]+{FLOAT}[[:space:]]*
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

{BASIS_OPTIONS_LINE} {
	BEGIN(in_basis_options_block);
	return(TK_BASIS_OPTIONS_START);
}

<in_basis_options_block>{BASIS_OPTION} {
	GAMESSLogParserlval.text = strdup(yytext);
	return (TK_OPTION);
}

<in_basis_options_block>^[[:blank:]]*\n {
	BEGIN(INITIAL);
	return (TK_BASIS_OPTIONS_END);
}

{BOND_BLOCK} {
	BEGIN(in_bond_block);
	return (TK_BOND_START);
}

<in_bond_block>{BOND_DATA} {
	GAMESSLogParserlval.text = strdup(yytext);
	return(TK_BOND);
}

<in_bond_block>.|\n	{
	GAMESSLogParserlval.text = yytext;
	return(TK_LINE);
}

<in_bond_block>^[[:blank:]]*--.*\n {
	BEGIN(INITIAL);
	return(TK_BOND_END);
}

{BOHR_LINE}	{
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

^.*EIGENVECTORS.*\n {
	BEGIN(in_eigenvector_block);
	return(TK_EIGEN_START);
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

<in_eigenvector_block>{COEFFICIENT_LINE} {
	GAMESSLogParserlval.text = yytext;
	return(TK_COEFFICIENTS);
}

<in_eigenvector_block>^[[:blank:]]*(\.)+.*\n	{
	BEGIN(INITIAL);
	return(TK_EIGEN_END);
}

<in_eigenvector_block,INITIAL>^.*\n	{
	GAMESSLogParserlval.text = yytext;
	return(TK_LINE);
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

// Commented out for performance reasons!
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

**/
