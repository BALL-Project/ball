// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "sources/ioModule.h"
#include "sources/fragmenter.h"

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/FORMAT/lineBasedFile.h>
//#include <BALL/SYSTEM/file.h>

#include <BALL/DATATYPE/string.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/STRUCTURE/connectedComponentsProcessor.h>
#include <BALL/STRUCTURE/UCK.h>
//#include <BALL/MATHS/vector3.h>
#include <BALL/KERNEL/forEach.h>
//#include <set>
#include <vector>

#include <openbabel/mol.h>
#include <openbabel/canon.h>
#include <openbabel/graphsym.h>

using namespace OpenBabel;
using namespace BALL;
using namespace std;

/// ################# H E L P E R    F U N C T I O N S #################
void writePositionLines(AtomContainer& mol, LineBasedFile* handle)
{
	(*handle) <<"key "<< mol.getProperty("key").getString() <<endl;
	(*handle) << String(mol.countAtoms()) << endl;
	
	AtomIterator ati = mol.beginAtom();
	for(; +ati; ati++)
	{
		(*handle) << String(ati->getPosition().x) << " ";
		(*handle) << String(ati->getPosition().y) << " ";
		(*handle) << String(ati->getPosition().z) << endl;
	}
}

// Write several result molecules to one file
void writeMolVec(vector<Fragment*> &input, LineBasedFile* handle)
{
	for(int i = 0; i < input.size(); i++)
	{
		writePositionLines(*input[i], handle);
	}
}

// Write several result molecules to one file
void writeMolVec(vector<Fragment*> &input, SDFile* handle)
{
	for(int i = 0; i < input.size(); i++)
	{
		handle->write(*input[i]);
	}
}

// --unique keys Write several result molecules to one file
void uniqueWriteMolVec(vector<Fragment*> &input, LineBasedFile* handle, set< String >& used)
{
	for(int i = 0; i < input.size(); i++)
	{
		// check that the key was not already used:
		String key = input[i]->getProperty("key").getString();
		if( used.find(key) == used.end() )
		{
			writePositionLines(*input[i], handle);
			used.insert(key);
		}
	}
}

// --unique keys Write several result molecules to one file
void uniqueWriteMolVec(vector<Fragment*> &input, SDFile* handle, set< String >& used)
{
	for(int i = 0; i < input.size(); i++)
	{
		// check that the key was not already used:
		String key = input[i]->getProperty("key").getString();
		if( used.find(key) == used.end() )
		{
			writePositionLines(*input[i], handle);
			used.insert(key);
		}
	}
}
/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("libFragmenter", "cut a molecule along its rotable bonds, generating fragments", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input SDF", INFILE, true);
	parpars.registerParameter("o", "output SDF", OUTFILE, true);
	
	parpars.registerFlag("unique", "only output one fragment for each topology");

	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");

	String manual = "For creating a fragment library.";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
	// START of CODE#############################################################
	
	SDFile in_file(parpars.get("i"), ios::in);
	
	// open output file:
	SDFile outfile(parpars.get("o"), ios::out);
	
	vector<Fragment*> fragments;
	vector<Fragment*> dummy;
	list< pair< Atom*, Atom*> > dummy2;
	Molecule* tmp;
	int cntr=0;
	set< String > used; // used fragment keys
	
	// Read all molecules.
	AtomIterator ati;
	Atom::BondIterator bit;
	MoleculeFragmenter molfrag;
	tmp = in_file.read();
	while ( tmp )
	{
		// get all rigid fragments from molecule 'tmp'
		molfrag.setMolecule( *tmp );
		molfrag.getMoleculeFragments(fragments, dummy, dummy2);
		
///----------------------Canonicalise Fragments---------------------------
/// CANONICAL:
		vector<Fragment*>::iterator fit;
		for(fit = fragments.begin(); fit!=fragments.end(); fit++)
		{
			// get babel mol:
			OBMol* temp = MolecularSimilarity::createOBMol(**fit, true);
			
			// get helper for canonicalisation:
			OBGraphSym grsym(temp);
			vector<unsigned int> sym;
			grsym.GetSymmetry(sym);
			
			// get mapping for canonical labels:
			vector<unsigned int> clabels;
			CanonicalLabels(temp, sym, clabels, OBBitVec(), 60);
			
			Fragment* new_mol = new Fragment;
			vector <Atom*> aList( (**fit).countAtoms() );
			for(int i=0; i<clabels.size(); i++)
			{
				aList[clabels[i]-1]=( (**fit).getAtom(i) );
			}

			for(int i=0; i<clabels.size(); i++)
				new_mol->append(*aList[i]);
			
			(**fit).swap(*new_mol);
			
/// UCK Keys:
			UCK keyGen(**fit, true, 5);
			
			(**fit).setProperty("key", keyGen.getUCK());
		}
		
/// write to output-------------------------------------------------------------
		
		if(parpars.has("unique"))
			uniqueWriteMolVec(fragments, &outfile, used);
		else
			writeMolVec(fragments, &outfile);
		
		// next iteration work:
		delete tmp;
		cntr++;
		tmp = in_file.read();
	} /// ------end while
	
	in_file.close();
	outfile.close();
	Log << "read "<< cntr<<" input structures, wrote fragments to: " << parpars.get("o") << endl;
}
