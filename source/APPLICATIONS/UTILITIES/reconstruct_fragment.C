// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: reconstruct_fragment.C,v 1.7 2004/05/27 18:13:08 oliver Exp $
//
// A little helper program that tries to reconstruct broken fragments in a
// molecule. This program assumes that there is only *one* chain in the
// file.
//
// Usage: reconstruct_fragemnt reference_fragment_name res_id infile outfile 


#include <BALL/common.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/STRUCTURE/reconstructFragmentProcessor.h>
#include <BALL/FORMAT/PDBFile.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{

	if ((argc != 5) && (argc != 3))
	{
		Log.info() << "Usage:" << endl;
		Log.info() << argv[0] << " infile.pdb outfile.pdb" << "\n" << endl;
		Log.info() << " or\n" << endl;
		Log.info() << argv[0] << " fragment-name residue-id infile.pdb outfile.pdb"
			<< endl;
		return(1);
	}

	bool automatic = true;

	String reference_fragment_name;
	String res_id;
	String infile_name;
	String outfile_name;

	if (argc == 5)
	{
		automatic = false;

		reference_fragment_name = argv[1];
		res_id = argv[2];
		infile_name = argv[3];
		outfile_name = argv[4];
	}
	else
	{
		infile_name = argv[1];
		outfile_name = argv[2];
	}

	System system;
	PDBFile infile(infile_name);
	infile >> system;
	infile.close();

	FragmentDB db("");
	system.apply(db.normalize_names);

	if (automatic)
	{
		ReconstructFragmentProcessor proc(db);
		system.apply(proc);
		system.apply(db.build_bonds);
	}
	else
	{

		ResidueIterator res_it = system.beginResidue();
		while (res_it->getID() != res_id) 
		{
			++res_it;
		}

		if (!res_it.isValid())
		{
			cerr << "Didn't find res_id " << res_id << " in system" << endl;
		}

		const Fragment& ref = *db.getFragment(reference_fragment_name);
		list<Atom*> atoms = ReconstructFragmentProcessor::reconstructFragment(*res_it, ref);
		Log.info() << " added " << atoms.size() << " atoms" << std::endl;
		Size bonds = db.build_bonds.buildFragmentBonds(*res_it, ref);
		Log.info() << " added " << bonds << " bonds" << std::endl;
	}

	ResidueChecker check(db);
	system.apply(check);

	PDBFile outfile(outfile_name, ios::out);
	outfile << system;
	outfile.close();
}
