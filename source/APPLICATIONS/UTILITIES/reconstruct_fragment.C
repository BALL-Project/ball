// $Id: reconstruct_fragment.C,v 1.1 2003/03/04 14:43:11 anker Exp $
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

	if (argc != 5)
	{
		Log.info() << "Usage:" << endl;
		Log.info() << argv[0] << " fragment-name residue-id infile.pdb outfile.pdb"
			<< endl;
		return(1);
	}

	String reference_fragment_name(argv[1]);
	String res_id(argv[2]);
	String infile_name(argv[3]);
	String outfile_name(argv[4]);

	System system;
	PDBFile infile(infile_name);
	infile >> system;
	infile.close();

	FragmentDB db;
	system.apply(db.normalize_names);
	system.apply(db.build_bonds);

	ResidueIterator res_it = system.beginResidue();
	while (res_it->getID() != res_id) 
	{
		++res_it;
	}

	if (!res_it.isValid())
	{
		std::cerr << "Didn't find res_id " << res_id << " in system" << std::endl;
	}

	const Fragment& ref = *db.getFragment(reference_fragment_name);
	Size atoms = ReconstructFragmentProcessor::reconstructFragment(*res_it, ref);
	Log.info() << " added " << atoms << " atoms" << std::endl;
	Size bonds = db.build_bonds.buildFragmentBonds(*res_it, ref);
	Log.info() << " added " << bonds << " bonds" << std::endl;

	ResidueChecker check(db);
	system.apply(check);

	PDBFile outfile(outfile_name, std::ios::out);
	outfile << system;
	outfile.close();
}
