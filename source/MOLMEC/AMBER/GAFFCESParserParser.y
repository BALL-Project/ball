%{

#include <BALL/MOLMEC/AMBER/GAFFCESParser.h>

#include <iostream>
#include <cstdlib>

using namespace BALL;
using namespace std;

extern int yylex();
extern void yyerror(const char* s);

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
%token					  TK_APS_PURE_SINGLE_BOND
%token					  TK_APS_PURE_DOUBLE_BOND
%token					  TK_APS_TRIPLE_BOND
%token					  TK_APS_AROMATIC_BOND
%token					  TK_APS_ALL_SINGLE_BONDS
%token					  TK_APS_ALL_DOUBLE_BONDS
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
%token					  TK_APS_TRUE
%token<number>  	TK_APS_NUMBER

%type	<number>				connection

%%

ces_string:	/* empty */ {
		Log.error() << "Warning: Empty String" << std::endl;
	}
	| ces_part {}
	| aps ces_part {}
	| aps {}
  ;

ces_part: open atomlist close {}
	| TK_MATCH_TRUE { 
		GAFFCESParser::state.current_parser->current_root_predicate->addTruePredicate(); 
	};

aps: TK_APS_START aps_and_terms TK_APS_END { }
	; 

aps_and_terms: aps_or_terms aps_and aps_and_terms {
	}
	| aps_or_terms { }
	;

aps_and: TK_APS_AND_TERM {
		 GAFFCESParser::state.current_parser->current_predicate->addNewAND(); 
	};

aps_or_terms: aps_or_term TK_APS_OR_TERM aps_or_terms { }
	| aps_or_term { }
	;

aps_or_term: aps_term { 
		GAFFCESParser::state.current_parser->current_predicate->addNewOR(GAFFCESParser::state.current_aps_type, GAFFCESParser::state.feature_number); 
	};


aps_term: TK_APS_TRUE {
		GAFFCESParser::state.current_aps_type = (GAFFCESParser::APSMatcher::APSType) (GAFFCESParser::APSMatcher::APS_TRUE);
	}
	| optional_aps_number TK_APS_DELOCALIZED connection { 
		GAFFCESParser::state.current_aps_type = (GAFFCESParser::APSMatcher::APSType) (GAFFCESParser::APSMatcher::DELOCALIZED_BOND + $3);
	}
	|	optional_aps_number TK_APS_PURE_SINGLE_BOND connection {
		GAFFCESParser::state.current_aps_type = (GAFFCESParser::APSMatcher::APSType) (GAFFCESParser::APSMatcher::PURE_SINGLE_BOND + $3);
	}
	|	optional_aps_number TK_APS_PURE_DOUBLE_BOND connection {
		GAFFCESParser::state.current_aps_type = (GAFFCESParser::APSMatcher::APSType) (GAFFCESParser::APSMatcher::PURE_DOUBLE_BOND + $3);
	}
	|	optional_aps_number TK_APS_TRIPLE_BOND connection {
		// NOTE: GAFF does not distinguish between different kinds of triple bonds!
		GAFFCESParser::state.current_aps_type = (GAFFCESParser::APSMatcher::APSType) (GAFFCESParser::APSMatcher::TRIPLE_BOND + $3);
	}
	|	optional_aps_number TK_APS_AROMATIC_BOND connection {
		GAFFCESParser::state.current_aps_type = (GAFFCESParser::APSMatcher::APSType) (GAFFCESParser::APSMatcher::AROMATIC_BOND + $3);
	}
	|	optional_aps_number TK_APS_ALL_SINGLE_BONDS connection {
		GAFFCESParser::state.current_aps_type = (GAFFCESParser::APSMatcher::APSType) (GAFFCESParser::APSMatcher::SINGLE_BOND + $3);
	}
	|	optional_aps_number TK_APS_ALL_DOUBLE_BONDS connection {
		GAFFCESParser::state.current_aps_type = (GAFFCESParser::APSMatcher::APSType) (GAFFCESParser::APSMatcher::DOUBLE_BOND + $3);
	}
	|	optional_aps_number TK_APS_PURE_AROMATIC_RING {
		GAFFCESParser::state.current_aps_type = GAFFCESParser::APSMatcher::IS_PURELY_AROMATIC;
	}
	|	optional_aps_number TK_APS_PLANAR_RING {
		GAFFCESParser::state.current_aps_type = GAFFCESParser::APSMatcher::IS_PLANAR;
	}
	|	optional_aps_number TK_APS_PLANAR_RING_WITH_DB {
		GAFFCESParser::state.current_aps_type = GAFFCESParser::APSMatcher::IS_PLANAR_WITH_DB_TO_NR;
	}
	|	optional_aps_number TK_APS_OTHER_RING_ATOM {
		GAFFCESParser::state.current_aps_type = GAFFCESParser::APSMatcher::IS_OTHER_RING;
	}
	|	optional_aps_number TK_APS_PURE_ALIPHATIC_RING {
		GAFFCESParser::state.current_aps_type = GAFFCESParser::APSMatcher::IS_PURELY_ALIPHATIC;
	}
	|	optional_aps_number TK_APS_THREE_RING {
		GAFFCESParser::state.current_aps_type = GAFFCESParser::APSMatcher::IS_3_RING_ATOM;
	}
	|	optional_aps_number TK_APS_FOUR_RING {
		GAFFCESParser::state.current_aps_type = GAFFCESParser::APSMatcher::IS_4_RING_ATOM;
	}
	|	optional_aps_number TK_APS_FIVE_RING {
		GAFFCESParser::state.current_aps_type = GAFFCESParser::APSMatcher::IS_5_RING_ATOM;
	}
	|	optional_aps_number TK_APS_SIX_RING {
		GAFFCESParser::state.current_aps_type = GAFFCESParser::APSMatcher::IS_6_RING_ATOM;
	}
	|	optional_aps_number TK_APS_SEVEN_RING {
		GAFFCESParser::state.current_aps_type = GAFFCESParser::APSMatcher::IS_7_RING_ATOM;
	}
	|	optional_aps_number TK_APS_EIGHT_RING {
		GAFFCESParser::state.current_aps_type = GAFFCESParser::APSMatcher::IS_8_RING_ATOM;
	}
	|	optional_aps_number TK_APS_NINE_RING {
		GAFFCESParser::state.current_aps_type = GAFFCESParser::APSMatcher::IS_9_RING_ATOM;
	}
	|	optional_aps_number TK_APS_RING {
		GAFFCESParser::state.current_aps_type = GAFFCESParser::APSMatcher::IS_RING_ATOM;
	}
	|	optional_aps_number TK_APS_NON_RING {
		GAFFCESParser::state.current_aps_type = GAFFCESParser::APSMatcher::IS_NON_RING_ATOM;
	}
	;

optional_aps_number: /* empty */ { GAFFCESParser::state.feature_number = -1; }
	| TK_APS_NUMBER {  GAFFCESParser::state.feature_number = $1; }
	;

connection: /* empty */ {  $$ = 0; }
	|	TK_APS_BOUND_TO_PARENT {  $$ = 1; }
	| TK_APS_NOT_BOUND_TO_PARENT { $$ = 2; }
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

atomsymbol: atom_definition { }
	| atom_definition aps { };
	
atom_definition:	ATOMSTRING {
		const set<String>& element_symbols = GAFFCESParser::state.current_parser->getElementSymbols();
		if((element_symbols.find($1)) != element_symbols.end())
		{
			GAFFCESParser::state.current_parser->current_root_predicate->addCESelementPredicate($1);
		}
		else if( ((GAFFCESParser::state.current_parser->current_predicate->getStringToWildcard()).find($1)) != ((GAFFCESParser::state.current_parser->current_predicate->getStringToWildcard()).end()) )
		{	
			GAFFCESParser::state.current_parser->current_root_predicate->addCESwildcardsPredicate($1);
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
void yyerror(const char* s)
{
	printf("Parse Error! %s\n", s);
}
