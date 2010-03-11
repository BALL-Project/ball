// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/connectedComponentsProcessor.h>
#include <BALL/FORMAT/MOLFile.h>

#include <iostream>

using namespace BALL;

int main(int argc, char** argv)
{
	// issue a usage hint if called without parameters
	if (argc != 3)
	{
		Log << argv[0] << " <MOL infile> <MOL outfile prefix>" << endl;
		return 1;
	}
	
	// open a MOL file with the name of the first argument
	MOLFile f(argv[1], std::ios::in);
	
	if (!f)
	{
		// if file does not exist: complain and abort
		Log.error() << "error opening " << argv[1] << " for input." << endl;
		return 2;
	}
	
	// create a system and read the contents of the MOL file
	System S;
	f >> S;
	f.close();

	// create the processor
	ConnectedComponentsProcessor ccp;
	
	Log.info() << "determine the files connected components..." << endl;
	S.apply(ccp);

	Log.info() << ccp.getNumberOfConnectedComponents() << std::endl;

	Log.info() << "split into connected components..." << endl;
	ccp.splitIntoMolecules(S);

	Log.info() << "writing the different connected components in individual files..." << endl;
	Size current_index = 0;
	for (MoleculeIterator m_it = S.beginMolecule(); +m_it; ++m_it)
	{	
		MOLFile f2(String(argv[2])+"_"+String(current_index)+".mol", std::ios::out);
		f2 << *m_it;
		f2.close();
		++current_index;
	}
	// done.
	return 0;
}

