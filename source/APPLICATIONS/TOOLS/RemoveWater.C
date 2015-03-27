// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/selector.h>
#include <BALL/STRUCTURE/peptides.h>

#include <BALL/FORMAT/commandlineParser.h>

#include "version.h"

using namespace BALL;

int main(int argc, char* argv[])
{
	// instantiate CommandlineParser object
	CommandlineParser parpars("RemoveWater", "removes water from PDB file ", VERSION, String(__DATE__), "Preparation");
	parpars.registerMandatoryInputFile("i",  "input pdb file ");
	parpars.registerMandatoryOutputFile("o",  "output fasta file");

	// the manual
	String man = String("This tool removes water from a given pdb file.");

	parpars.setToolManual(man);

	parpars.setSupportedFormats("i", "pdb");
	parpars.setSupportedFormats("o", "pdb");

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

	PDBFile out_file;
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

	// define the selector by Expression
	Selector wat("residue(HOH) OR residue(WAT)");
	// apply
	system.apply(wat);

	// remove
	system.removeSelected();
	system.deselect();

	Log << wat.getNumberOfSelectedAtoms() << std::endl;

	out_file << system;
	out_file.close();

	Log << "wrote file " << parpars.get("o") << std::endl;

	return 0;
}
