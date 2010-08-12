%{

#include <BALL/STRUCTURE/smilesParser.h>

using namespace BALL;
using namespace std;

extern int yylex();
extern void yyerror(char const* s);

%}
%union {
	char*														text;
	Index														number;
	SmilesParser::SPAtom*						atom;
	SmilesParser::ConnectionList*		conns;
	SmilesParser::ChiralDef*				chiral;
}

%token	<text>		TK_ATOM
%token	<text>		TK_ORG_SUBSET_ATOM
%token	<text>		TK_HYDROGEN

%token	<number>	TK_DIGIT
%token	<number>	TK_ONE_TWO
%token	<number>	TK_ONE_TWO_THREE

%token	<text>		TK_CHIRAL_CLASS_TH
%token	<text>		TK_CHIRAL_CLASS_AL
%token	<text>		TK_CHIRAL_CLASS_SP
%token	<text>		TK_CHIRAL_CLASS_TB
%token	<text>		TK_CHIRAL_CLASS_OH

%type		<atom>		smiles
%type		<atom>		expression
%type		<atom>		node
%type		<atom>		atom
%type		<atom>		atom_symbol
%type		<atom>		org_subset_atom_symbol

%type		<number>	bond

%type		<number>	opt_charge
%type		<number>	pos_charge
%type		<number>	neg_charge

%type		<conns>		opt_connection_list
%type		<conns>		connection_list

%type		<number>	opt_isotope
%type		<number>	isotope

%type		<number>	opt_hydrogens

%type		<chiral>	opt_chirality

%start smiles

%%

smiles:	expression 
		{ 
		}
	;

expression:
		node { $$ = $1; }
	| node bond expression { new SmilesParser::SPBond($1, $3, $2); $$ = $1; }
	| node expression { new SmilesParser::SPBond($1, $2); $$ = $1; }
	;

node:
		atom { $$ = $1; }
	| node '(' expression ')' { new SmilesParser::SPBond($3, $1, 1); $$ = $1; }
	| node '(' bond expression ')' { new SmilesParser::SPBond($4, $1, $3); $$ = $1; }
	;

atom:
		org_subset_atom_symbol opt_connection_list 
		{
			$$ = $1; 
			if ($2 != 0) 
			{ 
				SmilesParser::state.current_parser->createBonds($1, $2); 
				delete ($2);
			} 
		}
	|	'[' opt_isotope atom_symbol opt_chirality opt_hydrogens opt_charge ']' opt_connection_list
		{ 
			$3->setFormalCharge($6); 
			$3->setIsotope($2);
			$3->setInBrackets(true);
			if ($5 != 0)
			{
				for (int i = 0; i < $5; i++)
				{
					new SmilesParser::SPBond(SmilesParser::state.current_parser->createAtom("H"), $3);
				}
			}
			if ($4 != 0)
			{	
				$3->setChirality(*$4);
				delete $4;
			}
			if ($8 != 0) 
			{ 
				SmilesParser::state.current_parser->createBonds($3, $8); 
				delete  $8;
			} 
			$$ = $3; 
		}
	| '[' opt_isotope TK_HYDROGEN opt_charge ']' 
		{ 
			$$ = SmilesParser::state.current_parser->createAtom("H"); 
			$$->setFormalCharge($4); 
			$$->setIsotope($2);
			$$->setInBrackets(true); 
		}
	;

opt_hydrogens:
		{ $$ = 0; }
	|	TK_HYDROGEN 
		{
			$$ = 1;
		}
	|	TK_HYDROGEN TK_DIGIT
		{
			$$ = $2;
		}
	;

org_subset_atom_symbol:
		TK_ORG_SUBSET_ATOM { $$ = SmilesParser::state.current_parser->createAtom(yylval.text); }
	;

atom_symbol:
		org_subset_atom_symbol { $$ = $1; }
	|	TK_ATOM { $$ = SmilesParser::state.current_parser->createAtom(yylval.text); }
	;

opt_charge: 
		{ 
			$$ = 0; 
		}
	| pos_charge 
		{
			$$ = $1;
		}
	| neg_charge 
		{
			$$ = $1;
		}
	;

pos_charge:
		'+' { $$ = 1; }
	|	'+' '+' { $$ = 2; }
	|	'+' '+' '+' { $$ = 3; }
	|	'+' TK_DIGIT { $$ = $2; }
	;

neg_charge:
		'-' {	$$ = -1; }
	|	'-' '-' { $$ = -2; }
	|	'-' '-' '-' { $$ = -3; }
	|	'-' TK_DIGIT { $$ = -$2; }
	;

opt_connection_list: 
		{	$$ = 0; }
	|	connection_list	
		{	
			$$ = $1; 
		}
	;
	
connection_list:
		TK_DIGIT 
		{
			$$ = new SmilesParser::ConnectionList;
			$$->push_back($1);
		}
	|	connection_list TK_DIGIT
		{
			$$ = $1;
			$$->push_back($2);
		}
	;

opt_isotope: 
		{ $$ = 0; }
	|	isotope { $$ = $1; }
	;


isotope:
		TK_DIGIT { $$ = $1;	}
	|	isotope TK_DIGIT { $$ = 10 * $1 + $2; }
	;

bond:
		'.'		{ $$ = 0; }
	|	'-'		{ $$ = 1; }
	| ':'		{ $$ = 1; }
	| '='		{ $$ = 2; }
	| '#'		{ $$ = 3; }
	| '/'		{ $$ = 1; }
	| '\\'	{ $$ = 1; }
	;

opt_chirality:
		{ $$ = 0; }
	|	'@' 
		{ 
			$$ = new SmilesParser::ChiralDef(SmilesParser::TH, 1); 
		}
	|	'@' '@' 
		{ 
			$$ = new SmilesParser::ChiralDef(SmilesParser::TH, 1); 
		}
	|	TK_CHIRAL_CLASS_TH TK_ONE_TWO 
		{
			$$ = new SmilesParser::ChiralDef(SmilesParser::TH, $2); 
		}
	|	TK_CHIRAL_CLASS_AL TK_ONE_TWO 
		{
			$$ = new SmilesParser::ChiralDef(SmilesParser::AL, $2); 
		}
	|	TK_CHIRAL_CLASS_TB TK_DIGIT
		{
			$$ = new SmilesParser::ChiralDef(SmilesParser::TB, $2); 
		}
	|	TK_CHIRAL_CLASS_SP TK_DIGIT
		{
			$$ = new SmilesParser::ChiralDef(SmilesParser::TB, $2); 
		}
	|	TK_CHIRAL_CLASS_OH TK_DIGIT
		{
			$$ = new SmilesParser::ChiralDef(SmilesParser::OH, $2); 
		}
	;
	
%%

void yyerror(char const* s)
{
	throw Exception::ParseError(__FILE__, 0, 
															SmilesParser::state.buffer, 
															String(s) + String(" (at position ") 
																+ String(SmilesParser::state.char_count) + String(")"));
}
