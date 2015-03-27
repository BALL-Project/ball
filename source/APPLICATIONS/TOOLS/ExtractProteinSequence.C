// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/KERNEL/system.h>
#include <BALL/STRUCTURE/peptides.h>

#include "version.h"

using namespace BALL;

int main(int argc, char* argv[])
{
	// instantiate CommandlineParser object
	CommandlineParser parpars("ExtractProteinSequence", "extracts fasta sequence", VERSION, String(__DATE__), "Get Data");
	parpars.registerMandatoryInputFile("i",  "input pdb file from which to extract ");
	parpars.registerMandatoryOutputFile("o",  "output fasta file");
	parpars.registerOptionalStringParameter("c",  "chain specifier");

	// the manual
	String man = String("This tool extracts the fasta sequence from a given pdb file.");

	parpars.setToolManual(man);

	parpars.setSupportedFormats("i", "pdb");
	parpars.setSupportedFormats("o", "fasta");

	// parse the command line
	parpars.parse(argc, argv);

	PDBFile in_file;
	in_file.open(parpars.get("i"), std::ios::in);

	if (!in_file)
	{
		// if file does not exist: complain and abort
		Log.error() << "error opening " << parpars.get("i") << " for input." << std::endl;
		exit(2);
	}

	//TODO switch to the BALL FastaFile format if available
	LineBasedFile out_file;
	out_file.open(parpars.get("o"), std::ios::out);

	if (!out_file)
	{
		// if file does not exist: complain and abort
		Log.error() << "error opening " << parpars.get("o") << " for output." << std::endl;
		exit(2);
	}

	System system;
	in_file >> system;
	in_file.close();

	for (ProteinIterator it = system.beginProtein(); +it; ++it)
	{
		ChainConstIterator c_it = it->beginChain();
		for (; +c_it ; ++c_it)
		{
			// get the chain's sequence
			String seq = Peptides::GetSequence(*c_it);
			out_file << "> " << it->getName() + ":" + c_it->getName() << std::endl << seq << std::endl;
		}
	}

	out_file.close();
	Log << "wrote file " << parpars.get("o") << std::endl;

	return 0;

}
