// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef LIGSTRUC_FRAGMENTER_H
#define LIGSTRUC_FRAGMENTER_H

#include "base.h"

#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/molecule.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>


#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>

#include <vector>
#include <boost/pending/disjoint_sets.hpp>
#include <boost/unordered_map.hpp>

using namespace BALL;
using namespace std;

class MoleculeFragmenter
{
public:
	MoleculeFragmenter();
	~MoleculeFragmenter();
	
	void setMolecule(AtomContainer& in_mol);
	
	bool isRigidAtom(Atom &atm);
	
	bool isRotableBond(Bond &bnd);
	
	/**
	 * @brief isBridgingBond, bridging bonds are rotable bonds that connect
	 * two rigid fragments or a rigid fragment and a flexible fragment.
	 * (flexible fragments would not be separated by rotable bonds, but are 
	 * expanded by them)
	 * @param bnd
	 * @return 
	 */
	bool isBridgingBond(Bond &bnd);
	
	void fragment(ACVec &rigid_fragments, ACVec &linker_fragments,
														ConnectList& connections);
	
	void fragmentToSites(boost::unordered_map< String, pair<float, int> >& bondLenths,
											 vector< pair< String, AtomContainer*> >& connections, 
											 bool restrict_to_bridges=false);
	
	
	
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
	void addBondToConnectionsLib(Bond& bnd,
															 boost::unordered_map< String, pair<float, int> >& bondLenths,
															 vector< pair< String, AtomContainer*> > &connections);
	
	static bool compare(pair<String,Atom*>& a, pair<String,Atom*>& b);
	
	/*
	 * getSite
	 */
	AtomContainer* getSite(Atom* atm, String& key);
	
	/*
	 * Add in such a way that we keep the average bond length
	 */
	void addLengthtoLib(
			boost::unordered_map<String, pair<float, int> > & bondLengths, 
			String bk1, String bk2, float len);
	
	
	
	
	/// Fields:
	AtomContainer* _molecule;
	
	vector< AtmVec > _sssr;
	RingPerceptionProcessor _rpp;
	
	vector <bool> _is_InRing;
	boost::unordered_map< Bond*, bool > _is_BondInRing;
	
	vector < Atom* > _atom_list;
	boost::unordered_map<Atom *, int> _atom_to_pos;
};

#endif // LIGSTRUC_FRAGMENTER_H
