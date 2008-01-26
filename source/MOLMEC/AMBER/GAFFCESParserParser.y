%{

#include "GAFFCESParser.h"

#include <iostream>
#include <malloc.h>

using namespace BALL;
using namespace std;

extern int yylex();
extern void yyerror(char* s);

/* There is some nasty problem with alloca under Intel/icc/Linux */
/* so we use (inefficient) malloc instead. */
#undef alloca
#define alloca malloc
%}

%union {
	char*		text;
	int			number;
}


%token						TK_OPEN
%token						TK_END
%token						TK_AND
%token						TK_MATCH_TRUE
%token	<text>		ATOMSTRING
%token	<number>	OPTIONAL_NUMBER

%token						TK_APS_START
%token						TK_APS_END
%token						TK_APS_AND_TERM
%token						TK_APS_OR_TERM
%token						TK_APS_DELOCALIZED
%token					  TK_APS_ONLY_SINGLE_BOND
%token					  TK_APS_ONLY_DOUBLE_BOND
%token					  TK_APS_ONLY_TRIPLE_BOND
%token					  TK_APS_AROMATIC_BOND
%token					  TK_APS_ALL_SINGLE_BONDS
%token					  TK_APS_ALL_DOUBLE_BONDS
%token					  TK_APS_ALL_TRIPLE_BONDS
%token					  TK_APS_NOT_BOUND_TO_PARENT
%token					  TK_APS_BOUND_TO_PARENT
%token					  TK_APS_PURE_AROMATIC_RING
%token					  TK_APS_PLANAR_RING
%token					  TK_APS_PLANAR_RING_WITH_DB
%token					  TK_APS_OTHER_RING_ATOM
%token					  TK_APS_PURE_ALIPHATIC_RING
%token					  TK_APS_THREE_RING
%token					  TK_APS_FOUR_RING
%token					  TK_APS_FIVE_RING
%token					  TK_APS_SIX_RING
%token					  TK_APS_SEVEN_RING
%token					  TK_APS_EIGHT_RING
%token					  TK_APS_NINE_RING
%token					  TK_APS_RING
%token					  TK_APS_NON_RING

%type	<int>				connection

%%

ces_string:	/* empty */ {
		Log.error() << "Warning: Empty String" << std::endl;
	}
	| TK_MATCH_TRUE { GAFFCESParser::state.current_parser->current_root_predicate->addTruePredicate(); }
	| aps open atomlist close {}
	| aps {}
	| open atomlist close {}
  ;

aps: TK_APS_START aps_and_terms TK_APS_END {
		//why Log ERROR???
		Log.error() << "Parsing a new aps string" << std::endl;
	}
	; 

aps_and_terms: aps_or_terms TK_APS_AND_TERM aps_and_terms {
GAFFCESParser::state.current_parser->current_predicate->addNewAND(); }
	| aps_or_terms { }
	;

aps_or_terms: aps_term TK_APS_OR_TERM aps_or_terms { GAFFCESParser::state.current_parser->current_predicate->addNewOr(GAFFCESParser::state->current_aps_type); }
	| aps_term { }
	;

aps_term: TK_APS_DELOCALIZED connection { 
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::DELOCALIZED_BOND; 
						GAFFCESParser::state->current_bond_to_parent = $2;
					}
	|	TK_APS_ONLY_SINGLE_BOND connection {
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::ONLY_SINGLE_BOND;
						GAFFCESParser::state->current_bond_to_parent = $2;
					}
	|	TK_APS_ONLY_DOUBLE_BOND connection {
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::ONLY_DOUBLE_BOND;
						GAFFCESParser::state->current_bond_to_parent = $2;
					}
	|	TK_APS_ONLY_TRIPLE_BOND connection {
						// NOTE: GAFF does not distinguish between different kinds of triple bonds!
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::TRIPLE_BOND;
						GAFFCESParser::state->current_bond_to_parent = $2;
					}
	|	TK_APS_AROMATIC_BOND connection {
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::AROMATIC_BOND;
						GAFFCESParser::state->current_bond_to_parent = $2;
					}
	|	TK_APS_ALL_SINGLE_BONDS connection {
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::SINGLE_BOND;
						GAFFCESParser::state->current_bond_to_parent = $2;
					}
	|	TK_APS_ALL_DOUBLE_BONDS connection {
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::DOUBLE_BOND;
						GAFFCESParser::state->current_bond_to_parent = $2;
					}
	|	TK_APS_ALL_TRIPLE_BONDS connection {
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::TRIPLE_BOND;
						GAFFCESParser::state->current_bond_to_parent = $2;
					}
	|	TK_APS_PURE_AROMATIC_RING {
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::IS_PURELY_AROMATIC;
					}
	|	TK_APS_PLANAR_RING {
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::IS_PLANAR;
					}
	|	TK_APS_PLANAR_RING_WITH_DB {
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::IS_PLANAR_WITH_DB_TO_NR;
					}
	|	TK_APS_OTHER_RING_ATOM {
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::IS_OTHER_RING;
					}
	|	TK_APS_PURE_ALIPHATIC_RING {
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::IS_ALIPHATIC;
					}
	|	TK_APS_THREE_RING {
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::IS_3_RING_ATOM;
					}
	|	TK_APS_FOUR_RING {
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::IS_4_RING_ATOM;
					}
	|	TK_APS_FIVE_RING {
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::IS_5_RING_ATOM;
					}
	|	TK_APS_SIX_RING {
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::IS_6_RING_ATOM;
					}
	|	TK_APS_SEVEN_RING {
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::IS_7_RING_ATOM;
					}
	|	TK_APS_EIGHT_RING {
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::IS_8_RING_ATOM;
					}
	|	TK_APS_NINE_RING {
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::IS_9_RING_ATOM;
					}
	|	TK_APS_RING {
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::IS_RING_ATOM;
					}
	|	TK_APS_NON_RING {
						GAFFCESParser::state->current_aps_type = GAFFCESParser::APSMatcher::IS_NON_RING_ATOM;
					}
	;

connection: /* empty */ { return 0; }
	|	TK_APS_BOUND_TO_PARENT { return 1; }
	| TK_APS_NOT_BOUND_TO_PARENT { return 2; }
	;

open:	TK_OPEN {
		GAFFCESParser::state.current_parser->startChildPredicates();
	};

close:	TK_END {
		GAFFCESParser::state.current_parser->endChildPredicates();
	};	

atomlist:	atomsymbol optional_atomlist	{}
	| atomlist TK_AND atomsymbol optional_atomlist	{};

optional_atomlist: /* empty */	{}
	| open atomlist close	{};

atomsymbol:	ATOMSTRING aps	{
		const set<String>& element_symbols = GAFFCESParser::state.current_parser->getElementSymbols();
		if((element_symbols.find($1)) != element_symbols.end())
		{
			GAFFCESParser::state.current_parser->current_root_predicate->addCESelementPredicate($1);
		}
		else if( ((GAFFCESParser::state.current_parser->current_predicate->getStringToWildcard()).find($1)) != ((GAFFCESParser::state.current_parser->current_predicate->getStringToWildcard()).end()) )
		{	
			GAFFCESParser::state.current_parser->current_root_predicate->addCESwildcardsPredicate($1);
		}
		else
		{
			Atom* atom = GAFFCESParser::state.current_parser->root_predicate->atom_to_test;
			if(GAFFCESParser::state.current_parser->root_predicate->aps_matcher()(*atom))
			{
			}
			else
			{
				Log.error() << "No APS-Match found" << std::endl;
			}
		} 
	}
	|	ATOMSTRING  OPTIONAL_NUMBER	{
		const set<String>& element_symbols = GAFFCESParser::state.current_parser->getElementSymbols();
		if((element_symbols.find($1)) != element_symbols.end())
		{
			GAFFCESParser::state.current_parser->current_root_predicate->addCESelementConnectionPredicate($2, $1);
		}
		else if( ((GAFFCESParser::state.current_parser->current_predicate->getStringToWildcard()).find($1)) != ((GAFFCESParser::state.current_parser->current_predicate->getStringToWildcard()).end()) )
		{	
			GAFFCESParser::state.current_parser->current_root_predicate->addCESwildcardsConnectionPredicate($1, $2);
		}
		else
		{
			Log.error() << "No valid Atomsymbol found" << std::endl;
		}
	};

%%
void yyerror(char* s)
{
	printf("Parse Error! %s\n", s);
}


