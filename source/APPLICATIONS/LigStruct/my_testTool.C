// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

//#include "sources/structureAssembler.h"
#include "sources/moleculeConnector.h"
#include "sources/ioModule.h"

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>

#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/PTE.h>

//using namespace OpenBabel;
using namespace BALL;
using namespace std;

// get the position of an atom in the molcule list:
const int getAtomPosition(Atom* atm, Molecule* mol)
{
	AtomIterator ati = mol->beginAtom();
	for (int i = 0; +ati; ati++, i++)
	{
		if(&*ati == atm)
			return i;
	}
	return -1;
}

///################################# M A I N ###################################
///#############################################################################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("attachFragments", " connect two fragments", 0.1, String(__DATE__), "Assembly");
	parpars.registerParameter("i", "input SDF, with deuterium connected to the connection site", INFILE, true);
	parpars.registerParameter("o", "output of all possible connection results", OUTFILE, true);
	parpars.registerParameter("l", "library configuration file", INFILE, false);
	
	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");
	parpars.setSupportedFormats("l","conf");
	parpars.setOutputFormatSource("i","o");

	parpars.setToolManual("Test of fragment connection functionality.");

	parpars.parse(argc, argv);
	
///====================== actual testing: =========================
	
	// Load all template libs:
	IOModule lib_reader;
	if ( parpars.has("l") )
	{
		lib_reader.libraryPathesFromConfig( parpars.get("l") );
	}
	else
	{
		String pth = "/Users/pbrach/files/projects/Master-2014_2015/1_code/ball_ligandStruct/source/APPLICATIONS/LigStruct/examples/libraries.conf";
		lib_reader.libraryPathesFromConfig( pth );
	}
	
	lib_reader.readBondLib();
	lib_reader.readConnectionLib();
	
	// read all test cases:
	vector< Molecule* > test_cases;
	vector< int > connection_atoms;
	Molecule* tmp;
	SDFile molecules_file(parpars.get("i"), ios::in);
	
	tmp = molecules_file.read();
	while(tmp)
	{
		Atom* mark_atm;
		for(AtomIterator ati = tmp->beginAtom(); +ati; ++ati)
		{
			if( ati->getElement().getSymbol() == "?")
			{
				mark_atm = &*ati;
				break;
			}
		}
		Atom* con_atm = mark_atm->beginBond()->getBoundAtom(*mark_atm);
		int i = getAtomPosition(con_atm, tmp);
				
		connection_atoms.push_back(i); // remember connection atom
//		con_atm->getBond(mark_atm)->destroy(); // remove bond to marker
		tmp->remove(*mark_atm);               // remove marker
		test_cases.push_back(tmp);           // remember the test molecule
		
		tmp = molecules_file.read(); // loop iteration
	}
	molecules_file.close();
	cout<<"done initializing and reading input"<<endl;

///===========find connection site and match against conection library=========
	SDFile out_file(parpars.get("o"), ios::out);
	MoleculeConnector mcon;
	mcon.setLibs(lib_reader.connect_lib, lib_reader.bond_lib);
	
	// connect: each test case with itself and all other test cases
	for(int i=0; i < connection_atoms.size(); ++i)
	{
		for(int k=0; k < connection_atoms.size(); ++k)
		{
			cout<<"i: "<<i<<" "<<test_cases[i]<<", k: "<<k<<" "<<test_cases[k]<<endl;
			AtomContainer* tmp1 = new AtomContainer( (*(AtomContainer*)(test_cases[i]) ) );
			AtomContainer* tmp2 = new AtomContainer( (*(AtomContainer*)(test_cases[k]) ) );
			
			
			Atom* atm1 = tmp1->getAtom(connection_atoms[i]);
			Atom* atm2 = tmp2->getAtom(connection_atoms[k]);
			cout<<atm1<<" "<<atm2<<endl;
			// connect on datatype level:
			Bond* bnd = new Bond();
			bnd->setOrder(1);
			atm1->createBond(*bnd, *atm2);
			
			///CONNECT STUFF HERE
			cout<<"connecting 2 fragments"<<endl;
			mcon.connect(atm1, atm2);
			
			// merge atoms:
			tmp1->insert(*tmp2);
			
			/// WRITE RESULT TO out_file
			out_file << *tmp1;
			cout<<"wrote structure"<<endl;
			
			delete tmp1;
//			delete tmp2;
		}
	}
	out_file.close();
}
