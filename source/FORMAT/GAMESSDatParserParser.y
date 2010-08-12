%{

#include <BALL/FORMAT/GAMESSDatFile.h>

#include <iostream>
#include <cstdlib>

using namespace BALL;
using namespace std;

extern int yylex();
extern void yyerror(char const* s);

%}

%union {
	char*		text;
	float		value;
}

%token	<text>	 TK_COMMENT
%token	<text>	 TK_LINE
%token	<text>	 TK_BLOCK_START
%token	<text>	 TK_DATA
%token	<text>	 TK_ATOM
%token	<text>	 TK_GROUP
%token	<value>	 TK_FLOAT
%token	<text> 	 TK_BOND
%token					 TK_BLOCK_END
%token					 TK_DATA_BLOCK_START
%token					 TK_ZMAT_BLOCK_START
%token					 TK_VEC_BLOCK_START
%token					 TK_EQUALS

%type		<text>	non_blocked

%%

gamessfile:	/* empty */
		| gamessfile TK_COMMENT {}	
		| gamessfile block	{}
		| gamessfile non_blocked {}
		;

non_blocked: 
		  non_blocked	TK_LINE	{}
		| TK_LINE {};

block:
		start_block dataset TK_BLOCK_END { }
	|	TK_DATA_BLOCK_START mol_name mol_symmetry atom_data TK_BLOCK_END {
		}
	|	TK_ZMAT_BLOCK_START TK_GROUP zmat_block TK_BLOCK_END {}
	|	TK_VEC_BLOCK_START vec_block TK_BLOCK_END {}
	;

start_block:
	TK_BLOCK_START { GAMESSDatFile::state.current_parser->inBlock($1); }
	;

mol_name:
	| TK_LINE	{
			GAMESSDatFile::state.current_parser->newMolecule();
			GAMESSDatFile::state.current_parser->setMoleculeName($1);
		}
	;

mol_symmetry:
	| TK_LINE	{ }
	;

zmat_block:	
	  zmat_block TK_LINE { }
	|	TK_BOND {
			String s($1);
			GAMESSDatFile::state.current_parser->insertBond(s.getField(1,",").toInt()-1, s.getField(2,",").toInt()-1);
		}
	|	zmat_block TK_BOND {
			String s($2);
			GAMESSDatFile::state.current_parser->insertBond(s.getField(1,",").toInt()-1, s.getField(2,",").toInt()-1);
		}
	|	TK_LINE	{ }
	;

vec_block:	
	  vec_block	TK_LINE { }
	|	TK_LINE	{ }
	;

dataset: 
		data {}
	| dataset data {} 
	|	TK_LINE	{}
	| dataset TK_LINE { }
	;

atom_data: 
		atom_line {}
	| atom_data atom_line {} 
	;

data:
		TK_DATA	TK_EQUALS	TK_DATA	{ GAMESSDatFile::state.current_parser->insertBlockedData($1, $3); free($1); free($3); }
	;

atom_line:
	  TK_LINE {}
	|	TK_ATOM	TK_FLOAT TK_FLOAT TK_FLOAT TK_FLOAT
		{
			GAMESSDatFile::state.current_parser->insertAtom($1, $2, $3, $4, $5);
			free($1);
		}

%%
void yyerror(char const* s)
{
/**	throw Exception::ParseError(__FILE__, 0, 
															SmilesParser::state.buffer, 
															String(s) + String(" (at position ") 
																+ String(SmilesParser::state.char_count) + String(")"));
*/
	printf("Parse Error! (%s)\n", s);
}
