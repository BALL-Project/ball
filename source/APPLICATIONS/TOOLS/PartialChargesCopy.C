// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/KERNEL/molecule.h>
#include "version.h"

using namespace BALL;
using namespace std;

int main(int argc, char* argv[])
{
	CommandlineParser par("PartialChargesCopy", "transfer part. charges between files", VERSION, String(__DATE__), "Preparation");
	par.registerMandatoryInputFile("i", "input molecule file");
	par.registerMandatoryInputFile("chr", "file containing the same molecules as the input file, but with (different) partial charges");
	par.registerMandatoryOutputFile("o", "output file");
	String man = "This tool copies partial charges from a given file to the conformations read from a different file.\nThis can be useful when computing partial charges with external tools, most of which write output as mol2-files *without* support for storing molecular properties. By use of this tool we can thus assign the computed partial charges to the original molecules, thus retaining all properties.";
	par.setToolManual(man);
	par.setSupportedFormats("i",MolFileFactory::getSupportedFormats());
	par.setSupportedFormats("chr","mol2,drf");
	par.setSupportedFormats("o","mol2,drf");
	par.parse(argc, argv);

	GenericMolFile* input = MolFileFactory::open(par.get("i"));
	GenericMolFile* charges = MolFileFactory::open(par.get("chr"));
	GenericMolFile* output = MolFileFactory::open(par.get("o"), ios::out, input);

	int mol_no = 0;
	Molecule* charged_mol = charges->read();
	Molecule* mol = input->read();
	for (; mol && charged_mol; mol = input->read(), charged_mol = charges->read())
	{
		AtomIterator it1 = mol->beginAtom();
		AtomIterator it2 = charged_mol->beginAtom();
		for (; +it1 && +it2; it1++, it2++)
		{
			it1->setCharge(it2->getCharge());
		}
		if (+it1 || +it2)
		{
			cerr << "Error: unequal number of atoms !!"<<endl;
			exit(1);
		}

		*output << *mol;

		mol_no++;
		delete mol;
	}
	if (mol || charged_mol)
	{
		cerr <<"Error: unequal number of molecules !!"<<endl;
		exit(1);
	}

	Log.level(20)<<"\rwrote "<<mol_no<<" molecules."<<endl;

	delete input;
	delete output;
	delete charges;
}
