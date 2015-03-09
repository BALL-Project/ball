// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include "../sources/ioModule.h"
#include "../sources/structureAssembler.h"

#include <BALL/FORMAT/commandlineParser.h>

using namespace BALL;
using namespace std;

/// ################################## C O M M A N D L I N E    P A R S E R
int main(int argc, char* argv[])
{
	CommandlineParser parpars("StructurePrediction", " generate 3D coordinates for a query", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "query molecule as SDF", INFILE, true);
	parpars.registerParameter("o", "output molecule with 3D coordinates SDF", OUTFILE, true);
	
	parpars.registerParameter("c", "location of conf file", INFILE, false);
	parpars.setSupportedFormats("c","conf");
	
	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");
	parpars.setOutputFormatSource("o","i");

	String manual = "...currently only predicting structures that consist of rigid fragments...";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
/// ################################## L O A D    D A T A B A S E / I N P U T
	TemplateDatabaseManager lib_loader;
	if ( parpars.has("c") )
		lib_loader.libraryPathesFromConfig( parpars.get("c") );
	else
		lib_loader.libraryPathesFromConfig( "/Users/pbrach/files/projects/Master-2014_2015/0_data/used_libs_copies/libraries.conf");
	lib_loader.readAll();

	SDFile infile( parpars.get("i"), std::ios::in);
	Molecule* tmp = infile.read();
	
	
//// ################################## A S S E M B L E    3 D
	// StructureAssembler class: this is this tools wrapped main-class:
	StructureAssembler lig_assembler( lib_loader );

	SDFile outfile(parpars.get("o"), std::ios::out);
	
	// assemble and write every structure to the outfile
	while (tmp)
	{
		LigBase::removeHydrogens( *tmp );

		lig_assembler.assembleStructure( *tmp );
		
		outfile << *tmp;
				
		delete tmp;
		tmp = infile.read();
	}
	
	infile.close();
	outfile.close();
	
	Log <<std::endl<< "......done!"<<std::endl;
}

