// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef LIGSTRUC_FRAGMENTER_H
#define LIGSTRUC_FRAGMENTER_H


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

typedef vector <Fragment*> FragVec;
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
	
	void getMoleculeFragments(FragVec& rigid_fragments, FragVec& linker_fragments,
														list< pair<Atom*, Atom*> >& connections);
	
	
	
private:
	
	void calcRingProperties();
	
	/// Member Functions:
	/*
	 * clearProperties() removes all atom properties of the internal '_molecule'.
	 * Properties are need in order to annotate atoms with the 'InRing' property,
	 * but are also quite memory consuming.
	 */
	void clearProperties();
	
	/*
	 * calculates a mapping for the internal '_molecule'
	 */
	void calcAtomToPos();
	
	/// Fields:
	AtomContainer* _molecule;
	bool _properties_set; // flag indicating that 'InRing' properties were set
	vector< vector < Atom* > > _sssr;
	RingPerceptionProcessor _rpp;
	
	vector < Atom* > _atom_list;
	boost::unordered_map<Atom *, int> _atom_to_pos;
};

#endif // LIGSTRUC_FRAGMENTER_H
