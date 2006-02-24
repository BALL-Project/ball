%{

#include <BALL/STRUCTURE/smartsParser.h>
//#include <iostream>
#include <BALL/KERNEL/PTE.h>

using namespace BALL;
using namespace std;

extern int yylex();
extern void yyerror(char* s);

/* ??? */
/* There is some nasty problem with alloca under Intel/icc/Linux */
/* so we use (inefficient) malloc instead. */
#undef alloca
#define alloca malloc

#define YYDEBUG 1

%}
%union {
	char*														text;
	Index														number;
	SmartsParser::SPAtom*						atom;
	SmartsParser::SPNode*						node;
	SmartsParser::SPEdge*						edge;
	/*SmartsParser::ChiralDef*				chiral;*/
	SmartsParser::SPBond*						sp_bond;
	/*SmartsParser::LogicalOperator		logical;*/
	NamedProperty*									property;
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

//%type		<logical>	log_op

%left ';' 
%left ',' 
%left '&'
//%left '+' PREC_NEG_CHARGE
//%left PREC_CHIRALITY
//%left '-' '@'
//%left TK_DIGIT

//%right '+' PREC_NEG_CHARGE

//%left '!'

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
			SmartsParser::SPBond* b = new SmartsParser::SPBond(SmartsParser::SPBond::SINGLE);
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
			SmartsParser::SPBond* b = new SmartsParser::SPBond(SmartsParser::SPBond::SINGLE);
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
			$$ = e;
		}
	|	'!' ring_bond
		{
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setSPBond($2);
			e->setNot(true);
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
			e->setLogicalOperator(SmartsParser::OR);
			$$ = e;
		}
/*	|	sp_edge '!' ring_bond
		{
			SmartsParser::SPEdge* e1 = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e1->setSPBond($3);
			e1->setNot(true);
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setInternal(true);
			e->setFirstSPEdge($1);
			e->setSecondSPEdge(e1);
			e->setLogicalOperator(SmartsParser::AND);
			$$ = e;
		}*/
/*	|	sp_edge ring_bond
		{
			SmartsParser::SPEdge* e1 = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e1->setSPBond($2);
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setInternal(true);
			e->setFirstSPEdge($1);
			e->setSecondSPEdge(e1);
			e->setLogicalOperator(SmartsParser::AND);
			$$ = e;
		}*/
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
			$2->setInBrackets();
			SmartsParser::state.current_parser->addRingConnection($2, $4);
			$$ = $2;
		}
	| '[' node ']'
		{
			$2->setInBrackets();
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
/*	| node ';' atom
		{
			SmartsParser::SPNode* n = new SmartsParser::SPNode($3);
			$$ = new SmartsParser::SPNode($1, SmartsParser::AND_LOW, n);
		}
	|	node ',' atom
		{
			SmartsParser::SPNode* n = new SmartsParser::SPNode($3);
			$$ = new SmartsParser::SPNode($1, SmartsParser::OR, n);
		}
	|	node '&' atom
		{
			SmartsParser::SPNode* n = new SmartsParser::SPNode($3);
			$$ = new SmartsParser::SPNode($1, SmartsParser::AND, n);
		}
	|	node ';' unbraced_atom
		{
			SmartsParser::SPNode* n = new SmartsParser::SPNode($3);
			$$ = new SmartsParser::SPNode($1, SmartsParser::AND_LOW, n);
		}
	|	node ',' unbraced_atom
		{
			SmartsParser::SPNode* n = new SmartsParser::SPNode($3);
			$$ = new SmartsParser::SPNode($1, SmartsParser::OR, n);
		}
	|	node '&' unbraced_atom
		{
			SmartsParser::SPNode* n = new SmartsParser::SPNode($3);
			$$ = new SmartsParser::SPNode($1, SmartsParser::AND, n);
		}
	|	node ';' rec_expression
		{
			$$ = new SmartsParser::SPNode($1, SmartsParser::AND_LOW, $3);
		}
	|	node ',' rec_expression
		{
			$$ = new SmartsParser::SPNode($1, SmartsParser::OR, $3);
		}
	|	node '&' rec_expression
		{
			$$ = new SmartsParser::SPNode($1, SmartsParser::AND, $3);
		}*/
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

/*
log_op:
		'&'
		{
			$$ = SmartsParser::AND;
		}
	|	';'
		{
			$$ = SmartsParser::AND_LOW;
		}
	|	','
		{
			$$ = SmartsParser::OR;
		}
	;
*/
unbraced_atom:
		'!' unbraced_atom_symbol
		{
			$2->addAtomProperty(NamedProperty("Not", true));
			$$ = $2;
		}
	|	unbraced_atom_symbol
		{
			$$ = $1;
		}
	|	'!' atom_property
		{
			$$ = new SmartsParser::SPAtom();
			$$->addAtomProperty(NamedProperty("Not"+$2->getName(), $2->getUnsignedInt()));
		}
	|	atom_property 
		{
			$$ = new SmartsParser::SPAtom();
			$$->addAtomProperty(*$1);
		}
	| unbraced_atom atom_property
		{
			$1->addAtomProperty(*$2);
			$$ = $1;
		}
	| unbraced_atom '!' atom_property
		{
			$1->addAtomProperty(NamedProperty("Not"+$3->getName(), $3->getUnsignedInt()));
			$$ = $1;
		}
	|	unbraced_atom '!' pos_charge
		{
			$1->addAtomProperty(NamedProperty("NotCharge", Index($3)));
			$$ = $1;
		}
	|	unbraced_atom pos_charge
		{
			$1->addAtomProperty(NamedProperty("Charge", Index($2)));
			$$ = $1;
		}
	| '!' pos_charge
		{
			$$ = new SmartsParser::SPAtom("*");
			$$->addAtomProperty(NamedProperty("NotCharge", Index($2)));
		}
	|	pos_charge
		{
			$$ = new SmartsParser::SPAtom("*");
			$$->addAtomProperty(NamedProperty("Charge", Index($1)));
		}
	;

atom:
		atom_symbol
		{
			$$ = $1;
		}
	|	'!' atom_symbol
		{
			$2->addAtomProperty(NamedProperty("Not", true));
			$$ = $2;
		}
	|	'!' neg_charge
		{
			$$ = new SmartsParser::SPAtom("*");
			$$->addAtomProperty(NamedProperty("NotCharge", Index($2)));
		}
	|	neg_charge
		{
			$$ = new SmartsParser::SPAtom("*");
			$$->addAtomProperty(NamedProperty("Charge", Index($1)));
		}
	|	'!' chirality
		{
			//$$ = new SmartsParser::SPAtom("*");
			$$ = $2;
			//$$->setChirality(*$2);
			$$->addAtomProperty(NamedProperty("NotChirality", true));
		}
	|	chirality
		{
			//$$ = new SmartsParser::SPAtom("*");
			$$ = $1;
			//$$->setChirality(*$1);
		}
	| unbraced_atom '!' neg_charge
		{
			$1->addAtomProperty(NamedProperty("NotCharge", Index($3)));
			$$ = $1;
		}
	|	unbraced_atom neg_charge
		{
			$1->addAtomProperty(NamedProperty("Charge", Index($2)));
			$$ = $1;
		}
	| unbraced_atom '!' chirality
		{
			$1->addAtomProperty(NamedProperty("NotChirality", true));
			//$1->setChirality(*$3);
			for (Size i = 0; i != $3->countProperties(); ++i)
			{
				$1->addAtomProperty($3->getNamedProperty(i));
			}
			delete $3;
			$$ = $1;
		}
	| unbraced_atom chirality
		{
			//$1->setChirality(*$2);
			for (Size i = 0; i != $2->countProperties(); ++i)
			{
				$1->addAtomProperty($2->getNamedProperty(i));
			}
			delete $2;
			$$ = $1;
		}
	|	atom atom_property
		{
			$1->addAtomProperty(*$2);
			$$ = $1;
		}
	|	atom '!' neg_charge
		{
			$1->addAtomProperty(NamedProperty("NotCharge", Index($3)));
			$$ = $1;
		}
	|	atom neg_charge
		{
			$1->addAtomProperty(NamedProperty("Charge", Index($2)));
			$$ = $1;
		}
	|	atom '!' pos_charge
		{
			$1->addAtomProperty(NamedProperty("NotCharge", Index($3)));
			$$ = $1;
		}
	|	atom pos_charge
		{
			$1->addAtomProperty(NamedProperty("Charge", Index($2)));
			$$ = $1;
		}
	|	atom '!' chirality
		{
			//$1->setChirality(*$3);
			$1->addAtomProperty(NamedProperty("NotChirality", true));
			for (Size i = 0; i != $3->countProperties(); ++i)
			{
				$1->addAtomProperty($3->getNamedProperty(i));
			}
			delete $3;
			$$ = $1;
		}
	|	atom chirality
		{
			//$1->setChirality(*$2);
			for (Size i = 0; i != $2->countProperties(); ++i)
			{
				$1->addAtomProperty($2->getNamedProperty(i));
			}
			delete $2;
			$$ = $1;
		}
	|	atom '!' atom_property
		{
			$1->addAtomProperty(NamedProperty("Not"+$3->getName(), $3->getUnsignedInt()));
			delete $3;
			$$ = $1;
		}
	;

atom_property:
		num_rings
		{
			$$ = new NamedProperty("InNumRings", Size($1));
			SmartsParser::state.current_parser->setNeedsSSSR(true);
		}
	|	ring_size
		{
			$$ = new NamedProperty("InRingSize", Size($1));
		}
	|	valence
		{
			$$ = new NamedProperty("Valence", Size($1));
		}
	|	connected
		{
			$$ = new NamedProperty("Connectivity", Size($1));
		}
	|	implicit_hydrogens
		{
			$$ = new NamedProperty("ImplicitHydrogens", Size($1));
		}
	|	explicit_hydrogens
		{
			$$ = new NamedProperty("ExplicitHydrogens", Size($1));
		}
	|	degree
		{
			$$ = new NamedProperty("Degree", Size($1));
		}
	|	ring_connected
		{
			$$ = new NamedProperty("RingConnected", Size($1));
		}
	;

atom_symbol:
		isotope
		{
			$$ = new SmartsParser::SPAtom("*");
			$$->addAtomProperty(NamedProperty("Isotope", Size($1)));
		}
	|	isotope TK_ORG_SUBSET_ATOM
		{
			$$ = new SmartsParser::SPAtom($2);
			$$->addAtomProperty(NamedProperty("Isotope", Size($1)));
		}
	|	isotope TK_ATOM
		{
			$$ = new SmartsParser::SPAtom($2);
			$$->addAtomProperty(NamedProperty("Isotope", Size($1)));
		}
	|	isotope TK_AROMATIC_ATOM
		{
			$$ = new SmartsParser::SPAtom($2);
			$$->addAtomProperty(NamedProperty("Isotope", Size($1)));
		}
	| isotope TK_ALIPHATIC_ATOM
		{
			$$ = new SmartsParser::SPAtom($2);
			$$->addAtomProperty(NamedProperty("Isotope", Size($1)));
		}
	|	isotope TK_ANY_ATOM
		{
			$$ = new SmartsParser::SPAtom($2);
			$$->addAtomProperty(NamedProperty("Isotope", Size($1)));
		}
	|	isotope	atomic_number
		{
			$$ = new SmartsParser::SPAtom($2);
			$$->addAtomProperty(NamedProperty("Isotope", Size($1)));
		}
	;

unbraced_atom_symbol:
		TK_ORG_SUBSET_ATOM
		{
			$$ = new SmartsParser::SPAtom($1);
		}
	|	TK_AROMATIC_ATOM
		{
			$$ = new SmartsParser::SPAtom("*");
			$$->addAtomProperty(NamedProperty("Aromatic", true));
		}
	|	TK_ALIPHATIC_ATOM
		{
			$$ = new SmartsParser::SPAtom("*");
			$$->addAtomProperty(NamedProperty("Aliphatic", true));
		}
	|	TK_ANY_ATOM
		{
			$$ = new SmartsParser::SPAtom("*");
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
			$$->addAtomProperty(NamedProperty("Chirality", SmartsParser::CCW_DEFAULT));
		}
	|	'@' '@' /*%prec PREC_CHIRALITY*/
		{ 
			$$ = new SmartsParser::SPAtom();
			$$->addAtomProperty(NamedProperty("Chirality", SmartsParser::CW_DEFAULT));
		}
	| '@' '?'
		{
			$$ = new SmartsParser::SPAtom();
			$$->addAtomProperty(NamedProperty("Chirality", SmartsParser::CCW_DEFAULT_OR_UNSPECIFIED));
		}
	| '@' '@' '?' 
		{
			$$ = new SmartsParser::SPAtom();
			$$->addAtomProperty(NamedProperty("Chirality", SmartsParser::CW_DEFAULT_OR_UNSPECIFIED));
		}
/*	|	TK_CHIRAL_CLASS_TH TK_ONE_TWO 
		{
			$$ = new SmartsParser::ChiralDef(SmartsParser::TH, $2); 
		}
	|	TK_CHIRAL_CLASS_AL TK_ONE_TWO 
		{
			$$ = new SmartsParser::ChiralDef(SmartsParser::AL, $2); 
		}
	|	TK_CHIRAL_CLASS_TB TK_DIGIT
		{
			$$ = new SmartsParser::ChiralDef(SmartsParser::TB, $2); 
		}
	|	TK_CHIRAL_CLASS_SP TK_DIGIT
		{
			$$ = new SmartsParser::ChiralDef(SmartsParser::SP, $2); 
		}
	|	TK_CHIRAL_CLASS_OH TK_DIGIT
		{
			$$ = new SmartsParser::ChiralDef(SmartsParser::OH, $2); 
		}
	| chirality '?' 
		{
			// TODO correct class
			$$ = $1;
		}*/
	;

%%

void yyerror(char* s)
{
	throw Exception::ParseError(__FILE__, 0, 
															SmartsParser::state.buffer, 
															String(s) + String(" (at position ") 
																+ String(SmartsParser::state.char_count) + String(")"));
}


