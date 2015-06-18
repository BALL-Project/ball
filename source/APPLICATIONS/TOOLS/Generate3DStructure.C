// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include  <BALL/STRUCTURE/LIGAND3DGEN/ioModule.h>
#include  <BALL/STRUCTURE/LIGAND3DGEN/structureAssembler.h>

#include <BALL/FORMAT/commandlineParser.h>

using namespace BALL;
using namespace std;

/// ################################## C O M M A N D L I N E    P A R S E R
int main(int argc, char* argv[])
{
	CommandlineParser parpars("StructurePrediction", " generate 3D coordinates for a query", 0.1, String(__DATE__), "Preparation");
	parpars.registerMandatoryInputFile("i", "query molecule as SDF");
	parpars.registerMandatoryOutputFile("o", "output molecule with 3D coordinates SDF");
	
	parpars.registerOptionalInputFile("fragments", "specifies the path to a custom rigid fragments file in line format");
	parpars.registerOptionalInputFile("sites", "specifies the path to a custom site templates file in sdf format");
	
	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");
	parpars.setSupportedFormats("fragments","line");
	parpars.setSupportedFormats("sites","sdf");

	String manual = "...currently only predicting structures that consist of rigid fragments...";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
/// ################################## L O A D    D A T A B A S E / I N P U T
	cout<<" * loading databases "<<endl;
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

	
	SDFile infile( parpars.get("i"), std::ios::in);
	Molecule* tmp = infile.read();
	
	
//// ################################## A S S E M B L E    3 D
	// StructureAssembler class: this is this tools wrapped main-class:
	StructureAssembler lig_assembler( rigid_db, site_db );

	SDFile outfile(parpars.get("o"), std::ios::out);
	ConnectList* dummy_con_lst = 0;
			
	// assemble and write every structure to the outfile
	while (tmp)
	{
		LigBase::removeHydrogens( *tmp );

		try
		{
			dummy_con_lst = lig_assembler.assembleStructure( *tmp );

			delete dummy_con_lst;
			outfile << *tmp;
		}
		catch (BALL::Exception::StructureNotGenerated e)
		{
			cout<<"############## NOT Converted ###############"<<endl;
			cout<<"Molecule name: "<< tmp->getName() <<endl;
			cout<<e.getMessage()<<endl;
		}

		delete tmp;
		tmp = infile.read();
	}
	
	infile.close();
	outfile.close();
	
	Log <<std::endl<< "......done!"<<std::endl;
}

