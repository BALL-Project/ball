// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pdb2amber_naming.C,v 1.4 2004/05/27 18:13:08 oliver Exp $
//
// A little helper program that takes standard PDB files and converts them
// to PDB files which use the AMBER naming scheme.

#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/residueChecker.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{

	if (argc != 3) 
	{
		Log.error() << "Need two arguments: PDB -> PDB" << endl;
		exit(1);
	}

	System system;

	PDBFile f;
	f.open(argv[1]);
	f >> system;
	f.close();

	FragmentDB db("");

	// ResidueChecker check(db);
	// system.apply(check);

	system.apply(db.build_bonds);

	db.normalize_names.setNamingStandard("Amber");
	system.apply(db.normalize_names);

	Size cyx_couter = 0;
	Size hip_counter = 0;

	ResidueIterator it = system.beginResidue();
	for (; +it; ++it)
	{
		if (it->getFullName() == "CYS-S")
		{
			it->setName("CYX");
			cyx_couter++;
		}
		if (it->getFullName() == "HIS")
		{
			it->setName("HIP");
			hip_counter++;
		}
	}

	if (cyx_couter > 0)
	{
		Log.info() << "Renamed " << cyx_couter << " residues from CYS-S to CYX"
			<< endl;
	}

	if (hip_counter > 0)
	{
		Log.info() << "Renamed " << hip_counter << " residues from HIS to HIP"
			<< endl;
	}

	PDBFile g;
	g.open(argv[2], ios::out);
	g << system;
	g.close();

	Log.info()
		<< endl
		<< "Conversion to AMBER naming scheme done. Please note that you might"
		<< endl
		<< "have to edit the resulting file by hand (if there are HIS or CYS"
		<< endl
		<< "residues in the original file, e. g.)"
		<< endl
		<< endl
		<< "Good luck."
		<< endl;

}

