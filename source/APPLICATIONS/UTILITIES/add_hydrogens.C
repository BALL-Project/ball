// $Id: add_hydrogens.C,v 1.5.2.1 2004/05/10 12:04:14 amoll Exp $
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
#include <BALL/DATATYPE/string.h>
#include <BALL/SYSTEM/path.h>

using namespace std;
using namespace BALL;

int main(int argc, char** argv)
{
	if ((argc < 3) || (argc > 4))
	{
		cerr << "A small hydrogen adder." << endl;
		cerr << "Need two filenames: infile.pdb outfile.pdb " << endl;
		cerr << "A third argument can optionally define the amber parameter file"
			<< endl;
		return 1;
	}

	cout << "Loading " << argv[1] << "..." << endl;
	PDBFile infile(argv[1]);
	System system;
	infile >> system;
	cout << "done." << endl;

	cout << "Initializing FragmentDB..." << endl;
	FragmentDB db("");
	cout << "done." << endl;
	cout << "Adding hydrogens..." << endl;
	system.apply(db.add_hydrogens);
	cout << "done." << endl;
	cout << "Building Bonds..." << endl;
	system.apply(db.build_bonds);
	cout << "done." << endl;
	cout << "Normalizing names..." << endl;
	system.apply(db.normalize_names);
	cout << "done." << endl;

	cout << "Applying ResidueChecker..." << endl << endl;
	ResidueChecker check;
	system.apply(check);
	cout << "done." << endl;

	cout << "Initializing force field..." << endl;
	AmberFF amber_ff;
	if (argc == 4)
	{
		Path path;
		String tmp = path.find(argv[3]);
		if (tmp == "") tmp = argv[3];
		amber_ff.options.set(AmberFF::Option::FILENAME, tmp);
	}
	amber_ff.setup(system);
	cout << "done." << endl;
	cout << "Selecting H atoms..." << endl;
	Selector h_select("element(H)");
	system.apply(h_select);
	cout << "done." << endl;
	cout << "Starting minimizer: " << endl << endl;
	ConjugateGradientMinimizer cgm(amber_ff);
	cgm.minimize(1000);

	cout << "Writing " << argv[2] << "..." << endl;
	PDBFile outfile(argv[2], File::OUT);
	outfile << system;
	cout << "done." << endl;

	return 0;
}
