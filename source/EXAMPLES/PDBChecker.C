// $Id: PDBChecker.C,v 1.2 2000/01/17 13:12:17 oliver Exp $

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/STRUCTURE/residueChecker.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{
	// print a suage message, if called with the wrong number
	// of arguments
	if (argc != 2)
	{
		Log.error() << argv[0] << " <pdbfile>" << endl;
		Log.error() << "  checks the PDB file for consistency." << endl;
		return 1;
	}

	// create a System and read the contents of the PDB file
	System S;
	PDBFile pdb_file(argv[1]);
	if (pdb_file.bad())
	{
		Log.error() << "cannot open file " << argv[1] << endl;
		return 2;
	}
	pdb_file >> S;
	pdb_file.close();

	// create a fragment database
	// The fragment database contains information on
	// all amino acids: structure, names, bond distances, and so on.
	FragmentDB db;

	// normalize the names
	// This processor converts the atom names from several naming
	// conventions to the standard PDB names
	S.apply(*db.normalizeNames);

	// build bonds
	// Since PDB files normally do not contain bond information
	// we have to reconstruct the bonds from the templates in
	// the fragment database
	S.apply(*db.buildBonds);
	
	
	// now we create a ResidueChecker object and apply it
	// This object checks all residues it is given and 
	// prints warnings to Log if 
	//   - bond distances deviate from their usual values
	//   - non integral charges of a residues (does not apply here)
	//   - missing or superfluous atoms in a residue
	ResidueChecker check(db);
	S.apply(check);

	// done.
	return 0;
}
