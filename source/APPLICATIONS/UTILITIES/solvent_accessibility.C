// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


// This utility reads a PDB structure, removes water from it and
// computes which residues are solvent exposed by summing up
// atom-based solvent-accessible surface areas. It will then
// print all residues along with their solvent-accessible
// surface area and mark all residues with an area below 10 A^2 
// as buried.

#include <BALL/common.h>

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/selector.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/numericalSAS.h>
#include <BALL/STRUCTURE/defaultProcessors.h>

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

	Log.info() << "Loading " << argv[1] << "..." << endl;
	PDBFile infile(argv[1]);
	if (!infile)
	{
		// if the file could not be opened, print error message and exit
		Log.error() << "cannot open PBD file " << argv[1] << endl;
		return 2;
	}
	System S;
	infile >> S;
	infile.close();
	Log.info() << "  done." << endl;

	Log.info() << "Removing water molecules..." << endl;
	Size atoms = S.countAtoms();
	Selector wat("residue(HOH) OR residue(WAT)");
	S.apply(wat);
	S.removeSelected();
	Log.info() << "  removed " << atoms - S.countAtoms() << " atoms." << endl;

	Log.info() << "Apply the AssignRadiusProcessor..." << endl;
	// assign the radii defining the solvent accessible surface
	AssignRadiusProcessor rp("radii/PARSE.siz");
	S.apply(rp);
	Log.info() << "  done." << endl;

	float min_area = 10.0;
	if (argc == 3)
	{
		min_area = String(argv[2]).toFloat();
	}
	Log.info() << "Residues will be marked as 'buried' if their accessible area is below " 
       << min_area << " A^2" << endl;

	Options options;
	options[NumericalSAS::Option::NUMBER_OF_POINTS] = 400;
	options[NumericalSAS::Option::PROBE_RADIUS    ] = 1.5;
	options[NumericalSAS::Option::COMPUTE_VOLUME  ] = false;

	NumericalSAS numerical_sas(options);
	numerical_sas(S);
	HashMap<const Atom*, float> atom_areas = numerical_sas.getAtomAreas();

	Log.info() << "Residue solvent-accessible areas [A^2]:" << endl;
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
		Log.info() << "  " << ri->getName() << ri->getID() << "\t" << area << ((area < min_area) ? "\t (buried)" : "") << endl;
	}

	return 0;
}
