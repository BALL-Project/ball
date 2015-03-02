// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>

#include "../sources/clashResolver.h"
#include "../sources/structureAssembler.h"
#include "../sources/ioModule.h"

using namespace BALL;
using namespace std;

int main(int argc, char* argv[])
{
	/// ################################## C O M M A N D L I N E    P A R S E R
	CommandlineParser parpars("test ClashResolver", "  ", 0.1, String(__DATE__), "Testing");
	parpars.registerParameter("i", "input SDF", INFILE, true);
	parpars.registerParameter("o", "output SDF", OUTFILE, true);
	
	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");
	parpars.setOutputFormatSource("o","i");
	
	String manual = "...just testing clashes...";
	parpars.setToolManual(manual);
	
	parpars.parse(argc, argv);
	
	/// ################################## C O D E
	
	Log << "Reading molecule..."<<endl;
	SDFile outfile(parpars.get("o"), ios::out);
	
	// load templates
	TemplateDatabaseManager data_man;
	data_man.libraryPathesFromConfig( "/Users/pbrach/files/projects/Master-2014_2015/0_data/used_libs_copies/libraries.conf" );
	data_man.readAll();
	
	// load combiLib
	CombiLibManager combi_man;
	LineBasedFile combi_file( parpars.get("i"), ios::in );
	combi_man.setCombiLib( combi_file );
	
	CombiLibMap& r_fragments = combi_man.getCombiLib();
	
	
	// get coordinates for every combi-fragment:
	StructureAssembler assembler( data_man );
	
	CombiLibMap::iterator it1 = r_fragments.begin();
	for(; it1 != r_fragments.end(); ++it1)
	{
		vector< RFragment* >::iterator it2 = (*it1).begin();
		for(; it2 != (*it1).end(); ++it2)
		{
			(*it2)->rotor_lst = assembler.assembleStructure( * (*it2)->molecule );
		}
	}
	cout<<"Done with assembly!"<<endl<<endl;
	
	// manual connection:
	MoleculeConnector mc;
	mc.setLibs( data_man.getSiteTemplates(), data_man.getBondLengthData() );

	RFragment* scaff = r_fragments[0][0];
	RFragment* frag1 = r_fragments[1][0];
	
	cout<<"scaffold rotors: "<<scaff->rotor_lst->size()<<endl;
	cout<<"fragment rotors: "<<frag1->rotor_lst->size()<<endl;
	outfile << * scaff->molecule;
	outfile << * frag1->molecule;

	Atom* at1 = scaff->r_atom_lst.front().atm;
	Atom* at2 = frag1->group_atom;
	
	Bond* bnd = at1->createBond( *at2 );
	bnd->setOrder(1);
	
	cout<<"Connecting!"<<endl;
	mc.connect(at1, at2);

	// resolve the clashes:
	ConnectionResolver c_resolv;
	ConnectList rotors = *scaff->rotor_lst;
	rotors.splice(rotors.end(), ConnectList(*frag1->rotor_lst));
	
	c_resolv.setMolecule(*at1, *at2, rotors);
	
//	cout<<"Between fragments: "<<c_resolv.detectBetweenMolecules( *scaff->molecule, *frag1->molecule)<<endl;
//	cout<<"In Scaffold: "<<c_resolv.detectInMolecule( *scaff->molecule )<<endl;
//	cout<<"In fragment: "<<c_resolv.detectInMolecule( *frag1->molecule )<<endl;
	
	cout<<endl;
	c_resolv.resolve();
	cout<<endl;
	
	cout<<"Between fragments: "<<c_resolv.detectBetweenMolecules( *scaff->molecule, *frag1->molecule)<<endl;
	cout<<"In Scaffold: "<<c_resolv.detectInMolecule( *scaff->molecule )<<endl;
	cout<<"In fragment: "<<c_resolv.detectInMolecule( *frag1->molecule )<<endl;
	
	// insert new fragment into Molecule1:
	at1->getParent()->appendChild( at2->getRoot() );
	
	outfile<< *scaff->molecule;
	
	outfile.close();
	Log << "......done!"<<endl;
}

