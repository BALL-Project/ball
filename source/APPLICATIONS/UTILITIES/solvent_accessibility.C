// $Id: solvent_accessibility.C,v 1.1 2004/05/14 16:16:18 oliver Exp $
//
// This utility reads a PDB structure, removes water from it and
// computes which residues are solvent exposed by summing up
// atom-based solvent-accessible surface areas. It will then
// print all residues along with their solvent-accessible
// surface area and mark all residues with an area below 1 A^2 
// as buried.

#include <BALL/common.h>

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/selector.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/numericalSAS.h>

using namespace std;
using namespace BALL;

int main(int argc, char** argv)
{
	if ((argc != 2) && (argc != 3))
	{
		cerr << argv[0] << " <filename> [<buried area>]" << endl;
		cerr << " This utility reads a PDB structure, removes water from it and" << endl
				 << " computes which residues are solvent exposed by summing up" << endl
				 << " atom-based solvent-accessible surface areas. It will then" << endl
				 << " print all residues along with their solvent-accessible" << endl
				 << " surface area and mark all residues with an area below 10 A^2" << endl
				 << " as buried." << endl
				 << " If an additional argument for the buried area is given, then only" << endl
			   << " those residues with an area below this value will be marked as buried." << endl
				 << endl;
		return 1;
	}

	cout << "Loading " << argv[1] << "..." << endl;
	PDBFile infile(argv[1]);
	System S;
	infile >> S;
	infile.close();
	cout << "  done." << endl;

	cout << "Removing water molecules..." << endl;
	Size atoms = S.countAtoms();
	Selector wat("residue(HOH) OR residue(WAT)");
	S.apply(wat);
	S.removeSelected();
	cout << "  removed " << atoms - S.countAtoms() << " atoms." << endl;


	float min_area = 10.0;
	if (argc == 3)
	{
		min_area = String(argv[2]).toFloat();
	}
	cout << "Residues will be marked as 'buried' if their accessible area is below " 
       << min_area << " A^" << endl;

	HashMap<const Atom*, float> atom_areas;
	calculateSASAtomAreas(S, atom_areas);

	cout << "Residue solvent-accessible areas [A^2]:" << endl;
	HashMap<Residue*, float> residue_areas;
	for (ResidueIterator ri = S.beginResidue(); +ri; ++ri)
	{
		float area = 0.0;
		for (AtomIterator ai = ri->beginAtom(); +ai; ++ai)
		{
			if (atom_areas.has(&*ai))
			{
				area += atom_areas[&*ai];
			}
		}
		cout << "  " << ri->getName() << ri->getID() << "\t" << area << ((area < 5.0) ? "\t (buried)" : "") << endl;
	}

	return 0;
}
