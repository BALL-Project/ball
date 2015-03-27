// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/MOLMEC/COMMON/assignTypes.h>
#include <BALL/KERNEL/selector.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/STRUCTURE/fragmentDB.h>

#include "version.h"

using namespace BALL;
using namespace std;

int main(int argc, char* argv[])
{
	CommandlineParser parpars("AddMissingAtoms", "add missing atoms to protein structures", VERSION, String(__DATE__), "Preparation");
	parpars.registerMandatoryInputFile("i", "input file");
	parpars.registerMandatoryOutputFile("o", "output file");

	parpars.setSupportedFormats("i","pdb");
	parpars.setSupportedFormats("o","pdb,drf");

	parpars.registerFlag("opt_hyd", "optimize the positions of hydrogens", false);

	String manual = "AddMissingAtoms tool allows you to missing atoms, i.e. hydrogens to a protein structure.\n\n Optional parameter is the optimize_hydrogens flag (-opt_hyd), which uses the AMBER force field to run a quick energy minization for all hydrogen atoms.\n\nOutput of this tool is one pdb-file containing the input protein structure with added atoms.";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);

	PDBFile input(parpars.get("i"), ios::in);
	//GenericMolFile* input = MolFileFactory::open(parpars.get("i"), ios::in);
	//PDBFile* pdb_input = dynamic_cast<PDBFile*>(input);
	if (!input)
	{
		Log.error() << "Cannot open PDB input file " << parpars.get("i") << endl;
		return 2;
	}

	System S;
	input >> S;
	input.close();

	FragmentDB fragdb("");

	Log.disableOutput();
	S.apply(fragdb.normalize_names);
	S.apply(fragdb.add_hydrogens);
	S.apply(fragdb.build_bonds);
	Log.enableOutput();

	if (parpars.has("opt_hyd") && parpars.get("opt_hyd").toBool())
	{
		AmberFF FF;

		// we then select all hydrogens (element(H))
		// using a specialized processor (Selector)
		S.deselect();
		FF.setup(S);
		Selector selector("element(H)");
		S.apply(selector);

		// now we create a minimizer object that uses a conjugate 
		// gradient algorithm to optimize the atom positions
		ConjugateGradientMinimizer minimizer;

		// calculate the total energy of the system
		float initial_energy = FF.updateEnergy();
		Log << "initial energy: " << initial_energy << " kJ/mol" << endl;

		// initialize the minimizer and perform (up to)
		// 1000 optimization steps
		minimizer.setup(FF);
		minimizer.setEnergyOutputFrequency(1);
		minimizer.minimize(50);

		// calculate the terminal energy and print it
		float terminal_energy = FF.getEnergy();

		Log << "energy before/after minimization: " << initial_energy << "/" << terminal_energy << " kJ/mol" << endl;
	}

	// write the optimized structure to a file whose
	// name is given as the second command line argument
	String outfile_name = String(parpars.get("o"));
	PDBFile outfile(outfile_name, ios::out);
	outfile << S;
	outfile.close();

	Log << "wrote file " << outfile_name << endl;
}


