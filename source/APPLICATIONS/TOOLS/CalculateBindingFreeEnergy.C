// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/ENERGY/atomicContactEnergy.h>
#include <BALL/ENERGY/coulomb.h>
#include <BALL/FORMAT/PDBFile.h>

#include <BALL/FORMAT/commandlineParser.h>

#include "version.h"

using namespace std;
using namespace BALL;

int main(int argc, char* argv[])
{
	// instantiate CommandlineParser object
	CommandlineParser parpars("CalculateBindingFreeEnergy", "calculate binding energy of two proteins using AMBER", VERSION, String(__DATE__), "ForceFields");
	parpars.registerMandatoryInputFile("pdb_a",  "first input pdb file ");
	parpars.registerMandatoryInputFile("pdb_b",  "second input pdb file ");
	///TODO: generate an output file with the energy that is being calculated!!! stdout is not enough!!!

	// the manual
	String man = String("This tool computes the binding energy of two given pdb files using the AMBER force field.");

	parpars.setToolManual(man);

	parpars.setSupportedFormats("pdb_a", "pdb");
	parpars.setSupportedFormats("pdb_b", "pdb");

	// parse the command line
	parpars.parse(argc, argv);


	PDBFile pdb_a;
	pdb_a.open(parpars.get("pdb_a"), std::ios::in);

	if (!pdb_a)
	{
		// if file does not exist: complain and abort
		Log.error() << "error opening " << parpars.get("pdb_a") << " for input." << std::endl;
		exit(2);
	}

	PDBFile pdb_b;
	pdb_b.open(parpars.get("pdb_b"), std::ios::in);

	if (!pdb_b)
	{
		// if file does not exist: complain and abort
		Log.error() << "error opening " << parpars.get("pdb_b") << " for output." << std::endl;
		exit(2);
	}

	// read the contents of the file A into a system
	System A;
	pdb_a >> A;
	pdb_a.close();

	// read the contents of the file B into a system
	System B;
	pdb_b >> B;
	pdb_b.close();

	// normalize the names and build all bonds
	FragmentDB db("");
	A.apply(db.normalize_names);
	A.apply(db.build_bonds);
	B.apply(db.normalize_names);
	B.apply(db.build_bonds);


	// TODO: Ask ResidueChecker if everything is ok!


	// calculate the atomic contact energies of A and B
	// NOTE: these methods use BALL internal parameter files
	float ACE_A = calculateACE(A);
	float ACE_B = calculateACE(B);


	// calculate the electrostatic energies of A and B
	AmberFF amber;
	amber.options[AmberFF::Option::ASSIGN_CHARGES] = true;
	amber.options[AmberFF::Option::OVERWRITE_CHARGES] = true;
	amber.setup(A);
	amber.updateEnergy();
	float ES_A = amber.getESEnergy();
	float C_A = calculateCoulomb(A);
	amber.setup(B);
	amber.updateEnergy();
	float ES_B = amber.getESEnergy();
	float C_B = calculateCoulomb(B);

	// finally, join the to systems into a single system
	Log << "atoms in A:  " << A.countAtoms() << endl;
	Log << "atoms in B:  " << B.countAtoms() << endl;
	A.splice(B);
	Log << "final atoms: " << A.countAtoms() << endl;
	Log << "======================" << endl;
	float ACE_AB = calculateACE(A);
	amber.setup(A);
	amber.updateEnergy();
	float ES_AB = amber.getESEnergy();
	float C_AB = calculateCoulomb(A);

	// print the resulting energies
	Log << "ES energy of A: " << ES_A << endl;
	Log << "ES energy of B: " << ES_B << endl;
	Log << "ES energy of AB:" << ES_AB << endl;
	Log << "C energy of A: " << C_A << endl;
	Log << "C energy of B: " << C_B << endl;
	Log << "C energy of AB:" << C_AB << endl;
	Log << "======================" << endl;
	Log << "change in atomic contact energy on binding:   "
       << (ACE_AB - ACE_A - ACE_B) << " kJ/mol" << endl;
	Log << "change in electrostatic energy on binding:    "
       << (ES_AB - ES_A - ES_B) << " kJ/mol" << endl;
	Log << "total binding free energy:                     "
			 << (ACE_AB - ACE_A - ACE_B) + (ES_AB - ES_A - ES_B) << " kJ/mol" << endl;

	// done
	return 0;
}
