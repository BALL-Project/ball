// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "../sources/base.h"
#include "../sources/structureAssembler.h"

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>

using namespace BALL;
using namespace std;

/// ################# M A I N #################
int main(int argc, char* argv[])
{
	/// ------ S E T    P A R S E R -------
	CommandlineParser parpars("3D structure generation of ligands", " generate coordinates for a combiLib", 0.1, String(__DATE__), "Prediction");
	parpars.registerParameter("i", "combiLib as CONF file", INFILE, false);
	parpars.registerParameter("o", "output molecule with 3D coordinates SDF", OUTFILE, true);
	parpars.registerParameter("l", "library configuration file", INFILE, false);
	
	parpars.setSupportedFormats("i","smi");
	parpars.setSupportedFormats("o","sdf");
	parpars.setSupportedFormats("l","conf");
	parpars.setOutputFormatSource("i","o");

	String manual = 
			"Generate a valid (not optimized) 3D structure for structures of an "
			"entire combinatorial library which is defined in a CONF file (either "
			"containing SIMLES for each group or giving the paths multi line SMILES "
			"files).";
	
	parpars.setToolManual(manual);
	parpars.parse(argc, argv);
	

///// I N I T    A S S E M B L E R
//	StructureAssembler assem;
//	// Load all template libs:
//	if ( parpars.has("l") )
//	{
////		assem.setLibsFromConfig( parpars.get("l") );
//	}
//	else
//	{
//		String pth = "/Users/pbrach/files/projects/Master-2014_2015/1_code/ball_ligandStruct/source/APPLICATIONS/LigStruct/examples/libraries.conf";
////		assem.setLibsFromConfig(pth);
//	}

	
///// I N I T I A L    R - G R O U P    A S S E M B L Y

//	Log << "Reading combi-lib..."<<endl;
//	CombiLib in_lib;
//	String combi_path = "/Users/pbrach/files/projects/Master-2014_2015/1_code/ball_ligandStruct/source/APPLICATIONS/LigStruct/examples/test.conf";
//	readGroups(in_lib, combi_path, assem);
//	Log <<" - done - "<<endl<<endl;
	

	
	
///// F I N I S H
///// write output
//	String outfile_name = String(parpars.get("o"));
//	SDFile outfile(outfile_name, ios::out);

//	Log << "wrote structure to file: "<< endl << outfile_name << endl<<endl;
	
///// Clean up
//	outfile.close();
}

