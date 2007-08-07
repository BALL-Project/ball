// $Id: pdb2hin.C,v 1.1.12.3 2007/08/07 18:26:19 oliver Exp $
//
// A very simple utility for converting PDB files into HyperChem files
//
// Still missing: A flag for choosing an additional step checking the
// molecule. At the moment the molecule will treated with normalize_names
// and build_bonfs which is only applicable to molecules that are present
// in the FragmentDB, i. e. currently only proteins and DNA. This might
// confuse users.

#include <BALL/KERNEL/system.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{

	if (argc != 2)
	{
		Log.info() << "Usage:" << argv[0] << " <PDB infile>" << endl;
		//Log.error() << "Need one filename! Aborting." << endl;
		return(1);
	}
	
	String infile_name(argv[1]);
	String outfile_name(infile_name);
	outfile_name.reverse();
	outfile_name = outfile_name.after(".");
	outfile_name.reverse();
	outfile_name = outfile_name + ".hin";

	PDBFile pdb_file(infile_name);	
	if (pdb_file.bad())
	{
		Log.error() << "cannot read PDB file " << argv[1] << endl;
		return 2;
	}

	System system;
	pdb_file >> system;
	pdb_file.close();
	Log.info() << "System contains " << system.countAtoms() << " atoms." << endl;

	FragmentDB db("");
	system.apply(db.normalize_names);
	system.apply(db.build_bonds);

	HINFile hin_file(outfile_name, ios::out);
	hin_file << system;
	hin_file.close();
	Log.info() << "Wrote HyperChem file " << outfile_name << endl;

	return(0);
}
