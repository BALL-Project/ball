// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "../sources/ioModule.h"
#include "../sources/fragmenter.h"
#include "../sources/canonicalizer.h"

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
void writeMolVec(vector<AtomContainer*> &input, LineBasedFile* handle)
{
	for(int i = 0; i < input.size(); i++)
	{
		writePositionLines(*input[i], handle);
	}
}

// Write several result molecules to one file
void writeMolVec(vector<AtomContainer*> &input, SDFile* handle)
{
	for(int i = 0; i < input.size(); i++)
	{
		handle->write(*input[i]);
	}
}

// --unique keys Write several result molecules to one file
void uniqueWriteMolVec(vector<AtomContainer*> &input, LineBasedFile* handle, set< String >& used)
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
	
	// open input and output file:
	SDFile in_file(parpars.get("i"), ios::in);
	SDFile outfile(parpars.get("o"), ios::out);
	
	vector<AtomContainer*> fragments;
	vector<AtomContainer*> dummy;
	list< pair< Atom*, Atom*> > dummy2;
	Molecule* tmp;
	int molecule_cnt=0;
	int total_fragment_cnt=0;
	int unique_fragment_cnt=0;
	set< String > used; // used fragment keys
	
	
	MoleculeFragmenter molfrag;
	Canonicalizer canoni;
	tmp = in_file.read();
	
	// For each molecule:
	while ( tmp )
	{
		// get all rigid fragments from molecule 'tmp'
		molfrag.setMolecule( *tmp );
		molfrag.getMoleculeFragments(fragments, dummy, dummy2);
		
		total_fragment_cnt += fragments.size();
		
		// for each fragment in the molecule:
		vector<AtomContainer*>::iterator fit;
		for(fit = fragments.begin(); fit!=fragments.end(); fit++)
		{
			// canonicalise the atomlist & set UCK key
			canoni.canonicalize( **fit );
			(**fit).setProperty("key", Matcher::getUCK( **fit) );
		}
		
		// write to out file:
		if(parpars.has("unique"))
			uniqueWriteMolVec(fragments, &outfile, used);
		else
			writeMolVec(fragments, &outfile);
		
		// increase iteration:
		delete tmp;
		molecule_cnt++;
		tmp = in_file.read();
	}
	
	in_file.close();
	outfile.close();
	Log << "read "<< molecule_cnt<<" input structures, giving a total of "
			<< total_fragment_cnt <<" fragments and "<< unique_fragment_cnt
			<< "unique fragments"<<endl;
	
	Log << " wrote fragments to: " << parpars.get("o") << endl;
}
