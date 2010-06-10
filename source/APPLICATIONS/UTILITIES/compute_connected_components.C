// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/connectedComponentsProcessor.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/molFileFactory.h>

#include <iostream>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{
	Log.setPrefix(cout, "[%T] ");
	Log.setPrefix(cerr, "[%T] ERROR: ");

	// issue a usage hint if called without parameters
	if (argc != 3)
	{
		Log << "Usage: " << argv[0] << " <infile> <outfile format e.g. molecule.hin>" << endl;
		return 1;
	}
	
	// open a molecular file with the name of the first argument
	GenericMolFile* file = MolFileFactory::open(argv[1]);
	if (!file)
	{
		// if file does not exist: complain and abort
		Log.error() << "error opening " << argv[1] << " for input." << endl;
		return 2;
	}
	
	// create a system and read the contents of the molecular file
	System S;
	*file >> S;
	file->close();

	// create the processor
	ConnectedComponentsProcessor ccp;
	
	Log.info() << "determine the files connected components..." << endl;
	S.apply(ccp);

	Log.info() << "detected " << ccp.getNumberOfConnectedComponents() << " connected components." << std::endl;

	Log.info() << "split into connected components..." << endl;
	ccp.splitIntoMolecules(S);

	Log.info() << "writing the different connected components in individual files..." << endl;
	Size current_index = 0;
 
 	// create the outfile's name pattern 	
	String outnameprefix = argv[2];
	vector<String> splits;
 	outnameprefix.split(splits, ".");
	outnameprefix = splits[0];

	Size num_points = splits.size();
	for (Size i=1; i<num_points-1; i++)
		outnameprefix += "."+splits[i];

	// write all connected components into individual files
	for (MoleculeIterator m_it = S.beginMolecule(); +m_it; ++m_it)
	{	
		// write the structure to a file according to the name pattern 
		String outfilename = outnameprefix + "_" + String(current_index) + "." + splits[num_points -1];
		
		GenericMolFile* outfile = MolFileFactory::open(outfilename, std::ios::out);
		if (!outfile)
		{
			// if file type unknown: complain and abort
			Log.error() << "error writing " << outfilename << endl;
			return 2;
		}
		(*outfile) << *m_it;
		outfile->close();

		++current_index;
	}

	// done.
	return 0;
}

