// $Id: add_hydrogens.C,v 1.4 2003/07/15 13:27:46 anker Exp $
//
// A small program for adding hydrogens to a PDB file (which usually comes
// without hydrogen information) and minimizing all hydrogens by means of a
// conjugate gradient minimizer.

#include <BALL/common.h>

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/selector.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/MOLMEC/MINIMIZATION/steepestDescent.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/residueChecker.h>

using namespace std;
using namespace BALL;

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		Log.error() << "A small hydrogen adder." << endl;
		Log.error() << "Need two filenames: infile.pdb outfile.pdb " << endl;
		return 1;
	}

	Log.info() << "Loading " << argv[1] << "..." << endl;
	PDBFile infile(argv[1]);
	System system;
	infile >> system;
	Log.info() << "done." << endl;

	Log.info() << "Initializing FragmentDB..." << endl;
	FragmentDB db;
	Log.info() << "done." << endl;
	Log.info() << "Adding hydrogens..." << endl;
	system.apply(db.add_hydrogens);
	Log.info() << "done." << endl;
	Log.info() << "Building Bonds..." << endl;
	system.apply(db.build_bonds);
	Log.info() << "done." << endl;
	Log.info() << "Normalizing names..." << endl;
	system.apply(db.normalize_names);
	Log.info() << "done." << endl;

	Log.info() << "Applying ResidueChecker..." << endl << endl;
	ResidueChecker check;
	system.apply(check);
	Log.info() << "done." << endl;

	Log.info() << "Initializing force field..." << endl;
	AmberFF amber_ff(system);
	Log.info() << "done." << endl;
	Log.info() << "Selecting H atoms..." << endl;
	Selector h_select("element(H)");
	system.apply(h_select);
	Log.info() << "done." << endl;
	Log.info() << "Starting minimizer: " << endl << endl;
	ConjugateGradientMinimizer cgm(amber_ff);
	cgm.minimize(1000);

	Log.info() << "Writing " << argv[2] << "..." << endl;
	PDBFile outfile(argv[2], File::OUT);
	outfile << system;
	Log.info() << "done." << endl;

	return 0;
}
