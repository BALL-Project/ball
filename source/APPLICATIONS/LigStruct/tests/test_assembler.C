// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include "../sources/ioModule.h"
#include "../sources/structureAssembler.h"
#include "../sources/combiAssembler.h"

#include <BALL/FORMAT/commandlineParser.h>

using namespace BALL;
using namespace std;

/// ################################## C O M M A N D L I N E    P A R S E R
int main(int argc, char* argv[])
{
	CommandlineParser parpars("StructurePrediction", " generate 3D coordinates for a query", 0.1, String(__DATE__), "Test");
	parpars.registerParameter("i", "query molecule as SDF", INFILE, true);
	parpars.registerParameter("o", "output molecule with 3D coordinates SDF", OUTFILE, true);
	
	parpars.setSupportedFormats("i","combi");
	parpars.setSupportedFormats("o","sdf");
	parpars.setOutputFormatSource("o","i");

	String manual = "...currently only predicting structures that consist of rigid fragments...";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
	
	/// ################################## I N P U T
	CombiLibManager combi_man;
	LineBasedFile combi_file(parpars.get("i"), ios::in);
	combi_man.setCombiLib( combi_file );
	
	
	/// ################################## T E M P L A T E    D A T A B A S E
	TemplateDatabaseManager data_man;
	data_man.libraryPathesFromConfig( "/Users/pbrach/files/projects/Master-2014_2015/0_data/used_libs_copies/libraries.conf");
	data_man.readAll();
	
	
	//// ################################## A S S E M B L E    3 D
	//4.) assemble all individual RFragments:
	cout<<" * assebling R-fragments"<<endl;
	StructureAssembler assembler( data_man );
	
	CombiLibMap& r_fragments = combi_man.getCombiLib();
	
	CombiLibMap::iterator it1 = r_fragments.begin();
	for(; it1 != r_fragments.end(); ++it1)
	{
		vector< RFragment* >::iterator it2 = (*it1).begin();
		for(; it2 != (*it1).end(); ++it2)
		{
			cout<<"assembling: "<<LigBase::printInlineMol( (*it2)->molecule)<<endl;
			assembler.assembleStructure( * (*it2)->molecule );
		}
	}
	
	
	// manual connection:
	MoleculeConnector mc;
	mc.setLibs(data_man.getSiteTemplates(), data_man.getBondLengthData());

	RFragment* scaff = r_fragments[0][0];
	RFragment* frag1 = r_fragments[1][0];
	

	Atom* at1 = scaff->r_atom_lst.front().atm;
	Atom* at2 = frag1->group_atom;
	
	Bond* bnd = at1->createBond( *at2 );
	bnd->setOrder(1);
	
	mc.connect(at1, at2);

	at1->getParent()->appendChild( * at2->getParent() );
	
//	CombiAssembler combiner( data_man, &combi_man.getCombiLib() );
	
	SDFile outfile(parpars.get("o"), std::ios::out);
	
//	combiner.writeCombinations(outfile);
	
	outfile << * scaff->molecule;

	outfile.close();
	
	Log <<std::endl<< "......done!"<<std::endl;
}

