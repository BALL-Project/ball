// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef LIGAND3DBASE_H
#define LIGAND3DBASE_H

#include <BALL/DATATYPE/string.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/KERNEL/global.h>
#include <BALL/COMMON/exception.h>

#include <openbabel/obconversion.h>

#include <vector>
#include <list>

#include <boost/unordered_map.hpp>

namespace BALL 
{

class RigidFragmentDB;

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
// key==group number, value==all groupfragment for that group
typedef std::vector< std::vector<RFragment*> > CombiLibMap; 

//// key == UCK key
//typedef boost::unordered_map <BALL::String, TemplateCoord*> RigidsMap;

////typedef boost::unordered_map <BALL::String, float >         BondLengthMap;
//typedef boost::unordered_map <BALL::String, BALL::AtomContainer*> SiteMap;


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
	std::vector<BALL::Vector3> positions;
};

/// S t r u c t   R - A t o m
/// ############################################################################
class RFragment;

struct RAtom
{
	int id;
	BALL::Atom* atm;
	RFragment* parent;
	std::map< std::pair<int ,RFragment*>, int > known_partners;
	
	void addParnter(RFragment& other);
	int getCompatibleSet(RFragment& other);
};


/// C l a s s   R F r a g m e n t
/// ############################################################################
class RFragment
{
public:
	RFragment();
	~RFragment();
	/**
	 * Clone RFragment, manually clones the molecule
	 */
	RFragment(const RFragment& other);
	
	/// F I E L D S:
	BALL::Size size;
	int                group_id;   // to which r-group this r-fragment belongs
	BALL::Atom*        group_atom; // (donor) connection
	std::list< RAtom > r_atom_lst; // (acceptor) connections
	
	BALL::AtomContainer* molecule; // the actual molecule
	ConnectList*         rotor_lst; // all intra rotor bonds of this RFragment
	
	int                           curr_set;// coordinate set that is currently used
	std::vector< TemplateCoord* > coord_sets; // alternate positions/ conformations
	
	void setCoordsTo(const int& set);
	void newSetFromCurrent();
	
private:
	RAtom const* _isRAtom( const std::list< RAtom >& ilist, BALL::Atom* atom);
};

/// S t a t i c   C l a s s   L i g B a s e
/// ############################################################################
class LigBase
{
public:
	
	static bool containsUnknownElement( BALL::AtomContainer& ac );
	
	static bool containsHydrogen( BALL::AtomContainer& ac );
	
	static void removeHydrogens( BALL::AtomContainer &tmp );
	
	// generate a mini dump of a molecule
	static BALL::String printInlineMol( BALL::Composite* mol); //#DEBUG: for debugging
	static BALL::String printMol( BALL::Composite* mol);//#DEBUG: for debugging
	
	static BALL::String printInlineStarMol( BALL::Composite* mol);//#DEBUG: for debugging
	static BALL::String printInlineStarMol(AtmVec& mol);//#DEBUG: for debugging
	
	static BALL::String moleculeToSMILES( BALL::AtomContainer& ac );
	
	static int countBondsAndOrder( BALL::Atom& atm);
	static int countBondsInParent( BALL::Atom& atm, const BALL::Composite &parent);
	
	/**
	 * @brief getAtomPosition - get the position of an atom in the molcule list
	 * @param atm
	 * @param mol
	 * @return 
	 */
	static const int getAtomPosition( BALL::Atom* atm, BALL::AtomContainer *mol);
	
	/**
	 * @brief toAtmVec - Translate the AtomContainer 'fromMol' into an AtmVec '
	 * toMol'
	 * @param fromMol
	 * @param toMol
	 */
	static void toAtmVec( BALL::AtomContainer& fromMol, AtmVec& toMol);
	
	static void transferMolecule( BALL::AtomContainer* toMol, BALL::AtomContainer* fromMol);
	
	static void clearExternalBonds( BALL::AtomContainer* mol);
};


/// L i g a n d 3 D   E x c e p t i o n s
/// ############################################################################
namespace Exception
{
/// Exception : SiteTemplateNotFound #######################################
class BALL_EXPORT SiteTemplateNotFound
		: public GeneralException
{
public:
	SiteTemplateNotFound(const char* file, int line, String& key)
		: GeneralException(file, line, "SiteTemplateNotFound", "")
	{
		message_ = "no template could be found for the site key: ";
		message_ += key;
		
		globalHandler.setMessage(message_);
	}
};

/// Exception : FragmentTemplateNotFound ###################################
class BALL_EXPORT FragmentTemplateNotFound
		: public GeneralException
{
public:
	FragmentTemplateNotFound(const char* file, int line, AtomContainer& frag)
		: GeneralException(file, line, "FragmentTemplateNotFound", "")
	{
		message_ = "no template could be found for the fragment: ";
		message_ += LigBase::moleculeToSMILES( frag );
		
		globalHandler.setMessage(message_);
	}
};

/// Exception : RotationAxisInRing #########################################
class BALL_EXPORT RotationAxisInRing
		: public GeneralException
{
public:
	RotationAxisInRing(const char* file, int line, Atom& atm)
		: GeneralException(file, line, "RotationAxisInRing", "")
	{
		message_ = "attempted to rotate a bond within a ring. Molecule part was: \n";
		message_ += LigBase::moleculeToSMILES( *(AtomContainer*)&atm.getRoot() );
		
		globalHandler.setMessage(message_);
	}
};

/// Exception : StructureNotGenerated ######################################
class BALL_EXPORT StructureNotGenerated
		: public GeneralException
{
public:
	StructureNotGenerated(const char* file, int line, AtomContainer& mol, const GeneralException& in_exc)
		: GeneralException(file, line, "StructureNotGenerated", "")
	{
		message_ = " No 3D coordinates could be generated for molecule: \n";
		
		message_ += LigBase::moleculeToSMILES(mol);
		message_ += "\n\n";
		
		message_ += "CAUSE:\n";
		message_ += in_exc.getName() + String(" in line ")+String(in_exc.getLine()) 
				+ String(" of ") + in_exc.getFile() + String("\n");
		message_ += String("Message: ") + in_exc.getMessage() + String("\n");
		
		globalHandler.setMessage(message_);
	}
	
	StructureNotGenerated(const char* file, int line, AtomContainer& mol, const String& cause)
		: GeneralException(file, line, "StructureNotGenerated", "")
	{
		message_ = " No 3D coordinates could be generated for molecule: \n";
		
		message_ += LigBase::moleculeToSMILES(mol);
		message_ += "\n\n";
		
		message_ += "CAUSE:\n";
		message_ += cause;
		
		globalHandler.setMessage(message_);
	}
	
	StructureNotGenerated(const char* file, int line, AtomContainer& mol)
		: GeneralException(file, line, "StructureNotGenerated", "")
	{
		message_ = " No 3D coordinates could be generated for molecule: \n";
		
		message_ += LigBase::moleculeToSMILES(mol);
		message_ += "\n\n";
		
		message_ += "CAUSE: UNKNOWN";
		
		globalHandler.setMessage(message_);
	}
	
private:
	void canSMILESFromAtomContainer(AtomContainer& ac, String& can_smiles)
	{
		OpenBabel::OBMol* obmol = MolecularSimilarity::createOBMol(ac, true);
		
		OpenBabel::OBConversion conv;
		conv.SetOutFormat("can"); // canonical smiles
		can_smiles = conv.WriteString(obmol);
		
		// remove the stupid ID that openbabel always attaches to the generated canonical smile
		can_smiles = can_smiles.substr(0,can_smiles.find_first_of('\t'));
	}
};

}// End Namespace "Exception"


/// R i g i d F r a g m e n t s D a t a b a s e
/// ############################################################################
class RigidFragmentDB:
		public boost::unordered_map <BALL::String, TemplateCoord*>
{
public:
	RigidFragmentDB();
	
	RigidFragmentDB(const BALL::String& filename);
	
	~RigidFragmentDB();
	
private:
	BALL::String _filename;
};

/// S i t e F r a g m e n t s D a t a b a s e
/// ############################################################################
class SiteFragmentDB:
		public boost::unordered_map <BALL::String, BALL::AtomContainer*>
{
public:
	SiteFragmentDB();
	
	SiteFragmentDB(const BALL::String& filename);
	
	~SiteFragmentDB();
private:
	BALL::String _filename;
};

/// C l a s s   M a t c h e r
/// ############################################################################
class Matcher
{
public:
	Matcher( RigidFragmentDB& coord_map );
	~Matcher();
	
	void matchFragment(BALL::AtomContainer &fragment);
	
	static const BALL::String getUCK(BALL::AtomContainer & mol);
	
private:
	RigidFragmentDB& _coord_lib;
};
}// End Namespace "BALL"
#endif // LIGAND3DBASE_H
