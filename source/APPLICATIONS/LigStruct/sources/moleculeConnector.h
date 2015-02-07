// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef MOLECULECONNECTOR_H
#define MOLECULECONNECTOR_H

#include "base.h"
#include "starAligner.h"

#include <BALL/DATATYPE/string.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/atom.h>

#include <boost/unordered_map.hpp>

using namespace BALL;
using namespace std;

class MoleculeConnector
{
public:

	MoleculeConnector();
	~MoleculeConnector();
	
	void setConnectionLib(SiteMap& connectLib);
	void setBondLengthLib(BondLengthMap& bondLib);
	void setLibs(SiteMap& connectLib, BondLengthMap& bondLib);
/**  
 * connectFragments connects the two given fragments (by their child atoms)
 * in such a way, that molecule1 (given by 'atm1') retains its position,
 * whereas molecule 2 is transformed to fit the connection.
 * 
 * Precondition: 
 * atm1 and atm2 need to belong to separate BALL::AtomContainer. Between
 * both atoms (and thus between their respective AtomContainer) the bond
 * needs to already exist.
 */
	void connect(Atom* atm1, Atom* atm2);

private:
	void loadTemplates(AtomContainer*& tmp, String &key);
	
	/*
	 * From an atom (given by 'atm') determine the site and the key for the site.
	 * The site contains the given atom at position 0 and all other direct 
	 * neighbors of 'atm' sorted according to their element and bond order, except
	 * for neighbors that belond to a different molecule/parent.
	 * The site 'key' on the other hand also contains these foreign neighbors
	 */
	void getSite(Atom* atm, AtmVec& site, String& key);
	
	/* 
	 * get transformation vector to move atom 2 so that it has the correct 
	 * distance to atom 1
	 */
	Vector3 getDiffVec(Atom* atm1, Atom* atm2);
	
	Atom* getMatchingAtomAll(Atom *center, AtmVec& mol, Element &elem, short bo);
	
	/*
	 * compare ("element+bondorder",Atom*)-pairs according to the string part
	 */
	static bool compare(pair<String,Atom*>& a, pair<String,Atom*>& b);
	
	/// Class member:
	SiteMap* _connections;
	BondLengthMap* _bond_lengths;
	StarAligner _star_aligner;
};

#endif // MOLECULECONNECTOR_H
