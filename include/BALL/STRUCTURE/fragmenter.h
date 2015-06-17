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

class MoleculeFragmenter
{
public:
	MoleculeFragmenter();
	~MoleculeFragmenter();
	
	void setMolecule(BALL::AtomContainer& in_mol);
	
	bool isRigidAtom(BALL::Atom &atm);
	
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
	
	void fragment(BALL::ACVec &rigid_fragments, BALL::ACVec &linker_fragments,
														BALL::ConnectList& connections);
	
	void fragmentToSites(std::vector< std::pair< BALL::String, BALL::AtomContainer*> >& connections,
											 bool restrict_to_rotables=false);
	
	
	
private:
	/// For FragmentLibs:
	/*
	 * clearProperties() removes all atom properties of the internal '_molecule'.
	 * Properties are need in order to annotate atoms with the 'InRing' property,
	 * but are also quite memory consuming.
	 */
	void extractAndClearProperties();
	
	/*
	 * calculates a mapping for the internal '_molecule'
	 */
	void calcAtomToPos();
	
	/// For connectionLibs:
	void addBondToConnectionsLib(BALL::Bond& bnd,
			std::vector< std::pair< BALL::String, BALL::AtomContainer*> > &connections);
	
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
