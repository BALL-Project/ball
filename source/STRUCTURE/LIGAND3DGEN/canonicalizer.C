// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/LIGAND3DGEN/canonicalizer.h>

#include <BALL/STRUCTURE/molecularSimilarity.h>

#include <openbabel/mol.h>
#include <openbabel/canon.h>
#include <openbabel/graphsym.h>

#include <BALL/STRUCTURE/UCK.h>
#include <BALL/QSAR/aromaticityProcessor.h>

using namespace BALL;
using namespace std;

/// C l a s s   C a n o n i c a l i z e r
/// ############################################################################
Canonicalizer::Canonicalizer(){}

Canonicalizer::~Canonicalizer(){}

void Canonicalizer::canonicalize(AtomContainer& molecule)
{
	using namespace OpenBabel;
	
	// aromatize:
	AromaticityProcessor aroma;
	molecule.apply(aroma);
	
	OBMol* temp = MolecularSimilarity::createOBMol( molecule);
	int num_atoms = temp->NumAtoms();

	// get canonical labels:
	vector<unsigned int> sym;
	vector<unsigned int> clabels;

	OBGraphSym grsym(temp);
	grsym.GetSymmetry(sym);
	
	CanonicalLabels(temp, sym, clabels, OBBitVec(), 30, false);
	
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
	delete temp;
}



/// C l a s s   M a t c h e r
/// ############################################################################
Matcher::Matcher(RigidsMap &coord_map):_coord_lib(coord_map){}

Matcher::~Matcher(){}

void Matcher::matchFragment(AtomContainer& fragment)
{
	// for all fragments, match these against the lib:
	UCK keyGen( fragment, true, 5 );
	TemplateCoord* templat = _coord_lib[ keyGen.getUCK() ];
	
	if(templat && (templat->size() == fragment.countAtoms()) )
	{
		templat->applyCoordinates2Molecule( fragment );
	}
	else
	{
		throw Exception::FragmentTemplateNotFound("canonicalizer.C", 81, fragment);
	}
}

const String Matcher::getUCK(AtomContainer &mol)
{
	UCK keyGen( mol, true, 5 );
	return keyGen.getUCK();
}
