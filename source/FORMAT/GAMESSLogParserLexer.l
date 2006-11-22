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
%x in_eigenvector_block
%x in_eigenvalues_block
%x in_basis_options_block
%x expecting_int_block
%s in_zmat_block
%s in_bond_block

%option noyywrap
%option case-insensitive

INT		[+-]?[0-9]+
FLOAT	[+-]?[0-9]+"."[0-9]+

MINUS_LINE [[:blank:]]*-+[[:blank:]]*\n
INT_LINE ([[:blank:]]*{INT})+[[:blank:]]*\n
FLOAT_LINE ([[:blank:]]*{FLOAT})+[[:blank:]]*\n
CHAR_LINE ([[:blank:]]*[a-zA-Z])+[[:blank:]]*\n
COORDINATE_ANGS_LINE ^.*COORDINATES[[:blank:]]OF[[:blank:]]ALL[[:blank:]]ATOMS[[:blank:]]ARE[[:blank:]]\(ANGS\).*\n.*\n.*\n	
ATOM_LINE ^[[:blank:]]*[a-zA-Z][[:alnum:]]*([[:blank:]]+{FLOAT}){4}.*\n
ZMAT_LINE	^.*ENCODED[[:blank:]]Z[[:blank:]]MATRIX.*\n.*\n
CHARGE_LINE ^.*NET[[:blank:]]CHARGES.*\n.*\n.*\n.*\n
POPULATION_LINE ^.*ATOMIC[[:blank:]]POPULATIONS\n.*\n
COEFFICIENT_LINE ^[[:blank:]]*{INT}[[:blank:]]*[a-zA-Z]+[[:blank:]]*{INT}[[:blank:]]*[a-zA-Z]+[[:blank:]]*({FLOAT}[[:blank:]]*)+[[:blank:]]*\n
ZMAT_BOND	 					[[:blank:]]*[0-9]+[[:blank:]]+1([[:blank:]]+{INT}){2}.*\n
ZMAT_OTHER 					[[:blank:]]*[0-9]+[[:blank:]]+[2-7].*\n
BOHR_LINE ^.*ATOM[[:blank:]]*ATOMIC[[:blank:]]*COORDINATES[[:blank:]]\(BOHR\).*\n.*\n
BASIS_OPTIONS_LINE ^[[:blank:]]*BASIS[[:blank:]]OPTIONS[[:blank:]]*\n.*\n
BASIS_OPTION	[[:blank:]]*[[:alnum:]]+=[[:blank:]]*[[:alnum:]]+[[:blank:]]*\n?
BOND_BLOCK ^[[:blank:]]*BOND[[:blank:]]ORDER.*ANALYSIS.*\n.*\n
BOND_ORDER_LINE ^([[:blank:]]*ATOM[[:blank:]]+PAIR[[:blank:]]+DIST[[:blank:]]+ORDER).*\n
BOND_DATA	[[:blank:]]*{INT}[[:blank:]]+{INT}[[:blank:]]+{FLOAT}[[:blank:]]+{FLOAT}[[:space:]]*
NUM_ORBITALS_ALPHA_LINE ^.*NUMBER[[:blank:]]OF[[:blank:]]OCCUPIED[[:blank:]]ORBITALS[[:blank:]]+\(ALPHA\)[[:blank:]]*=[[:blank:]]*/{INT}[[:blank:]]*\n
NUM_ORBITALS_BETA_LINE ^.*NUMBER[[:blank:]]OF[[:blank:]]OCCUPIED[[:blank:]]ORBITALS[[:blank:]]\(BETA[[:blank:]]\)[[:blank:]]*=[[:blank:]]*/{INT}[[:blank:]]*\n
ECP_LINE ^(.*)THE ECP RUN REMOVES
EIGENVECTOR_LINE ^{MINUS_LINE}.*EIGENVECTORS.*\n{MINUS_LINE}.*\n
%%

^.*RUN[[:blank:]]TITLE.*\n.*\n	{
	BEGIN(in_title_block);
	return(TK_TITLE);
}

<in_title_block>^.*\n	{
	BEGIN(INITIAL);
	GAMESSLogParserlval.text = strdup(yytext);
	return(TK_LINE);
}

{BASIS_OPTIONS_LINE} {
	BEGIN(in_basis_options_block);
	return(TK_BASIS_OPTIONS_START);
}

^[[:blank:]]*-+[[:blank:]]ALPHA[[:blank:]]SET[[:blank:]]-+[[:blank:]]*\n {
	printf("alpha\n");
	GAMESSLogParserlval.text = "alpha";
	return TK_SPINSET;
}

^[[:blank:]]*-+[[:blank:]]BETA[[:blank:]]SET[[:blank:]]-+[[:blank:]]*\n {
	printf("beta\n");
	GAMESSLogParserlval.text = "beta";
	return TK_SPINSET;
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

{NUM_ORBITALS_ALPHA_LINE} {
	BEGIN(expecting_int_block);
	GAMESSLogParserlval.text = "alpha";
	return (TK_NUM_ORBITALS);
}

{NUM_ORBITALS_BETA_LINE} {
	BEGIN(expecting_int_block);
	GAMESSLogParserlval.text = "beta";
	return (TK_NUM_ORBITALS);
}

<expecting_int_block>{INT}[[:blank:]]*\n? {
	BEGIN(INITIAL);
	GAMESSLogParserlval.number = atoi(yytext);
	return TK_INT;
}

{EIGENVECTOR_LINE} {
	BEGIN(in_eigenvector_block);
	return(TK_EIGEN_START);
}

{POPULATION_LINE}	{
	BEGIN(in_population_block);
	return(TK_POPULATION_START);
}

<in_population_block>^[[:blank:]]*[0-9]+.*\n	{
	GAMESSLogParserlval.text = strdup(yytext);
	return(TK_POPULATION);
}

<in_population_block>^.*\n	{
	BEGIN(INITIAL);
	return(TK_POPULATION_END);
}

<in_eigenvector_block>^{INT_LINE} {
	BEGIN(in_eigenvalues_block);
	return(TK_EIGENVALUES_START);
}

<in_eigenvalues_block>^{FLOAT_LINE} {
	GAMESSLogParserlval.text = strdup(yytext);
	return(TK_EIGENVALUES);
}

<in_eigenvalues_block>^{CHAR_LINE} {
	BEGIN(in_eigenvector_block);
	return TK_EIGENVALUES_END;
}

<in_eigenvector_block>{COEFFICIENT_LINE} {
	GAMESSLogParserlval.text = strdup(yytext);
	return(TK_COEFFICIENTS);
}

<in_eigenvector_block>^[[:blank:]]*\n/{INT_LINE} {
	return(TK_NEXT_EIGENBLOCK);
}

<in_eigenvector_block>^[[:blank:]]*\n	{
	BEGIN(INITIAL);
	return(TK_EIGEN_END);
}

<in_eigenvector_block,INITIAL>^.*\n	{
	GAMESSLogParserlval.text = strdup(yytext);
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
