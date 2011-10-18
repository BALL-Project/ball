// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/structureMapper.h>

using namespace std;
using namespace BALL;

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		cerr << "Need two PDB file names. Aborting." << endl;
		return(0);
	}

	PDBFile file1(argv[1]);
	System system1;
	file1 >> system1;
	file1.close();

	PDBFile file2(argv[2]);
	System system2;
	file2 >> system2;
	file2.close();

	if (system1.countAtoms() != system2.countAtoms())
	{
		cerr << "Different atom counts: " << system1.countAtoms() 
			<< " : " << system2.countAtoms() << endl;
		return(0);
	}

	StructureMapper mapper(system1, system2);
	mapper.calculateDefaultBijection();
	if (mapper.getBijection().size() != system1.countAtoms())
	{
		cout << "WARNING: only " << mapper.getBijection().size() 
			<< " of " << system1.countAtoms() << " atoms mapped!" 
			<< endl;
	}
	cout << "RMSD is " << mapper.calculateRMSD() << endl;
	return(0);
}
