// $Id: ramachandran.C,v 1.3 2000/01/17 13:12:18 oliver Exp $

//============================================================================
// BALL - Ramachandran plot example
//============================================================================
// this example reads a PDB file and calculates the backbone torsion angles
// Phi and Psi that are needed to create ramachandran plots.


#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/ENERGY/atomicContactEnergy.h>
#include <BALL/ENERGY/coulomb.h>
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

	// normalize the names and build all bonds
	Log.info() << "normalizing names and building bonds..." << endl;
	FragmentDB db;
	S.apply(*db.normalizeNames);
	
	// now, iterate over all residues
	ResidueIterator res_it = S.beginResidue();
	for (;+res_it; ++res_it)
	{
		Atom* C = 0;
		Atom* N = 0;
		Atom* CA = 0;
		AtomIterator atom_it = res_it->beginAtom();
		for (; +atom_it; ++atom_it)
		{
			// remember the backbone atoms  C, N, and CA of residue i
			if (atom_it->getName() == "C") C = &*atom_it;
			if (atom_it->getName() == "N") N = &*atom_it;
			if (atom_it->getName() == "CA") CA = &*atom_it;
		}
		
		// phi is not defined for N-terminal residues
		if (!res_it->isNTerminal())
		{	
			// find C(i-1), the C atom of the previous residue
			ResidueIterator last_res = res_it;
			last_res--;
			Atom* last_C = 0;
			for (atom_it = last_res->beginAtom(); +atom_it; atom_it++)
			{
				if (atom_it->getName() == "C")
				{
					last_C = &*atom_it;
					break;
				}
			}
			
			// if we found C(i-1), N(i), CA(i), and C(i), we can calculate phi
			if ((last_C != 0) && (N != 0) && (CA != 0) && (C != 0))
			{
				Angle phi = calculateTorsionAngle(*last_C, *N, *CA, *C);
				cout << "phi(" << res_it->getName() << res_it->getID() << ") = " << phi.toDegree() << endl;
			}
		}

		// psi is not defined for C-terminal residues
		if (!res_it->isCTerminal())
		{	
			// find N(i+1)
			ResidueIterator next_res = res_it;
			next_res++;
			Atom* next_N = 0;
			for (atom_it = next_res->beginAtom(); +atom_it; atom_it++)
			{
				if (atom_it->getName() == "N")
				{
					next_N = &*atom_it;
					break;
				}
			}
			
			// if we found N(i), CA(i), C(i), and N(i+1), we can calculate psi
			if ((next_N != 0) && (N != 0) && (CA != 0) && (C != 0))
			{
				Angle psi = calculateTorsionAngle(*N, *CA, *C, *next_N);
				cout << "psi(" << res_it->getName() << res_it->getID() << ") = " << psi.toDegree() << endl;
			}
		}
	}

	// done
	return 0;
}
			

	
	
