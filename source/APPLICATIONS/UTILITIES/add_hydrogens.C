// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// A small program for adding hydrogens to a PDB file (which usually comes
// without hydrogen information) and minimizing all hydrogens by means of a
// StrangLBFGS minimizer.

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/selector.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/MOLMEC/MINIMIZATION/steepestDescent.h>
#include <BALL/MOLMEC/MINIMIZATION/strangLBFGS.h>
#include <BALL/MOLMEC/MINIMIZATION/shiftedLVMM.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/SYSTEM/path.h>

using namespace std;
using namespace BALL;

int main(int argc, char** argv)
{
	if ((argc < 3) || (argc > 4))
	{
		Log << "Usage:" << argv[0] << " <PDB infile> <PDB outfile> [<amber parameter file>]" << endl;
		return 1;
	}

	Log << "Loading " << argv[1] << "..." << endl;
	PDBFile infile(argv[1]);
	if (!infile)
	{
		// if file does not exist: complain and abort
		Log.error() << "error opening " << argv[1] << " for input." << endl;
		return 2;
	}

	System system;
	infile >> system;
	Log << "done." << endl;

	Log << "Initializing FragmentDB..." << endl;
	FragmentDB db("");
	Log << "done." << endl;

	Log << "Normalizing names..." << endl;
	system.apply(db.normalize_names);
	Log << "done." << endl;
	
	Log << "Adding hydrogens..." << endl;
	system.apply(db.add_hydrogens);
	Log << "  ...added " << db.add_hydrogens.getNumberOfInsertedAtoms() 
			<< " atoms" << endl;
	Log << "done." << endl;

	Log << "Building Bonds..." << endl;
	system.apply(db.build_bonds);
	Log << "done." << endl;
	

	Log << "Applying ResidueChecker..." << endl << endl;
	ResidueChecker check;
	system.apply(check);
	Log << "done." << endl;

	Log << "Initializing force field..." << endl;
	AmberFF amber_ff;
	if (argc == 4)
	{
		Path path;
		String tmp = path.find(argv[3]);
		if (tmp == "") tmp = argv[3];
		amber_ff.options.set(AmberFF::Option::FILENAME, tmp);
	}
	amber_ff.setup(system);
	Log << "done." << endl;
	Log << "Selecting H atoms..." << endl;
	Selector h_select("element(H)");
	system.apply(h_select);
	Log << "done." << endl;
	Log << "Starting minimizer: " << endl << endl;
	
	// We choose the L-BFGS minimizer
	
	/*
	SteepestDescentMinimizer sdm(amber_ff);
	sdm.setEnergyOutputFrequency(1);
	sdm.minimize(1000);
	*/
	
	/*
	ConjugateGradientMinimizer cgm(amber_ff);
	cgm.setEnergyOutputFrequency(1);
	cgm.minimize(1000);
	*/
	
	/*
	ShiftedLVMMMinimizer sm(amber_ff);
	sm.setEnergyOutputFrequency(1);
	sm.minimize(1000);
	*/
	
	StrangLBFGSMinimizer bfgsm(amber_ff);
	bfgsm.setEnergyOutputFrequency(1);
	bfgsm.minimize(1000);
	
	Log << "Writing " << argv[2] << "..." << endl;
	PDBFile outfile(argv[2], std::ios::out);
	outfile << system;
	Log << "done." << endl;

	return 0;
}

