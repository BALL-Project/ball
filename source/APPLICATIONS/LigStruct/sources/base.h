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
using namespace BALL;
using namespace std;

struct RFragment;
class TemplateCoord;

/// T y p e   D e f i n i t i o n s
/// ############################################################################

// Standard Data:
typedef vector<Atom*>                     AtmVec;
typedef vector<Atom*>::iterator           AVIter;
typedef vector <AtomContainer*>           ACVec;
typedef vector <AtomContainer*>::iterator ACVecIter;

// Special Data:
typedef list< pair<Atom*, Atom*> > ConnectList;

// Database types:
typedef vector< vector<RFragment*> > CombiLibMap; // key==group number, value==all groupfragment for that group
typedef boost::unordered_map <String, TemplateCoord*> RigidsMap;// key == UCK key
typedef boost::unordered_map <String, float >         BondLengthMap;
typedef boost::unordered_map <String, AtomContainer*> SiteMap;


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

/// S t r u c t   G r o u p F r a g m e n t
/// ############################################################################
struct RAtom
{
	int id;
	bool done;
	Atom* atm;
};

class RFragment
{
public:
	RFragment()
	{
		this->group_atom = 0;
		this->group_id   = 0;
		this->molecule   = 0;
	}
	
	/*
	 * Clone RFragment, manually clones the molecule
	 */
	RFragment(const RFragment& other)
	{
		this->molecule = new AtomContainer();
		
		// clone atoms:
		RAtom const * tmp_r = 0;
		for(AtomIterator ati = other.molecule->beginAtom(); +ati; ++ati)
		{
			Atom* tmp_at = new Atom( *ati );
			this->molecule->insert( *tmp_at );
			
			// check what kind of atom we are currently coping
			// case group atom: transfer the group atom:
			if( &*ati == other.group_atom)
			{
				this->group_atom = tmp_at;
			}
			// else check for r-atom
			else
			{
				tmp_r = _isRAtom( other.r_atom_lst, &*ati);
				if( tmp_r )
				{
					RAtom new_r;
					new_r.atm = tmp_at;
					new_r.id  = tmp_r->id;
					new_r.done= tmp_r->done;
					
					this->r_atom_lst.push_back( new_r );
				}
			}
		}
		
		// clone bonds
		BALL::cloneBonds(*other.molecule, *this->molecule);
		
		group_id = other.group_id;
		rotor_lst = other.rotor_lst;
	}
	
	/// F I E L D S:
	int group_id; //id 0 identifies the scaffold
	AtomContainer* molecule;
	
	ConnectList rotor_lst; // all intra rotor bonds of this RFragment
	
	// inter connections
	Atom* group_atom;
	list< RAtom > r_atom_lst;
	
//	vector< TemplateCoord > coord_set;

private:
	RAtom const* _isRAtom( const list< RAtom >& ilist, Atom* atom)
	{
		list< RAtom >::const_iterator it = ilist.begin();
		for(; it != ilist.end(); ++it)
		{
			if( (*it).atm == atom )
				return &*it;
		}
		
		return 0;
	}
};


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
