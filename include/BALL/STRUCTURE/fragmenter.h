// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef LIGAND_FRAGMENTER_H
#define LIGAND_FRAGMENTER_H

#include <BALL/STRUCTURE/LIGAND3DGEN/ligand3DBase.h>

#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>

namespace BALL 
{

/**
 * @brief The MoleculeFragmenter class - handles fragmenting and assignment of
 * rigidness/flexibility according to topology. Theoretically this violates
 * the single responsibility principle. In future the assignment should be
 * handled by a separate class.
 */
class MoleculeFragmenter
{
public:

	/**
	 * @brief MoleculeFragmenter initializes to an empty fragmenter with no
	 * molecule. The all public fragmenter methods may only be used after
	 * the setMolecule() method was used to specify the internal molecule
	 * which is to be fragmented.
	 *
	 * Also: the methods isRigidAtom(), isRotableBond() and isBridgingBond()
	 * may only be called with atoms or bonds of the molecule that was set.
	 */
	MoleculeFragmenter();
	~MoleculeFragmenter();
	
	/**
	 * @brief setMolecule - sets the internally used molecule to the given input
	 * molecule
	 * @param in_mol
	 */
	void setMolecule(BALL::AtomContainer& in_mol);
	
	/**
	 * @brief isRigidAtom - checks wether the atom belongs to the rigid atoms.
	 *
	 * These are: atoms that are part of any(!) ring, terminal atoms that are
	 * connected to rigid atoms and atoms that form a bond with bond order > 1.
	 *
	 * @param atm
	 * @return
	 */
	bool isRigidAtom(BALL::Atom &atm);
	
	/**
	 * @brief isRotableBond - is the bond rotatable?
	 *
	 * Such bonds are defined by this method as all bonds that are NOT:
	 * single bonds (bond order ==1), or a ring bond, or are terminal bonds
	 * (to a terminal atom)
	 *
	 * @param bnd
	 * @return
	 */
	bool isRotableBond(BALL::Bond &bnd);
	
	/**
	 * @brief isBridgingBond, bridging bonds are rotable bonds that connect
	 * two rigid fragments or a rigid fragment and a flexible fragment.
	 * (flexible fragments would not be separated by rotable bonds, but are 
	 * expanded by them)
	 * @param bnd
	 * @return 
	 */
	bool isBridgingBond(BALL::Bond &bnd);
	
	/**
	 * @brief fragment - fragments to rigid and flexible/linker fragments
	 *
	 * These are stored in AtomContainer vectors (ACVec). Also a list of
	 * connecting atoms between the respective fragments is saved in
	 * "connections". The fragments are formed by iterating over all bonds.
	 * Every bridging bond indicates a pair of atoms for the "conntections"
	 * list. Depending on their "isRigidAtom()" result atoms are assigned to
	 * a linker or rigid disjoint set. Individual fragments can be identified
	 * because bridging bonds are removed.
	 *
	 * @param rigid_fragments
	 * @param linker_fragments
	 * @param connections
	 */
	void fragment(BALL::ACVec &rigid_fragments, BALL::ACVec &linker_fragments,
														BALL::ConnectList& connections);
	
	/**
	 * @brief fragmentToSites - Selects all site-templates from a given input
	 * molecule
	 *
	 * The site templates are saved as pair with their site-key string (see the
	 * thesis for details). If restrict_to_rotables is set to true, only atoms
	 * that are part of a rotable bond will be used as site-centers. (Standard
	 * behavior) Otherwise all single bonds will be used.
	 *
	 * @param connections
	 * @param restrict_to_rotables
	 */
	void fragmentToSites(std::vector< std::pair< BALL::String, BALL::AtomContainer*> >& connections,
											 bool restrict_to_rotables=false);
	
	
	
private:
	/// For FragmentLibs:
	/*
	 * clearProperties() removes all atom properties of the internal '_molecule'.
	 * Properties are needed in order to annotate atoms with the 'InRing'
	 * property, but are also quite memory consuming.
	 */
	void extractAndClearProperties();
	
	/*
	 * calculates a mapping for the internal '_molecule'
	 */
	void calcAtomToPos();
	
	/// For connectionLibs:
	void addBondToConnectionsLib(BALL::Bond& bnd,
			std::vector< std::pair< BALL::String, BALL::AtomContainer*> > &connections);
	
	/*
	 * compares lexically on the String of the two pairs.
	 */
	static bool compare(std::pair<BALL::String,BALL::Atom*>& a, 
											std::pair<BALL::String,BALL::Atom*>& b);
	
	/*
	 * getSite
	 */
	BALL::AtomContainer* getSite(BALL::Atom* atm, BALL::String& key);
	
	/// Fields:
	BALL::AtomContainer* _molecule;
	
	std::vector< BALL::AtmVec >   _sssr;
	BALL::RingPerceptionProcessor _rpp;
	
	std::vector <bool>                        _is_InRing;
	boost::unordered_map< BALL::Bond*, bool > _is_BondInRing;
	
	std::vector < BALL::Atom* >             _atom_list;
	boost::unordered_map<BALL::Atom *, int> _atom_to_pos;
};

} //End Namespace BALL
#endif // LIGAND_FRAGMENTER_H
