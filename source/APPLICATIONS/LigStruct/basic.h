// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef BASIC_H
#define BASIC_H

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/SYSTEM/file.h>

#include <BALL/KERNEL/forEach.h>
#include <BALL/DATATYPE/string.h>

#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/STRUCTURE/UCK.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/MATHS/angle.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/matrix44.h>

#include <openbabel/obconversion.h>
#include <openbabel/mol.h>
#include <openbabel/canon.h>
#include <openbabel/graphsym.h>

#include <vector>

#include <boost/unordered_map.hpp>
#include <boost/pending/disjoint_sets.hpp>


using namespace OpenBabel;
using namespace BALL;
using namespace std;

/// ################# H E L P E R    T Y P E #################

/**
 * @brief The TemplateCoord class is a simple array wrapper for BALL::Vector3
 * 
 * To efficiently save coordinates without much overhead
 */
class TemplateCoord
{
	
public:
	TemplateCoord(Size n)
	{
		_size = n;
		positions = new Vector3[n];
	}
	
	~TemplateCoord()
	{
		delete[] positions;
	}
	
	Vector3& operator[](Size idx)
	{
		return *(positions + idx);
	}

	const Size& size()
	{
		return _size;
	}

private:
	Size _size;
	Vector3 *positions;
};

struct PredictorConfig
{
	String fragment_lib_path;
	String bondlenth_lib_path;
	String connection_lib_path;
};

struct GroupFragment
{
	list< Bond* > rotor_lst;
	list< pair< unsigned int, Atom*> > connections;
	Molecule* molecule;
};

/// Typedefs:
typedef vector<GroupFragment*> GroupFragmentList;
typedef boost::unordered_map< int, GroupFragmentList* > CombiLib;


/// ################# H E L P E R    F U N C T I O N S #################

// TODO: maybe change this
/// Delete empty fragments and check that all were empty
void checkAndDeleteFragments(vector <Molecule*> frags)
{
	int empty = 0;
	for (vector <Molecule*>::iterator i = frags.begin(); i != frags.end(); i++)
	{
		if( (*i)->countAtoms() == 0)
			delete (*i);
		else
			empty++;
	}
	if( empty != 0 )
		Log << "WARNING: There are still " << empty <<" unconnected fragments!"<<endl;
}



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


/// empty 'fromMol' and append the atoms to 'toMol'
void transferMolecule( Molecule* toMol, Molecule* fromMol)
{
	int num_atm = fromMol->countAtoms();
	for(int i = 0; i < num_atm; i++)
		toMol->insert( *fromMol->beginAtom() ); // insert auto removes from its old parent
}


/// check if the atom is a rigid one:
bool isAtomRigid(OBAtom* atm)
{
/// TODO: add OBRotorList object to use custom torlib!
	if (atm->IsInRing())
		return true;
	else
	{
		OBBondIterator b_it = atm->BeginBonds();
		for(; b_it!=atm->EndBonds(); b_it++)
		{
			if( ! (*b_it)->IsRotor() )
				return true;
		}
		return false;
	}
}

// cut bonds that are shared with atoms from other fragments:
void clearExternalBonds(Molecule* mol)
{
	Atom::BondIterator bit;
	AtomIterator ait;

	BALL_FOREACH_INTERBOND(*mol, ait, bit)
	{
		bit->destroy();
	}
}

void setCoordinates(Molecule* query, TemplateCoord* templat)
{
	AtomIterator qit = query->beginAtom();
	for (int i = 0 ; i < templat->size(); i++, qit++)
	{
		qit->setPosition( (*templat)[i]);
	}
}

#endif // BASIC_H








