// $Id: PDB2discover.C,v 1.5 2000/01/31 16:06:06 oliver Exp $

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
	
	PDBFile	in_file(argv[1]);
	if (in_file.bad())
	{
		Log.error() << "cannot read PDB file " << argv[1] << endl;
		return 2;
	}

	// read the contents of the file into a System
	Log << "reading file " << argv[1] << endl;
	System S;
	in_file >> S;
	in_file.close();

	Log << "opening fragment database" << endl;
	FragmentDB	db;

	Log << "adding hydrogens..." << endl;
	S.apply(*db.addHydrogens);

	Log << "normalizing atom names" << endl;
	db.normalizeNames->setNamingStandard("Discover");
	S.apply(*db.normalizeNames);

	Log << "writing file " << argv[2] << endl;
	PDBFile out_file(argv[2], ios::out);
	out_file << S;
	out_file.close();

	return 0;
}
