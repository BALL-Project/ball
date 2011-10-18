// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/molFileFactory.h>

#include <BALL/KERNEL/system.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{
	Log.setPrefix(cout, "[%T] ");
	Log.setPrefix(cerr, "[%T] ERROR: ");

	// issue a usage hint if called without parameters
	if (argc != 3)
	{
		Log <<  "Usage: " << argv[0] << " <infile> <outfile>" << endl;
		return 1;
	}

	// open a molecular file with the name of the first argument
	GenericMolFile* file = MolFileFactory::open(argv[1]);
	if (!file)
	{
		// if file does not exist: complain and abort
		Log.error() << "error opening " << argv[1] << " for input." << endl;
		return 2;
	}

	// create a system and read the contents of the molecular file
	System S;
	*file >> S;
	file->close();

	// print the number of molecules, atoms, and bonds read from the file
	Log << "read " << S.countMolecules() << " molecules, "
	               << S.countAtoms()     << " atoms, and "
								 << S.countBonds()     << " bonds." << endl;

	// write the structure to a file whose
	// name is given as the second command line argument
	Log << "writing outfile " << argv[2] << endl;
	GenericMolFile* outfile = MolFileFactory::open(argv[2], std::ios::out);
	if (outfile)
	{
		(*outfile) << S;
		outfile->close();
	}
	else
	{
		// if file type unknown: complain and abort
		Log.error() << "error writing " << argv[1] << endl;
		return 2;
	}

	// done.
	return 0;
}

