%{

#include <BALL/STRUCTURE/smartsParser.h>
//#include <iostream>
#include <BALL/KERNEL/PTE.h>

using namespace BALL;
using namespace std;

extern int yylex();
extern void yyerror(char const* s);

#define YYDEBUG 1

%}
%union {
	char*														text;
	Index														number;
	SmartsParser::SPAtom*						atom;
	SmartsParser::SPNode*						node;
	SmartsParser::SPEdge*						edge;
	SmartsParser::SPBond*						sp_bond;
	SmartsParser::SPAtom::Property* 	property;
}

%token ';' ',' '&' '!' '#' '-' '+' '@'
%token	<text>		TK_ATOM
%token	<text>		TK_ORG_SUBSET_ATOM
%token	<text>		TK_HYDROGEN_EXPLICIT
%token	<text>		TK_HYDROGEN_IMPLICIT
%token	<text>		TK_RECURSIVE
%token	<text>		TK_AROMATIC_ATOM
%token	<text>		TK_ALIPHATIC_ATOM
%token	<text>		TK_ATOM_RING_SIZE
%token	<text>		TK_ATOM_NUM_RINGS
%token	<text>		TK_ATOM_VALENCE
%token	<text>		TK_ATOM_CONNECTED
%token	<text>		TK_ATOM_RING_CONNECTED
%token	<text>		TK_ATOM_DEGREE
%token	<text>		TK_ANY_ATOM
%token	<text>		TK_CHIRAL_CLASS_TH
%token	<text>		TK_CHIRAL_CLASS_AL
%token	<text>		TK_CHIRAL_CLASS_SP
%token	<text>		TK_CHIRAL_CLASS_TB
%token	<text>		TK_CHIRAL_CLASS_OH
%token  <number>  TK_DIGIT
%token  <number>  TK_ONE_TWO

%type   <node>    smarts
%type		<node>		rec_expression

%type   <atom>    atom
%type		<atom>		unbraced_atom
%type		<atom>		atom_symbol
%type		<atom>		unbraced_atom_symbol
%type		<property>		atom_property
%type		<node>		node
%type		<node>		node_expression

%type		<node>		sp_node
%type		<node>		atom_node

%type		<edge>		sp_edge
%type		<sp_bond>	bond
%type		<sp_bond>	ring_bond

%type		<number>	pos_charge
%type		<number>	neg_charge

%type		<number>	connection_list

%type		<number>	isotope
%type		<number>	atomic_number
%type		<number>	ring_size
%type		<number>	num_rings
%type		<number>	implicit_hydrogens
%type		<number>	explicit_hydrogens
%type		<number>	valence
%type		<number>	connected
%type		<number>	ring_connected
%type		<number>	degree

%type		<atom>	chirality

%left ';' 
%left ',' 
%left '&'

%start smarts

%%

smarts:
		node_expression
		{
			SmartsParser::state.current_parser->setRoot($1);
			$$ = $1;			
		}
	|	'(' smarts ')' // component level grouping
		{
			SmartsParser::state.current_parser->setNextComponentNumberToSubTree($2);
			$$ = $2;
		}
	|	smarts '.' smarts // dot disconnection
		{
			SmartsParser::state.current_parser->setRoot($1);
			SmartsParser::state.current_parser->setComponentGrouping(true);
			SmartsParser::SPBond* b = new SmartsParser::SPBond(SmartsParser::SPBond::NOT_NECESSARILY_CONNECTED);
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setSPBond(b);
			e->setFirstSPNode($1);
			e->setSecondSPNode($3);
			$1->addSPEdge(e);
			$$ = $1;
		}
	;

rec_expression:
		TK_RECURSIVE '(' node_expression ')'
		{
			$3->setRecursive(true);
			SmartsParser::state.current_parser->setRecursive(true);
			$$ = $3;
		}
	| '!' rec_expression
		{
			$2->setNot(true);
			$$ = $2;
		}
	;

node_expression:
		sp_node
		{
			$$ = $1;
		}
	|	sp_node node_expression
		{
			SmartsParser::SPBond* b = new SmartsParser::SPBond(SmartsParser::SPBond::SINGLE_OR_AROMATIC);
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setSPBond(b);
			e->setFirstSPNode($1);
			e->setSecondSPNode($2);
			$1->addSPEdge(e);
			$$ = $1;
		}
	|	sp_node sp_edge node_expression
		{
			$2->setFirstSPNode($1);
			$2->setSecondSPNode($3);
			$1->addSPEdge($2);
			$$ = $1;
		}
	;

sp_node:
		atom_node
		{
			$$ = $1;
		}
	|	sp_node '(' node_expression ')'
		{
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			SmartsParser::SPBond* b = new SmartsParser::SPBond(SmartsParser::SPBond::SINGLE_OR_AROMATIC);
			e->setSPBond(b);
			e->setFirstSPNode($1);
			e->setSecondSPNode($3);
			$1->addSPEdge(e);
			$$ = $1;
		}
	| sp_node '(' sp_edge node_expression ')'
		{
			$3->setFirstSPNode($1);
			$3->setSecondSPNode($4);
			$1->addSPEdge($3);
			$$ = $1;
		}
	;

sp_edge:
		bond
		{
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setSPBond($1);
			$$ = e;
		}
	|	'!' bond
		{
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setSPBond($2);
			e->setNot(true);
			$$ = e;
		}
	|	ring_bond
		{
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setSPBond($1);
			SmartsParser::state.current_parser->setNeedsSSSR(true);
			$$ = e;
		}
	|	'!' ring_bond
		{
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setSPBond($2);
			SmartsParser::state.current_parser->setNeedsSSSR(true);
			$2->setNot(true);
			$$ = e;
		}
	|	sp_edge ';' sp_edge
		{
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setInternal(true);
			e->setFirstSPEdge($1);
			e->setSecondSPEdge($3);
			e->setLogicalOperator(SmartsParser::AND_LOW);
			$$ = e;
		}
	| sp_edge ',' sp_edge
		{
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setInternal(true);
			e->setFirstSPEdge($1);
			e->setSecondSPEdge($3);
			e->setLogicalOperator(SmartsParser::OR);
			$$ = e;
		}
	|	sp_edge '&' sp_edge
		{
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setInternal(true);
			e->setFirstSPEdge($1);
			e->setSecondSPEdge($3);
			e->setLogicalOperator(SmartsParser::AND);
			$$ = e;
		}
	;

bond:
		'-'
		{
			$$ = new SmartsParser::SPBond(SmartsParser::SPBond::SINGLE);
		}
	|	'/'
		{
      $$ = new SmartsParser::SPBond(SmartsParser::SPBond::SINGLE_UP);
		}
	|	'\\'
		{
      $$ = new SmartsParser::SPBond(SmartsParser::SPBond::SINGLE_DOWN);
		}
	|	'/' '?' 
		{
			$$ = new SmartsParser::SPBond(SmartsParser::SPBond::SINGLE_UP_OR_ANY);
		}
	|	'\\' '?'
		{
			$$ = new SmartsParser::SPBond(SmartsParser::SPBond::SINGLE_DOWN_OR_ANY);
		}
	|	'='
		{
		  $$ = new SmartsParser::SPBond(SmartsParser::SPBond::DOUBLE);
		}
	|	'#'
		{
		  $$ = new SmartsParser::SPBond(SmartsParser::SPBond::TRIPLE);
		}
	|	':'
		{
		  $$ = new SmartsParser::SPBond(SmartsParser::SPBond::AROMATIC);
		}
	|	'~'
		{
		  $$ = new SmartsParser::SPBond(SmartsParser::SPBond::ANY);
		}
	;

ring_bond:
		'@'
		{
			$$ = new SmartsParser::SPBond(SmartsParser::SPBond::IN_RING);
		}
	;

atom_node:
		'[' node ']' connection_list
		{
			//$2->setInBrackets();
			SmartsParser::state.current_parser->addRingConnection($2, $4);
			$$ = $2;
		}
	| '[' node ']'
		{
			$$ = $2;
		}
	| unbraced_atom connection_list
		{
			$$ = new SmartsParser::SPNode($1);
			SmartsParser::state.current_parser->addNode($$);
			SmartsParser::state.current_parser->addRingConnection($$, $2);
		}
	|	unbraced_atom
		{
			$$ = new SmartsParser::SPNode($1);
			SmartsParser::state.current_parser->addNode($$);
		}
	|	rec_expression
		{
			$$ = $1;
		}
	;


node:
		atom
		{
			$$ = new SmartsParser::SPNode($1);
			SmartsParser::state.current_parser->addNode($$);
		}
	| unbraced_atom
		{
			$$ = new SmartsParser::SPNode($1);
			SmartsParser::state.current_parser->addNode($$);
		}
	|	rec_expression
		{
			$$ = $1;
		}
	|	node ';' node
		{
			$$ = new SmartsParser::SPNode($1, SmartsParser::AND_LOW, $3);
			SmartsParser::state.current_parser->addNode($$);
		}
	|	node ',' node
		{
			$$ = new SmartsParser::SPNode($1, SmartsParser::OR, $3);
			SmartsParser::state.current_parser->addNode($$);
		}
	|	node '&' node
		{
			$$ = new SmartsParser::SPNode($1, SmartsParser::AND, $3);
			SmartsParser::state.current_parser->addNode($$);
		}
	;

unbraced_atom:
		'!' unbraced_atom_symbol
		{
			$2->setNotProperty(SmartsParser::SPAtom::SYMBOL);
			$$ = $2;
		}
	|	unbraced_atom_symbol
		{
			$$ = $1;
		}
	|	'!' atom_property
		{
			$$ = new SmartsParser::SPAtom();
			$$->setNotProperty($2->getType());
			$$->setProperty(*$2);
			delete $2;
		}
	|	atom_property 
		{
			$$ = new SmartsParser::SPAtom();
			$$->setProperty(*$1);
			delete $1;
		}
	| unbraced_atom atom_property
		{
			$1->setProperty(*$2);
			$$ = $1;
			delete $2;
		}
	| unbraced_atom '!' atom_property
		{
			$1->setProperty(*$3);
			$1->setNotProperty($3->getType());
			delete $3;
			$$ = $1;
		}
	|	unbraced_atom '!' pos_charge
		{
			$1->setProperty(SmartsParser::SPAtom::CHARGE, $3);
			$1->setNotProperty(SmartsParser::SPAtom::CHARGE);
			$$ = $1;
		}
	|	unbraced_atom pos_charge
		{
			$1->setProperty(SmartsParser::SPAtom::CHARGE, $2);
			$$ = $1;
		}
	| '!' pos_charge
		{
			$$ = new SmartsParser::SPAtom();
			$$->setProperty(SmartsParser::SPAtom::CHARGE, $2);
			$$->setNotProperty(SmartsParser::SPAtom::CHARGE);
		}
	|	pos_charge
		{
			$$ = new SmartsParser::SPAtom();
			$$->setProperty(SmartsParser::SPAtom::CHARGE, $1);
		}
	;

atom:
		atom_symbol
		{
			$$ = $1;
		}
	|	'!' atom_symbol
		{
			$2->setNotProperty(SmartsParser::SPAtom::SYMBOL);
			
			$$ = $2;
		}
	|	'!' neg_charge
		{
			$$ = new SmartsParser::SPAtom();
			$$->setProperty(SmartsParser::SPAtom::CHARGE, $2);
			$$->setNotProperty(SmartsParser::SPAtom::CHARGE);
		}
	|	neg_charge
		{
			$$ = new SmartsParser::SPAtom();
			$$->setProperty(SmartsParser::SPAtom::CHARGE, $1);
		}
	|	'!' chirality
		{
			$$ = $2;
			$$->setNotProperty(SmartsParser::SPAtom::CHIRALITY);
		}
	|	chirality
		{
			$$ = $1;
		}
	| unbraced_atom '!' neg_charge
		{
			$1->setProperty(SmartsParser::SPAtom::CHARGE, $3);
			$1->setNotProperty(SmartsParser::SPAtom::CHARGE);
			$$ = $1;
		}
	|	unbraced_atom neg_charge
		{
			$1->setProperty(SmartsParser::SPAtom::CHARGE, $2);
			$$ = $1;
		}
	| unbraced_atom '!' chirality
		{
			$1->addPropertiesFromSPAtom($3);
			$1->setNotProperty(SmartsParser::SPAtom::CHIRALITY);
			delete $3;
			$$ = $1;
		}
	| unbraced_atom chirality
		{
			$1->addPropertiesFromSPAtom($2);
			delete $2;
			$$ = $1;
		}
	|	atom atom_property
		{
			$1->setProperty(*$2);
			delete $2;
			$$ = $1;
		}
	|	atom '!' neg_charge
		{
			$1->setProperty(SmartsParser::SPAtom::CHARGE, $3);
			$1->setNotProperty(SmartsParser::SPAtom::CHARGE);
			$$ = $1;
		}
	|	atom neg_charge
		{
			$1->setProperty(SmartsParser::SPAtom::CHARGE, $2);
			$$ = $1;
		}
	|	atom '!' pos_charge
		{
			$1->setProperty(SmartsParser::SPAtom::CHARGE, $3);
			$1->setNotProperty(SmartsParser::SPAtom::CHARGE);
			$$ = $1;
		}
	|	atom pos_charge
		{
			$1->setProperty(SmartsParser::SPAtom::CHARGE, $2);
			$$ = $1;
		}
	|	atom '!' chirality
		{
			$1->setNotProperty(SmartsParser::SPAtom::CHIRALITY);
			$1->addPropertiesFromSPAtom($3);
			delete $3;
			$$ = $1;
		}
	|	atom chirality
		{
			$1->addPropertiesFromSPAtom($2);
			delete $2;
			$$ = $1;
		}
	|	atom '!' atom_property
		{
			$1->setProperty(*$3);
			$1->setNotProperty($3->getType());
			delete $3;
			$$ = $1;
		}
	;

atom_property:
		num_rings
		{
			$$ = new SmartsParser::SPAtom::Property(SmartsParser::SPAtom::IN_NUM_RINGS, $1);
			SmartsParser::state.current_parser->setNeedsSSSR(true);
		}
	|	ring_size
		{
			$$ = new SmartsParser::SPAtom::Property(SmartsParser::SPAtom::IN_RING_SIZE, $1);
			SmartsParser::state.current_parser->setNeedsSSSR(true);
		}
	|	valence
		{
			$$ = new SmartsParser::SPAtom::Property(SmartsParser::SPAtom::VALENCE, $1);
		}
	|	connected
		{
			$$ = new SmartsParser::SPAtom::Property(SmartsParser::SPAtom::CONNECTED, $1);
		}
	|	implicit_hydrogens
		{
			$$ = new SmartsParser::SPAtom::Property(SmartsParser::SPAtom::IMPLICIT_HYDROGENS, $1);
		}
	|	explicit_hydrogens
		{
			$$ = new SmartsParser::SPAtom::Property(SmartsParser::SPAtom::EXPLICIT_HYDROGENS, $1);
		}
	|	degree
		{
			$$ = new SmartsParser::SPAtom::Property(SmartsParser::SPAtom::DEGREE, $1);
		}
	|	ring_connected
		{
			$$ = new SmartsParser::SPAtom::Property(SmartsParser::SPAtom::RING_CONNECTED, $1);
		}
	;

atom_symbol:
		isotope
		{
			$$ = new SmartsParser::SPAtom();
			$$->setProperty(SmartsParser::SPAtom::ISOTOPE, $1);
		}
	|	isotope TK_ORG_SUBSET_ATOM
		{
			$$ = new SmartsParser::SPAtom($2);
			$$->setProperty(SmartsParser::SPAtom::ISOTOPE, $1);
		}
	|	isotope TK_ATOM
		{
			$$ = new SmartsParser::SPAtom($2);
			$$->setProperty(SmartsParser::SPAtom::ISOTOPE, $1);
		}
	|	isotope TK_AROMATIC_ATOM
		{
			$$ = new SmartsParser::SPAtom($2);
			$$->setProperty(SmartsParser::SPAtom::ISOTOPE, $1);
		}
	| isotope TK_ALIPHATIC_ATOM
		{
			$$ = new SmartsParser::SPAtom($2);
			$$->setProperty(SmartsParser::SPAtom::ISOTOPE, $1);
		}
	|	isotope TK_ANY_ATOM
		{
			$$ = new SmartsParser::SPAtom();
			$$->setProperty(SmartsParser::SPAtom::ISOTOPE, $1);
		}
	|	isotope	atomic_number
		{
			$$ = new SmartsParser::SPAtom($2);
			$$->setProperty(SmartsParser::SPAtom::ISOTOPE, $1);
		}
	;

unbraced_atom_symbol:
		TK_ORG_SUBSET_ATOM
		{
			$$ = new SmartsParser::SPAtom($1);
		}
	|	TK_AROMATIC_ATOM
		{
			$$ = new SmartsParser::SPAtom();
			$$->setProperty(SmartsParser::SPAtom::AROMATIC, true);
		}
	|	TK_ALIPHATIC_ATOM
		{
			$$ = new SmartsParser::SPAtom();
			$$->setProperty(SmartsParser::SPAtom::ALIPHATIC, true);
		}
	|	TK_ANY_ATOM
		{
			$$ = new SmartsParser::SPAtom();
		}
	|	atomic_number
		{
			$$ = new SmartsParser::SPAtom($1);
		}
	| TK_ATOM
		{
			$$ = new SmartsParser::SPAtom($1);
		}
	;

implicit_hydrogens:
		TK_HYDROGEN_IMPLICIT
		{
			$$ = 1;
		}
	|	TK_HYDROGEN_IMPLICIT TK_DIGIT
		{
			$$ = $2;
		}
	;

explicit_hydrogens:
		TK_HYDROGEN_EXPLICIT
		{
			$$ = 1;
		}
	|	TK_HYDROGEN_EXPLICIT TK_DIGIT
		{
			$$ = $2;
		}
	;

atomic_number:
		'#' TK_DIGIT 
		{ 
			$$ = $2; 
		}
	|	atomic_number TK_DIGIT 
		{
			$$ = 10 * $1 + $2; 
		}
	;

valence:
		TK_ATOM_VALENCE
		{
			$$ = 0;
		}
	|	TK_ATOM_VALENCE TK_DIGIT
		{
			$$ = $2;
		}
	;
	
connected:
		TK_ATOM_CONNECTED
		{
			$$ = 0;
		}
	|	TK_ATOM_CONNECTED TK_DIGIT
		{
			$$ = $2;
		}
	;

ring_connected:
		TK_ATOM_RING_CONNECTED
		{
			$$ = 0;
		}
	|	TK_ATOM_RING_CONNECTED TK_DIGIT
		{
			$$ = $2;
		}
	;

degree:	
		TK_ATOM_DEGREE
		{
			$$ = 0;
		}
	|	TK_ATOM_DEGREE TK_DIGIT
		{
			$$ = $2;
		}
	;

num_rings:
		TK_ATOM_NUM_RINGS
		{
			$$ = 999;
		}
	|	TK_ATOM_NUM_RINGS TK_DIGIT
		{
			$$ = $2;
		}
	;

ring_size:
		TK_ATOM_RING_SIZE
		{
			$$ = 0;
		}
	|	TK_ATOM_RING_SIZE TK_DIGIT
		{
			$$ = $2;
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

connection_list:
		TK_DIGIT 
		{
			$$ = $1;
		}
	|	connection_list TK_DIGIT
		{
			$$ = $1 + 10 * $2;
		}
	;

isotope:
		TK_DIGIT { $$ = $1;	}
	|	isotope TK_DIGIT { $$ = $1 + 10 * $2; }
	;

chirality:
		'@' /*%prec PREC_CHIRALITY*/
		{
			$$ = new SmartsParser::SPAtom();
			$$->setProperty(SmartsParser::SPAtom::CHIRALITY, SmartsParser::CCW_DEFAULT);
		}
	|	'@' '@' /*%prec PREC_CHIRALITY*/
		{ 
			$$ = new SmartsParser::SPAtom();
			$$->setProperty(SmartsParser::SPAtom::CHIRALITY, SmartsParser::CW_DEFAULT);
		}
	| '@' '?'
		{
			$$ = new SmartsParser::SPAtom();
			$$->setProperty(SmartsParser::SPAtom::CHIRALITY, SmartsParser::CCW_DEFAULT_OR_UNSPECIFIED);
		}
	| '@' '@' '?' 
		{
			$$ = new SmartsParser::SPAtom();
			$$->setProperty(SmartsParser::SPAtom::CHIRALITY, SmartsParser::CW_DEFAULT_OR_UNSPECIFIED);
		}
	;

%%

void yyerror(char const* s)
{
	throw Exception::ParseError(__FILE__, 0, 
															SmartsParser::state.buffer, 
															String(s) + String(" (at position ") 
																+ String(SmartsParser::state.char_count) + String(")"));
}
