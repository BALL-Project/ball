// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include "../sources/ioModule.h"
#include "../sources/structureAssembler.h"

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/QSAR/aromaticityProcessor.h>

using namespace BALL;
using namespace std;

/// ################################## C O M M A N D L I N E    P A R S E R
int main(int argc, char* argv[])
{
	CommandlineParser parpars("CheckFragmenter", " check if structures could be generated", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "query libary as SDF", INFILE, true);
	
	parpars.registerParameter("c", "location of conf file", INFILE, false);
	parpars.setSupportedFormats("c","conf");
	
	parpars.setSupportedFormats("i","sdf");

	String manual = "Check if we can predict all molecules from a library";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
/// ################################## L O A D    D A T A B A S E / I N P U T
	TemplateDatabaseManager lib_loader;
	if ( parpars.has("c") )
	{
		cout<<"Reading configuration from: "<< parpars.get("c") << endl;
		lib_loader.libraryPathesFromConfig( parpars.get("c") );
	}
	else
		lib_loader.libraryPathesFromConfig( "/Users/pbrach/files/projects/Master-2014_2015/0_data/4_used_libs_copies/libraries.conf");
	
	lib_loader.readAll();

	SDFile infile( parpars.get("i"), std::ios::in);
	Molecule* tmp = infile.read();
	
	
//// ################################## A S S E M B L E    3 D
	// StructureAssembler class: this is this tools wrapped main-class:
	
	MoleculeFragmenter   fragmenter;
	AromaticityProcessor arproc;
	Matcher              matcher(lib_loader.getRigidTemplates());

	ACVec fragments, dummy;
	ConnectList dummy2;
	
	// assemble and write every structure to the outfile
	while (tmp)
	{
		LigBase::removeHydrogens( *tmp );
		tmp->apply(arproc);
		
		fragmenter.setMolecule(*tmp);
		fragmenter.fragment(fragments, dummy, dummy2);
		
		try
		{
			for(ACVecIter it = fragments.begin(); it != fragments.end(); ++it)
			{
				matcher.matchFragment( **it);
			}
		}
		catch (...)
		{
			cout<<"-#-"<<endl;
		}

		for(ACVecIter it = dummy.begin(); it != dummy.end(); ++it)
		{
			delete *it;
		}
		
		for(ACVecIter it = fragments.begin(); it != fragments.end(); ++it)
		{
			delete *it;
		}
		
		delete tmp;
		tmp = infile.read();
	}
	
	infile.close();
	
	Log <<std::endl<< "......done!"<<std::endl;
}

