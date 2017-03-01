// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//



#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/SMILESParserNEW.h>
#include "version.h"

#include <iostream>
#include <string>
#include <vector>

using namespace BALL;
using namespace std;


/*
struct TESTParser : qi::grammar<std::string::const_iterator>
{
		TESTParser() : TESTParser::base_type(start)
		{
			bracket =

			chiral =

			start = "[" >> ( chiral | bracket ) >> "]";
		}

		qi::rule<std::string::const_iterator> r1;
		qi::rule<std::string::const_iterator> start;
};
*/


int main(int argc, char* argv[])
{
	CommandlineParser par("Boost Spirit SMILES Parser", "parse a SMILES string", VERSION, String(__DATE__), "Input");
	String man("Initial attempt to replace flex/bison by Boost's parser library Spirit.");

	par.registerOptionalStringParameter("smi", "SMILES string to parse", "CC");
	par.registerMandatoryOutputFile("o", "SD output filename");
	par.setSupportedFormats("o", "SDF");

	par.setToolManual(man);
	par.parse(argc, argv);

	string smiles = par.get("smi").c_str();
	cout << "INPUT: " << smiles << endl;

	string::const_iterator begin = smiles.begin();
	string::const_iterator end = smiles.end();

	BALL::Molecule m;

	SMILESParser smiles_parser;
	bool success = smiles_parser.parse(smiles, &m);

	/*
	for(unsigned int i=0; i!=m.countAtoms(); ++i)
	{
		cerr << i << "\t" << (m.getAtom(i)->getElement()).getSymbol() << endl;
	}
	*/

	SDFile sdf(par.get("o"), std::ios::out);
	sdf.write(m);
	sdf.close();

	return 0;
}











