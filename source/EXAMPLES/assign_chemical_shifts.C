// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//
// A small program for assigning chemical shifts to a peptide given an NMRStar file. 

#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/FORMAT/NMRStarFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/peptides.h>
#include <BALL/NMR/shiftModule.h>

#include <BALL/SYSTEM/path.h>

using namespace std;
using namespace BALL;

int main(int argc, char** argv)
{
	if ((argc < 3) || (argc > 6))
	{
		Log << "Usage:" << argv[0] << " <PDB infile> <NMRStar infile> [pdb_alignment nmr_alignment]" << endl;
		return 1;
	}
    
	Log << "Loading " << argv[1] << "..." << endl;
	PDBFile infile(argv[1]);
	if (!infile.good())
	{
		// if file does not exist: complain and abort
		Log.error() << "error opening " << argv[1] << " for input." << endl;
		return 2;
	}

	System system;
	infile >> system;
	Log << "done." << endl;

	Log << "Initializing FragmentDB..." << endl;
	FragmentDB db("");
	Log << "done." << endl;
		
	Log << "Normalizing names..." << endl;
	system.apply(db.normalize_names);
	Log << "done." << endl;
	
	Chain& chain = *(system.beginChain());
	
	Log << "Loading " << argv[2] << "..." << endl;
	NMRStarFile nmr_file; 

	try{
		nmr_file.open(argv[2]);
	}
	catch(...){
		Log.error() << "error opening " << argv[2] << " for input." << endl;
		return 2;
	}
	nmr_file.read();
	Log << "done." << endl;

	Log << "Creating a mapping "; 
	NMRStarFile::BALLToBMRBMapper mapper(chain, nmr_file);
	if (argc==5)
	{
		String pdb_alignment(argv[3]);
		String nmr_alignment(argv[4]);
		Log << "based on given alignment..." << endl;
		mapper.createMapping(pdb_alignment, nmr_alignment);
	}
	else
	{		
		Log << " based on trivial alignment...";
		mapper.createTrivialMapping();
	}
	Log << "done." << endl;

	Log << "Assigning the chemical shifts..." ;
	nmr_file.assignShifts(mapper);
	Log << "done." << endl;

	for (AtomIterator at_it = chain.beginAtom(); at_it != chain.endAtom(); at_it++)
	{
		if (at_it->hasProperty(ShiftModule::PROPERTY__EXPERIMENTAL__SHIFT))
		{
			cout << at_it->getFullName() << " has shift " 
				   << at_it->getProperty(ShiftModule::PROPERTY__EXPERIMENTAL__SHIFT).getFloat() << endl;
		}
	}
	
	Log << "Assiged " <<	nmr_file.getNumberOfShiftsAssigned() << " out of " 
			<<	nmr_file.getNumberOfAtoms() << " chemical shifts." << endl;

	return 0;
}


