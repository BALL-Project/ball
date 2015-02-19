// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "../sources/base.h"
#include "../sources/ioModule.h"
#include "../sources/combiAssembler.h"

#include <BALL/FORMAT/commandlineParser.h>

#define CONF_PATH "/Users/pbrach/files/projects/Master-2014_2015/0_data/used_libs_copies/libraries.conf"

using namespace BALL;
using namespace std;

/// ################################## C O M M A N D L I N E    P A R S E R
int main(int argc, char* argv[])
{
	CommandlineParser parpars("combiLib 3D structure generation", " generate coordinates for a combiLib", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "combiLib as *.combi", INFILE, false);
	parpars.registerParameter("o", "3D molecules in SDFormat", OUTFILE, true);
	parpars.registerParameter("c", "configuration file in *.conf", INFILE, false);
	
	parpars.setSupportedFormats("i","combi");
	parpars.setSupportedFormats("o","sdf");
	parpars.setSupportedFormats("c","conf");
	parpars.setOutputFormatSource("i","o");

	String manual = 
			"Generate a valid (not optimized) 3D structure for structures of an "
			"entire combinatorial library which is defined in a COMBI file";
	
	parpars.setToolManual(manual);
	parpars.parse(argc, argv);
	
	
	/// ################################## L O A D    D A T A B A S E / I N P U T
	//1.) setup combiLib manager, parse combiLib, check if its correctly written
	cout<<" * parsing input combilib... ";
	
	CombiLibManager combi_man;
	LineBasedFile combi_file(parpars.get("i"), ios::in);
	combi_man.setCombiLib( combi_file );
	
	cout<<"OK!"<<endl;
	
	
	//2.) setup template database manager, to load the fragment databases
	String config_path = CONF_PATH;
	
	if ( parpars.has("c") )
		config_path = parpars.get("c");
	
	TemplateDatabaseManager template_man;
	template_man.libraryPathesFromConfig( config_path );
	
	cout<<" * loading databases specified in: "<<config_path<<endl;
	template_man.readAll(); 

	
	//// ################################## C O M B I    A S S E M B L E    3 D
	//3.) assemble all individual RFragments:
	cout<<" * assebling R-fragments"<<endl;

	StructureAssembler assem( template_man ); // init structureAssembler with Databases
	
	CombiLibMap& c_lib = combi_man.getCombiLib();
	
	// for each r-group
	for(CombiLibMap::iterator r_group = c_lib.begin(); 
			r_group != c_lib.end(); ++r_group)
	{
		// for each r-fragment in that group
		for(vector< RFragment* >::iterator r_frag = (*r_group).begin(); 
				r_frag != (*r_group).end(); ++r_frag)
		{
			assem.assembleStructure( * (*r_frag)->molecule ); // assem each r-fragment
		}
	}
	
	
	//4.) finally calculate all valid combinations:
	cout<<" * generating 3D combinations, and write results to "
		  << parpars.get("o") <<endl;
	
	CombiAssembler combiner( template_man, &combi_man.getCombiLib() );
	SDFile outfile( parpars.get("o"), ios::out);
	
	combiner.writeCombinations( outfile );
	
	outfile.close();

	Log << "....done!"<<endl;
}

