// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/SYSTEM/file.h>

#include "sources/ioModule.h"
#include "sources/structureAssembler.h"

using namespace BALL;
using namespace std;

/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("StructurePrediction", " generate 3D coordinates for a query", 0.1, String(__DATE__), "Prediction");
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
	

///L O A D    L I B s
	TemplateLibraryManager lib_loader;
	if ( parpars.has("c") )
		lib_loader.libraryPathesFromConfig( parpars.get("c") );
	else
		lib_loader.libraryPathesFromConfig( "/Users/pbrach/files/projects/Master-2014_2015/1_code/ball_ligandStruct/source/APPLICATIONS/LigStruct/examples/libraries.conf");
	lib_loader.readAll();

///// A S S E M B L E   3 D
	SDFile infile( parpars.get("i"), ios::in);
	SDFile outfile(parpars.get("o"), ios::out);
	
	Molecule* tmp = infile.read();
	StructureAssembler lig_assembler( lib_loader );

	while (tmp)
	{
		cout<<"read mol starting assembly"<<endl;
		lig_assembler.assembleStructure( *tmp );
		cout<<"assembled structure"<<endl;
		
		outfile << *tmp;
		cout<<"wrote structure"<<endl;
		delete tmp;
		cout<<"deleted old structure"<<endl;
		tmp = infile.read();
		cout<<"read new structure"<<endl;
	}
	
/// F I N I S H
	outfile.close();
	infile.close();
	
	Log << "......done!"<<endl;
}

