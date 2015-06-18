// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include  <BALL/STRUCTURE/LIGAND3DGEN/ligand3DBase.h>
#include  <BALL/STRUCTURE/LIGAND3DGEN/ioModule.h>
#include  <BALL/STRUCTURE/LIGAND3DGEN/structureAssembler.h>
#include  <BALL/STRUCTURE/LIGAND3DGEN/combiAssembler.h>

#include <BALL/FORMAT/commandlineParser.h>

using namespace BALL;
using namespace std;

/// ################################## C O M M A N D L I N E    P A R S E R
int main(int argc, char* argv[])
{
	CommandlineParser parpars("combiLib 3D structure generation", " generate coordinates for a combiLib", 0.1, String(__DATE__), "Preparation");
	parpars.registerMandatoryInputFile("i", "combiLib as *.combi");
	parpars.registerMandatoryOutputFile("o", "3D molecules in SDFormat");
	parpars.registerOptionalInputFile("fragments", "specifies the path to a custom rigid fragments file in line format");
	parpars.registerOptionalInputFile("sites", "specifies the path to a custom site templates file in sdf format");
	
	parpars.setSupportedFormats("i","combi");
	parpars.setSupportedFormats("o","sdf");
	parpars.setSupportedFormats("fragments","line");
	parpars.setSupportedFormats("sites","sdf");

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
	
	
	//2.) load the fragment databases
	cout<<" * loading databases"<<endl;
	String rigids_path = "";
	String sites_path  = "";
	
	if ( parpars.has("fragments")  )
	{
		rigids_path = parpars.get("fragments");
		cout<<"   loading rigid fragments from: "<<rigids_path<<endl;
	}
	
	if ( parpars.has("sites")  )
	{
		sites_path = parpars.get("sites");
		cout<<"   loading site templates from: "<<sites_path<<endl;
	}
	
	RigidFragmentDB rigid_db( rigids_path );
	SiteFragmentDB  site_db( sites_path );

	//// ################################## C O M B I    A S S E M B L E    3 D
	//3.) assemble all individual RFragments:
	cout<<" * assebling R-fragments"<<endl;

	StructureAssembler assem(rigid_db, site_db); // init structureAssembler with Databases
	
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
	
	//4.) finally calculate all valid combinations:
	cout<<" * generating 3D combinations, and write results to "
		  << parpars.get("o") <<endl;
	
	CombiAssembler combiner( site_db, &combi_man.getCombiLib() );
	SDFile outfile( parpars.get("o"), ios::out);
	
	ConnectionResolver::num_resolve_calls = 0;

	combiner.writeCombinations( outfile );
	
	outfile.close();

	Log << "....done!"<<endl;
}

