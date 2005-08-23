%{

#include <BALL/FORMAT/GAMESSLogFile.h>

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
	float		value;
}

%token	<text>	 TK_LINE
%token	<text>	 TK_ATOM
%token	<value>	 TK_FLOAT
%token	<text> 	 TK_BOND
%token	<text> 	 TK_OTHER
%token	<text> 	 TK_CHARGE
%token	<text> 	 TK_POPULATION
%token					 TK_TITLE
%token					 TK_COORDS_BOHR
%token					 TK_COORDS_ANGSTROM
%token					 TK_END_MOL
%token					 TK_ZMAT_START
%token					 TK_ZMAT_END
%token					 TK_CHARGE_START
%token					 TK_CHARGE_END
%token					 TK_POPULATION_START
%token					 TK_POPULATION_END

%%

gamessfile:	/* empty */
		|	gamessfile TK_LINE    {}
		| gamessfile molecule	  {}
		| gamessfile geometry		{}
		| gamessfile zmat			  {}
		| gamessfile charges    {}
		| gamessfile population {}
		;

//non_blocked: 
//		  non_blocked	TK_LINE	{printf("1 %s", $2);}
//		| TK_LINE {printf("2 %s\n", $1);}
//		;

molecule: molecule_header {};

molecule_header:
	TK_TITLE TK_LINE TK_LINE	{
		GAMESSLogFile::state.current_parser->newMolecule();
		GAMESSLogFile::state.current_parser->setMoleculeName($2);
	};

geometry: unit coordinates TK_END_MOL {
	GAMESSLogFile::state.current_parser->molecule_already_defined = true;
};

unit: 
	  TK_COORDS_BOHR {
			GAMESSLogFile::state.current_parser->current_atom = 0;
			GAMESSLogFile::state.current_parser->setUnitConversionFactor(0.529177);
		}
	|	TK_COORDS_ANGSTROM {
			GAMESSLogFile::state.current_parser->current_atom = 0;
			GAMESSLogFile::state.current_parser->setUnitConversionFactor(1.0);
		}
	;

coordinates: /* empty */
 	| coordinates TK_ATOM
		{
			String line($2);
			
			GAMESSLogFile::state.current_parser->insertAtom(line.getField(0).c_str(),
																											0.,
																											line.getField(2).toFloat(),
																											line.getField(3).toFloat(),
																											line.getField(4).toFloat());
		}
	;

zmat:	TK_ZMAT_START zmat_data TK_ZMAT_END	{};

zmat_data: /* empty */
	|	zmat_data TK_BOND	{
			String s($2);
			GAMESSLogFile::state.current_parser->insertBond(s.getField(2).toInt()-1, s.getField(3).toInt()-1);
		}
	| zmat_data TK_OTHER {}
	;

charges: charges_header charges_data TK_CHARGE_END {};

charges_header:	TK_CHARGE_START	{
	GAMESSLogFile::state.current_parser->current_atom = 0;
};

charges_data:	/* empty */
	| charges_data TK_CHARGE {
		/** Do we always want to overwrite those? **/
		String s($2);

		GAMESSLogFile::state.current_parser->setCurrentCharge(s.getField(1).toFloat());
		GAMESSLogFile::state.current_parser->current_atom++;
	}
	;

population:	TK_POPULATION_START population_data TK_POPULATION_END	{};

population_data: /* empty */
	|	population_data TK_POPULATION {
			/** Do we really always want this? This will always set the mulliken charges... **/
			String line($2);

			GAMESSLogFile::state.current_parser->current_atom = line.getField(0).toInt()-1;
			GAMESSLogFile::state.current_parser->setCurrentCharge(line.getField(3).toFloat());
		}
	;

%%
void yyerror(char* s)
{
/**	throw Exception::ParseError(__FILE__, 0, 
															SmilesParser::state.buffer, 
															String(s) + String(" (at position ") 
																+ String(SmilesParser::state.char_count) + String(")"));
*/
	printf("Parse Error! %s\n", s);
}
