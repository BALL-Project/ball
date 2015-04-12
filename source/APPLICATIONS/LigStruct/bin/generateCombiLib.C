// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "../sources/base.h"
#include "../sources/ioModule.h"
#include "../sources/combiAssembler.h"

#include <BALL/FORMAT/commandlineParser.h>

using namespace BALL;
using namespace std;

/// ################################## C O M M A N D L I N E    P A R S E R
int main(int argc, char* argv[])
{
	float overhead_time, assembly_time, combi_time; //#DEBUG
	Timer my_timer; //#DEBUG
	my_timer.start(); //#DEBUG
	
	CommandlineParser parpars("combiLib 3D structure generation", " generate coordinates for a combiLib", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "combiLib as *.combi", INFILE, false);
	parpars.registerParameter("o", "3D molecules in SDFormat", OUTFILE, true);
	parpars.registerParameter("c", "configuration file in *.conf", INFILE, false);
	
	parpars.registerFlag("w", "write results to out file"); //#DEBUG
	parpars.registerFlag("u", "use structure re-use"); //#DEBUG
	
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
	combi_file.close();
	
	cout<<"ok"<<endl;
	
	
	//2.) setup template database manager, to load the fragment databases
	String config_path = "databases.conf";
	
	if ( parpars.has("c") )
		config_path = parpars.get("c");
	
	TemplateDatabaseManager template_man;
	template_man.libraryPathesFromConfig( config_path );
	
	cout<<" * loading databases specified in: "<<config_path<<endl;
	template_man.readAll(); 

	//#DEBUG - start
	my_timer.stop();
	overhead_time = my_timer.getSeconds();
	my_timer.reset();
	my_timer.start();
	//#DEBUG - end
	
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
			 // assemble each r-fragment, and set the rotors_list
			(*r_frag)->rotor_lst = assem.assembleStructure( * (*r_frag)->molecule );
		}
	}
	
	//#DEBUG - start
	my_timer.stop();
	assembly_time = my_timer.getSeconds();
	my_timer.reset();
	my_timer.start();
	//#DEBUG - end
	
	//4.) finally calculate all valid combinations:
	cout<<" * generating 3D combinations, and write results to "
		  << parpars.get("o") <<endl;
	
	CombiAssembler combiner( template_man, &combi_man.getCombiLib() );
	SDFile outfile( parpars.get("o"), ios::out);
	
	ConnectionResolver::num_resolve_calls = 0;

	CombiAssembler::apply_reuse = parpars.has("u");
	CombiAssembler::write_output = parpars.has("w") ;
	combiner.writeCombinations( outfile );
	
	//#DEBUG - start
	my_timer.stop();
	combi_time = my_timer.getSeconds();
	cout<<"calls to resolve(): "<<ConnectionResolver::num_resolve_calls<<endl; 
	cout<<"overhead:   "<<overhead_time<<endl; 
	cout<<"assembly:   "<<assembly_time<<endl;
	cout<<"combiner:   "<<combi_time<<endl;
		//#DEBUG - end
	
	outfile.close();

//	Log << "....done!"<<endl;
}

