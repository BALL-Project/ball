// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pdb2amber_naming.C,v 1.4.28.1 2007/08/07 18:26:19 oliver Exp $
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
		Log << "Usage:" << argv[0] << " <PDB infile> <PDB outfile> [<amber parameter file>]" << endl;	

		return 1;
	}

	System system;

	PDBFile f;
	f.open(argv[1]);	
	if (f.bad())
	{
		Log.error() << "cannot read PDB file " << argv[1] << endl;
		return 2;
	}
	f >> system;
	f.close();

	FragmentDB db("");

	// ResidueChecker check(db);
	// system.apply(check);


	db.normalize_names.setNamingStandard("Amber");
	system.apply(db.normalize_names);

	system.apply(db.build_bonds);

	Size cyx_counter = 0;
	Size hip_counter = 0;

	ResidueIterator it = system.beginResidue();
	for (; +it; ++it)
	{
		if (it->getFullName() == "CYS-S")
		{
			it->setName("CYX");
			cyx_counter++;
		}
		if (it->getFullName() == "HIS")
		{
			it->setName("HIP");
			hip_counter++;
		}
	}

	if (cyx_counter > 0)
	{
		Log.info() << "Renamed " << cyx_counter << " residues from CYS-S to CYX"
			<< endl;
	}

	if (hip_counter > 0)
	{
		Log.info() << "Renamed " << hip_counter << " residues from HIS to HIP"
			<< endl;
	}

	PDBFile g;
	g.open(argv[2], ios::out);
	if (g.bad())
	{
		Log.error() << "cannot write PDB file " << argv[2] << endl;
		return 2;
	}


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

