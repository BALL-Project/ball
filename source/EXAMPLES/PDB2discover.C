// $Id: PDB2discover.C,v 1.4 2000/01/17 13:12:17 oliver Exp $

#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/FORMAT/PDBFile.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv) 
{

	// check for command line arguments
	if (argc != 3)
	{
		Log.error() << argv[0] << " <PDB file> <new PDB file>" << endl;
		Log.error() << "  read a PDB file from <PDB file>, add hydrogen atoms where needed" << endl;
		Log.error() << "  convert the atom names to the Discover naming scheme, and write" << endl;
		Log.error() << "  everything to <new PDB file>." << endl << endl;
		return 1;
	}
	
	PDBFile	pdb_file(argv[1]);
	if (pdb_file.bad())
	{
		Log.error() << "cannot read PDB file " << argv[1] << endl;
		return 2;
	}

	// read the contents of the file into a System
	Log << "reading file " << argv[1] << endl;
	System S;
	pdb_file >> S;
	pdb_file.close();

	Log << "opening fragment database" << endl;
	FragmentDB	db;

	Log << "adding hydrogens..." << endl;
	S.apply(*db.addHydrogens);

	Log << "normalizing atom names" << endl;
	db.normalizeNames->setNamingStandard("Discover");
	S.apply(*db.normalizeNames);

	Log << "writing file " << argv[2] << endl;
	pdb_file2.open(argv[2], ios::out);
	pdb_file2 << S;
	pdb_file2.close();

	return 0;
}
