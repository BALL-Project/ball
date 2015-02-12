// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "../sources/base.h"
#include "../sources/ioModule.h"
#include "../sources/structureAssembler.h"
#include "../sources/combiAssembler.h"

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
	parpars.registerParameter("o", "output molecules with 3D coordinates in one SDF", OUTFILE, true);
//	parpars.registerParameter("l", "library configuration file", INFILE, false);
	
	parpars.setSupportedFormats("i","combi");
	parpars.setSupportedFormats("o","sdf");
//	parpars.setSupportedFormats("l","conf");
	parpars.setOutputFormatSource("i","o");

	String manual = 
			"Generate a valid (not optimized) 3D structure for structures of an "
			"entire combinatorial library which is defined in a CONF file (either "
			"containing SIMLES for each group or giving the paths multi line SMILES "
			"files).";
	
	parpars.setToolManual(manual);
	parpars.parse(argc, argv);
	
	
///// I N I T    A S S E M B L E R
	
	//1.) setup template database manager, to load the fragment database:
	String config_path = "/Users/pbrach/files/projects/Master-2014_2015/0_data/used_libs_copies/libraries.conf";
	cout<<" * loading databases specified in: "<<config_path<<endl;
	
	TemplateDatabaseManager template_man;
	template_man.libraryPathesFromConfig(config_path);
	template_man.readAll();
	
	// setup structureAssembler:
	StructureAssembler assem( template_man );

	//2.) setup combiLib manager
	cout<<" * parsing input combilib "<<endl;
	
	CombiLibManager combi_man;
	LineBasedFile combi_file(parpars.get("i"), ios::in);
	combi_man.setCombiLib( combi_file );
	
	//#DEBUG 2d controll output
//	//3.) simple testing:
//	String control_path = parpars.get("o") + "_control.sdf";
//	cout<<" * generating 2D combinations, and write them to "<< control_path <<endl;
	
//	//--- generate the control-data: all combinations without coordinates
//	//--- as input for single prediction:
//	list< AtomContainer* > combins;
//	combins.clear();
//	combi_man.generateCombinationsAtomContainer( combins );
	
//	SDFile control_file(control_path, ios::out);
//	//--- write AC-list to SDFile
//	for(list<AtomContainer*>::iterator it = combins.begin(); it != combins.end(); ++it)
//		control_file << **it;
	//#DEBUG 2d controll output END
	
	//4.) assemble all individual RFragments:
	cout<<" * assebling R-fragments"<<endl;
	CombiLibMap& r_fragments = combi_man.getCombiLib();
	
	CombiLibMap::iterator it1 = r_fragments.begin();
	for(; it1 != r_fragments.end(); ++it1)
	{
		vector< RFragment* >::iterator it2 = (*it1).begin();
		for(; it2 != (*it1).end(); ++it2)
		{
			assem.assembleStructure( * (*it2)->molecule );
//			outfile << * (*it2)->molecule; // DEBUG
		}
	}

	
	//5.) finally calculate all valid combinations:
	cout<<" * generating 3D combinations, and write results to "<< parpars.get("o") <<endl;
	
	CombiAssembler combiner( template_man, &combi_man.getCombiLib() );
	SDFile outfile( parpars.get("o"), ios::out);
	
	combiner.writeCombinations( outfile );
	
	outfile.close();


///// Clean up
	combi_file.close();
	Log << "....done!"<<endl;
}

