// $Id: PDB2discover.C,v 1.1 1999/08/26 08:02:45 oliver Exp $

#include <BALL/common.h>
#include <BALL/KERNEL/fragmentDB.h>
#include <BALL/FORMAT/PDBFile.h>

int main(int argc, char** argv) 
{

	using namespace BALL;
	
	if (argc < 2)
	{
		cerr << "filename required" << endl;
		return 1;
	}
	
	cout << "reading file " << argv[1] << endl;

	PDBFile	pdb_file;
	Protein	protein;

	pdb_file.open(argv[1]);
	pdb_file >> protein;
	pdb_file.close();
	
	Log.insert(cout);
	
	cout << "opening fragment database" << endl;
	FragmentDB	db("/KM/comp-bio/BALL-data/fragments/Fragments.db");


	cout << "adding hydrogens...";
	protein.apply(*db.addHydrogens);
	cout << "added " << db.addHydrogens->getNumberOfInsertedH() << endl;

	cout << "normalizing atom names" << endl;
	db.normalizeNames->setNamingStandard("Discover");
	protein.apply(*db.normalizeNames);

	cout << "writing file test.pdb" << endl;
	pdb_file.open("test.pdb", ios::out);
	pdb_file << protein;
	pdb_file.close();

	return 0;
}
