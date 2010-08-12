%{

#include <BALL/FORMAT/GAMESSLogFile.h>

#include <iostream>
#include <malloc.h>

using namespace BALL;
using namespace std;

extern int yylex();
extern void yyerror(char const* s);

%}

%union {
	char*		text;
	float		value;
	int			number;
}

%token	<text>	 TK_LINE
%token	<text>	 TK_OPTION
%token	<text>	 TK_EIGEN_LINE
%token	<text>	 TK_ATOM
%token	<value>	 TK_FLOAT
%token	<text> 	 TK_BOND
%token	<text> 	 TK_OTHER
%token	<text> 	 TK_CHARGE
%token	<text> 	 TK_POPULATION
%token	<text> 	 TK_COEFFICIENTS
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
%token					 TK_EIGEN_START
%token					 TK_EIGEN_END
%token					 TK_BASIS_OPTIONS_START
%token					 TK_BASIS_OPTIONS_END
%token					 TK_BOND_START
%token					 TK_BOND_END

%%

gamessfile:	/* empty */
		| gamessfile molecule	  	 {}
		| gamessfile geometry			 {}
		| gamessfile zmat			  	 {}
		|	gamessfile basis_set		 {}
		| gamessfile bond 		  	 {}
		| gamessfile charges    	 {}
		| gamessfile population 	 {}
		| gamessfile eigenvectors  {}
		| gamessfile TK_LINE			 {
				String s($2);
				s.toUpper();
				if (s.hasSubstring("ALPHA"))
					GAMESSLogFile::state.current_parser->current_set = 1;
				else
					GAMESSLogFile::state.current_parser->current_set = 2;
			}
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

bond:	start_bond bonds TK_BOND_END {};

start_bond: TK_BOND_START {
	GAMESSLogFile::state.current_parser->clearBonds();
}

bonds: /* empty */
	| bonds TK_LINE {}
	| bonds TK_BOND {
			String line($2);
			std::vector<String> fields;

			line.split(fields);
			/** TODO: is this a sensible threshold?? **/
			if (fields[3].toFloat() > 0.3)
				GAMESSLogFile::state.current_parser->insertBond(fields[0].toInt()-1, fields[1].toInt()-1);

			free($2);
		}
	;

basis_set: TK_BASIS_OPTIONS_START basis_options TK_BASIS_OPTIONS_END {};

basis_options: /* empty */
	| basis_options	TK_OPTION {
		 /** Some of this stuff should really be done in flex. But if we add those
			*  rules, flex gets terribly slow... **/
		 String line($2);
		 std::vector<String> fields;
 		 line.split(fields, "=");

		 GAMESSLogFile::state.current_parser->addBasisOption(fields[0], fields[1]);
		 free($2);
		}
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

eigenvectors:	TK_EIGEN_START eigendata TK_EIGEN_END {};

eigendata: /* empty */
	| eigendata  TK_LINE {}
	| eigendata	 TK_COEFFICIENTS {
			String s($2);
			std::vector<String> fields;
			s.split(fields);

			GAMESSLogFile::state.current_parser->current_coefficient_line += fields.size();
			for (Size i=0; i<fields.size(); i++)
			{
				GAMESSLogFile::state.current_parser->addCoefficient(fields[i].toFloat());
			}
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
