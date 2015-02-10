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
#include <BALL/KERNEL/global.h>

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
//using namespace std;
//using namespace BALL;

struct RFragment;
class TemplateCoord;

/// T y p e   D e f i n i t i o n s
/// ############################################################################

// Standard Data:
typedef std::vector<BALL::Atom*>                     AtmVec;
typedef std::vector<BALL::Atom*>::iterator           AVIter;
typedef std::vector <BALL::AtomContainer*>           ACVec;
typedef std::vector <BALL::AtomContainer*>::iterator ACVecIter;

// Special Data:
typedef std::list< std::pair<BALL::Atom*, BALL::Atom*> > ConnectList;

// Database types:
typedef std::vector< std::vector<RFragment*> > CombiLibMap; // key==group number, value==all groupfragment for that group
typedef boost::unordered_map <BALL::String, TemplateCoord*> RigidsMap;// key == UCK key
typedef boost::unordered_map <BALL::String, float >         BondLengthMap;
typedef boost::unordered_map <BALL::String, BALL::AtomContainer*> SiteMap;


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
	TemplateCoord(BALL::Size n);

	TemplateCoord( BALL::AtomContainer& mol);
	
	~TemplateCoord();
	
	BALL::Vector3& operator[]( BALL::Index idx);
	
	BALL::Vector3& get( BALL::Index idx);

	const BALL::Size& size();
	
	/**
	 * Apply coordinates of this to the input BALL::AtomContainer
	 * @brief setCoordinates
	 * @param mol
	 */
	void applyCoordinates2Molecule( BALL::AtomContainer& mol);
	
	void readCoordinatesFromMolecule( BALL::AtomContainer& mol);

private:
	BALL::Size _size;
	BALL::Vector3 *positions;
};

/// S t r u c t   R - A t o m
/// ############################################################################
struct RAtom
{
	int id;
	bool done;
	BALL::Atom* atm;
};


/// C l a s s   R F r a g m e n t
/// ############################################################################
class RFragment
{
public:
	RFragment();
	
	/**
	 * Clone RFragment, manually clones the molecule
	 */
	RFragment(const RFragment& other);
	
	/// F I E L D S:
	int                  group_id; //id 0 identifies the scaffold
	BALL::AtomContainer* molecule;
	ConnectList          rotor_lst; // all intra rotor bonds of this RFragment
	BALL::Atom*          group_atom; // (donor) connection
	std::list< RAtom >   r_atom_lst; // (acceptor) connections
	
	std::vector< TemplateCoord > coord_set; // alternate positions/ conformations

private:
	RAtom const* _isRAtom( const std::list< RAtom >& ilist, BALL::Atom* atom);
};

/// C l a s s   C o m b i n a t i o n
/// ############################################################################
class Combination
{
public:
	
	std::list< RAtom > r_list;
private:
	
};

/// C l a s s   L i g B a s e
/// ############################################################################
class LigBase
{
public:
	
	// generate a mini dump of a molecule
	static BALL::String printInlineMol( BALL::Composite* mol);
	static BALL::String printMol( BALL::Composite* mol);

	static BALL::String printInlineStarMol( BALL::Composite* mol);
	static BALL::String printInlineStarMol(AtmVec& mol);
	
	static int countBondsAndOrder( BALL::Atom& atm);
	static int countBondsInPartent( BALL::Atom& atm, const BALL::Composite &parent);
	
	// get the position of an atom in the molcule list:
	static const int getAtomPosition( BALL::Atom* atm, BALL::AtomContainer *mol);
	
	// Translate the AtomContainer 'fromMol' into an AtmVec 'toMol'
	static void toAtmVec( BALL::AtomContainer& fromMol, AtmVec& toMol);
	
	static void transferMolecule( BALL::AtomContainer* toMol, BALL::AtomContainer* fromMol);
	
	static void clearExternalBonds( BALL::AtomContainer* mol);
	
	static void copyMoleculeProperies( BALL::AtomContainer &orig, BALL::AtomContainer &cop);

	// Remove Hydrogens
	static void removeHydrogens( BALL::AtomContainer &tmp );
};

#endif // LIGANDSTRUCTUREBASE_H
