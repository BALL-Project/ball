// $Id: ramachandran.C,v 1.6 2000/03/23 20:01:36 oliver Exp $

//============================================================================
// BALL - Ramachandran plot example
//============================================================================
// this example reads a PDB file and calculates the backbone torsion angles
// Phi and Psi that are needed to create ramachandran plots.


#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/FORMAT/PDBFile.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		Log.error() << argv[0] << " <pdb file>" << endl;
		return 1;
	}

	// open the first PDB file
	PDBFile pdb(argv[1]);
	if (pdb.bad())
	{
		// if the file could not be opened, print error message and exit
		Log.error() << "cannot open PBD file " << argv[1] << endl;
		return 2;
	}

	// read the contents of the file A into a system
	System S;
	pdb >> S;
	pdb.close();

	// normalize the atom names (torsion angles are identified
	// via the atom names)
	Log.info() << "normalizing names..." << endl;
	FragmentDB db;
	S.apply(db.normalize_names);
	
	// now, iterate over all residues
	ResidueIterator res_it = S.beginResidue();
	for (;+res_it; ++res_it)
	{
    // torsion angle phi does not exist for N terminal residues
		if (res_it->hasTorsionPhi())
		{
			// calculate the torsion angle and print its value in degree
			Angle phi = res_it->getTorsionPhi();
			cout << "phi(" << res_it->getName() << res_it->getID() << ") = " << phi.toDegree() << endl;
		}

    // torsion angle psi does not exist for C terminal residues
		if (res_it->hasTorsionPsi())
		{
			// calculate the torsion angle and print its value in degree
			Angle psi = res_it->getTorsionPsi();
			cout << "psi(" << res_it->getName() << res_it->getID() << ") = " << psi.toDegree() << endl;
		}
	}

	// done
	return 0;
}
			

	
	
