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
#include <limits>

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
	
	Vector3& operator[](Index idx)
	{
		return positions[idx];
	}
	
	Vector3& get(Index idx)
	{
		return positions[idx];
	}

	const Size& size()
	{
		return _size;
	}
	
	/**
	 * Apply coordinates of this to the input BALL::AtomContainer
	 * @brief setCoordinates
	 * @param mol
	 */
	void transferCoordinates(AtomContainer* mol)
	{
		AtomIterator qit = mol->beginAtom();
		for (int i = 0 ; i < _size; i++, qit++)
		{
			qit->setPosition( (positions[i]) );
		}
	}

private:
	Size _size;
	Vector3 *positions;
};


// TODO: optimize the data strcuture, implement initialzier which sets map
//       and vector allocations to reasonable initial values
struct GroupFragment
{
	// intra connections
	vector< Fragment* > linker_lst; // all bonds within a linker are rotable
	vector< Fragment* > rigid_lst; // all matchable fragments
	list< Bond* > rotor_lst;      // all intra fragment bonds of the molecule
	list< pair< Atom*, Atom*> > frag_con2;
	// for each fragment, the list of bonds to other fragments
	boost::unordered_map< Fragment*, list<Bond*> > fragment_connections;
	
//	// alternative connections per fragment mapping (perhaps faster and more memory efficient than map?)
//	vector< Fragment* > fragment_idx;
//	vector< list<Bond*> > fragment_connections;
	
	// inter connections
	list< pair< unsigned int, Atom*> > connections;
	
	// data:
	Molecule* molecule;
};

/// Typedefs:

// Standard Data:
typedef vector<Atom*> AtmVec;

typedef vector<GroupFragment*> GroupFragmentList;
typedef boost::unordered_map< int, GroupFragmentList* > CombiLib;

// Lib Data:
typedef boost::unordered_map <String, TemplateCoord*> CoordinateMap;// String == UCK key
typedef boost::unordered_map <String, float >         BondLengthMap;
typedef boost::unordered_map <String, Fragment* >     ConnectionMap;

///// ################# H E L P E R    F U N C T I O N S #################

//// TODO: maybe change this
///// Delete empty fragments and check that all were empty
//void checkAndDeleteFragments(vector <Molecule*> frags)
//{
//	int empty = 0;
//	for (vector <Molecule*>::iterator i = frags.begin(); i != frags.end(); i++)
//	{
//		if( (*i)->countAtoms() == 0)
//			delete (*i);
//		else
//			empty++;
//	}
//	if( empty != 0 )
//		Log << "WARNING: There are still " << empty <<" unconnected fragments!"<<endl;
//}



//// get the position of an atom in the molcule list:
//const int getAtomPosition(Atom* atm, Molecule* mol)
//{
//	AtomIterator ati = mol->beginAtom();
//	for (int i = 0; +ati; ati++, i++)
//	{
//		if(&*ati == atm)
//			return i;
//	}
//	return -1;
//}


///// empty 'fromMol' and append the atoms to 'toMol'
//void transferMolecule( AtomContainer* toMol, AtomContainer* fromMol)
//{
//	int num_atm = fromMol->countAtoms();
//	for(int i = 0; i < num_atm; i++)
//		toMol->insert( *fromMol->beginAtom() ); // insert auto removes from its old parent
//}

#endif // BASIC_H
