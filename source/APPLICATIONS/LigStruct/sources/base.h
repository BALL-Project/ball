// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef LIGANDSTRUCTUREBASE_H
#define LIGANDSTRUCTUREBASE_H

/// BALL: File IO
//#include <BALL/FORMAT/SDFile.h>
//#include <BALL/FORMAT/lineBasedFile.h>
//#include <BALL/SYSTEM/file.h>

/// BALL: Macros
//#include <BALL/KERNEL/forEach.h>

/// BALL: Base Classes (String + Math)
#include <BALL/DATATYPE/string.h>
//#include <BALL/MATHS/angle.h>
//#include <BALL/MATHS/vector3.h>
//#include <BALL/MATHS/matrix44.h>

/// BALL: Atom, Bond, Element
#include <BALL/KERNEL/atom.h>
//#include <BALL/KERNEL/atomIterator.h>
//#include <BALL/KERNEL/bond.h>
//#include <BALL/KERNEL/bondIterator.h>
//#include <BALL/KERNEL/PTE.h>

/// BALL: Molecule Container
#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/atomContainer.h>
//#include <BALL/KERNEL/fragment.h>
//#include <BALL/KERNEL/molecule.h>

/// BALL: Algorithms
//#include <BALL/STRUCTURE/UCK.h>
//#include <BALL/STRUCTURE/molecularSimilarity.h>
//#include <BALL/STRUCTURE/structureMapper.h>
//#include <BALL/STRUCTURE/geometricTransformations.h>

/// Open Babel
//#include <openbabel/obconversion.h>
//#include <openbabel/mol.h>
//#include <openbabel/canon.h>
//#include <openbabel/graphsym.h>

/// STL
#include <vector>
//#include <limits>

/// BOOST
#include <boost/unordered_map.hpp>
//#include <boost/pending/disjoint_sets.hpp>

///// Name Spaces
//using namespace OpenBabel;
using namespace BALL;
using namespace std;


/// C l a s s   T e m p l a t e C o o r d
/// ############################################################################
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

	TemplateCoord(AtomContainer& mol)
	{
		_size = mol.countAtoms();
		positions = new Vector3[_size];
		setCoordinates( mol );
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
	void transferCoordinates(AtomContainer& mol)
	{
		AtomIterator qit = mol.beginAtom();
		for (int i = 0 ; i < _size; i++, qit++)
		{
			qit->setPosition( positions[i] );
		}
	}
	
	void setCoordinates(AtomContainer& mol)
	{
		AtomIterator qit = mol.beginAtom();
		for (int i = 0 ; i < _size; i++, qit++)
		{
			positions[i] = qit->getPosition();
		}
	}

private:
	Size _size;
	Vector3 *positions;
};

/// C l a s s   G r o u p F r a g m e n t
/// ############################################################################
struct GroupFragment
{
	// intra connections
	vector< Fragment* > linker_lst; // all bonds within a linker are rotable
	vector< Fragment* > rigid_lst; // all matchable fragments
	list< pair< Atom*, Atom*>  > rotor_lst;      // all intra fragment bonds of the molecule
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

/// T y p e   D e f i n i t i o n s
/// ############################################################################

// Standard Data:
typedef vector<Atom*>                     AtmVec;
typedef vector<Atom*>::iterator           AVIter;
typedef vector <AtomContainer*>           ACVec;
typedef vector <AtomContainer*>::iterator ACVecIter;

// Special Data:
typedef list< pair<Atom*, Atom*> > ConnectList;
typedef vector<GroupFragment*>     GroupFragVec;
typedef vector< String >           SMILESVec;

// Lib Data:
typedef  boost::unordered_map < int, SMILESVec > CombiLibMap;
//typedef boost::unordered_map< int, GroupFragVec* >    CombiLib; // key==group number, value==all groupfragment for that group
typedef boost::unordered_map <String, TemplateCoord*> CoordinateMap;// key == UCK key
typedef boost::unordered_map <String, float >         BondLengthMap;
typedef boost::unordered_map <String, vector<float> > TorsionMap;
typedef boost::unordered_map <String, Fragment* >     ConSiteMap;


/// C l a s s   L i g B a s e
/// ############################################################################
class LigBase
{
public:
	
	// generate a mini dump of a molecule
	static String printInlineMol(Composite* mol);
	static String printMol(Composite* mol);
	
	static String printInlineStarMol(Composite* mol);
	static String printInlineStarMol(AtmVec& mol);
	
	static int countBondsAndOrder(Atom& atm);
	static int countBondsInPartent(Atom& atm, const Composite &parent);
	
	// get the position of an atom in the molcule list:
	static const int getAtomPosition(Atom* atm, AtomContainer *mol);
	
	// Translate the AtomContainer 'fromMol' into an AtmVec 'toMol'
	static void toAtmVec( AtomContainer& fromMol, AtmVec& toMol);
	
	static void transferMolecule( AtomContainer* toMol, AtomContainer* fromMol);
	
	static void clearExternalBonds(AtomContainer* mol);
	
	static void copyMoleculeProperies(AtomContainer &orig, AtomContainer &cop);

	// Remove Hydrogens
	static void removeHydrogens(AtomContainer &tmp );
};

#endif // LIGANDSTRUCTUREBASE_H
