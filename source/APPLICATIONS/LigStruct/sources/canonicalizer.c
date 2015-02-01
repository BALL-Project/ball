// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "canonicalizer.h"

#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/KERNEL/atomContainer.h>

#include <openbabel/mol.h>
#include <openbabel/canon.h>
#include <openbabel/graphsym.h>

#include <BALL/STRUCTURE/UCK.h>



/// C l a s s   C a n o n i c a l i z e r
/// ############################################################################
Canonicalizer::Canonicalizer(){}

Canonicalizer::~Canonicalizer(){}

void Canonicalizer::canonicalize(AtomContainer& molecule)
{
	using namespace OpenBabel;
	
	int num_atoms = molecule.countAtoms();

	OBMol* temp = MolecularSimilarity::createOBMol( molecule, true);
	
	// get canonical labels:
	OBGraphSym grsym(temp);
	std::vector<unsigned int> sym;
	grsym.GetSymmetry(sym);
	
	vector<unsigned int> clabels;
	CanonicalLabels(temp, sym, clabels);
	
	// resort atom list by inserting into a new empty molecule
	AtomContainer* new_frag = new AtomContainer;
	vector <Atom*> aList(num_atoms);
	for( int i = 0; i < clabels.size(); i++)
		aList[ clabels[i]-1 ] = molecule.getAtom(i);
	
	// insert in correct order into temporary atomContainer
	for( int i = 0; i < clabels.size(); i++)
		new_frag->append( *aList[i] );

	// swap the temp with the original and delete the temp
	molecule.swap(*new_frag);
	delete new_frag;
}



/// C l a s s   M a t c h e r
/// ############################################################################
Matcher::Matcher(CoordinateMap &coord_map):_coord_lib(coord_map){}

Matcher::~Matcher(){}

void Matcher::matchFragment(AtomContainer& fragment)
{
	// for all fragments, match these against the lib:
	UCK keyGen( fragment, true, 5 );
	TemplateCoord* templat = _coord_lib[ keyGen.getUCK() ];
		
	if(templat && (templat->size() == fragment.countAtoms()) )
	{
		templat->transferCoordinates( fragment );
	}
	else // print error msg, showing element-list and key of the not matchable molecule
	{
		cout<<"ERROR: could not find a template for: ";
		cout<< LigBase::printInlineMol(&fragment)<<endl;
		cout<< "It's UCK-key: "<<keyGen.getUCK()<<endl;
		exit(EXIT_FAILURE);
	}
}

const String Matcher::getUCK(AtomContainer &mol)
{
	UCK keyGen( mol, true, 5 );
	return keyGen.getUCK();
}
