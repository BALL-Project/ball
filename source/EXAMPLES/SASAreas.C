// $Id: SASAreas.C,v 1.1 2001/01/31 05:47:27 oliver Exp $

// Calculate the solvent accessible surface (SAS) areas of the
// residues of a protein.

#include <BALL/STRUCTURE/numericalSAS.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/FORMAT/PDBFile.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{
	// prefix all output by the current time
	Log.setPrefix(cout, "[%T]: ");
	Log.setPrefix(cerr, "[%T]: ERROR: ");

	// print usage
	if (argc != 2)
	{
		Log.error() << argv[0] << " <pdb file>" << endl;
		return 1;
	}

	// open the PDB file
	PDBFile pdb(argv[1]);
	if (!pdb)
	{
		// if the file could not be opened, print error message and exit
		Log.error() << "cannot open PBD file " << argv[1] << endl;
		return 2;
	}

	// read the contents of the file into a system
	System S;
	pdb >> S;
	pdb.close();

	// assign the radii defining the solvent accessible surface
	AssignRadiusProcessor rp("radii/PARSE.siz");
	S.apply(rp);

	// calculate the contribution of every single
	// atom of the surface and store it in a hash map
	HashMap<Atom*, float> atom_areas;
	double total_area = calculateSASAtomAreas(S, atom_areas, 1.5, 400);
	Log.info() <<  "total area: " << total_area << "A^2" << endl;
	Log.info() << "# of SAS atoms: " << atom_areas.size() << endl;

	Size number_of_residues = S.countResidues();
	Log.info() << "# of residues: " << number_of_residues << endl;
	
	// we store the accumulated areas of each residue
	// (i.e., the sum of all surface areas of the residue's atoms)
	// in a vector
	vector<float> residue_areas(number_of_residues);

	// iterate over all residues...
	ResidueIterator res_it = S.beginResidue();
	AtomIterator atom_it;
	for (; +res_it; ++res_it)
	{
		//...and sum up the atom areas.
		float residue_area = 0.0;
		for (atom_it = res_it->beginAtom(); +atom_it; ++atom_it)
		{
			if (atom_areas.has(&*atom_it))
			{
				// retrieve the atom area by its pointer
				residue_area += atom_areas[&*atom_it];
			}
		}

		// print the surface area along with the residue name and ID
		Log.info() << res_it->getName() << res_it->getID() << " ";
		if (residue_area > 0.0)
		{
			 Log.info() << residue_area << endl;
		} 
		else 
		{
			 Log.info() << "buried" << endl;
		}
	}
	
	// done
	return 0;
}
			

	
	
