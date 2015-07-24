// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef MOLECULECONNECTOR_H
#define MOLECULECONNECTOR_H

#include <BALL/STRUCTURE/LIGAND3DGEN/ligand3DBase.h>
#include <BALL/STRUCTURE/LIGAND3DGEN/starAligner.h>

namespace BALL 
{
/**
 * @brief The MoleculeConnector class connects two molecules by transforming
 * the coordinates of the second molecule in such a way that the geometry
 * corresponds to the given bond between the two molecules.
 *
 * This is done by using the StarAligner:
 * To the (star like) connection site
 * of mol1 a star like template is aligned. the template contains all atoms
 * of the site PLUS the bond-atom from mol2. Thus we derive the position
 * of the partner atom from mol2 from the additional atom from the aligned site
 * template.
 *
 * The same is done for the site on mol2 so that we have a bond vector for
 * mol1 and mol2. These two vectors are then aligned in a simple way
 *
 * NOTE: the connection is NOT guaranteed to be clash free!!!
 */
class MoleculeConnector
{
public:

	MoleculeConnector();
	~MoleculeConnector();
	
	void setConnectionLib(SiteFragmentDB& connectLib);
/**  
 * connectFragments - connects the two given fragments (by their child atoms)
 * in such a way, that molecule1 (given by 'atm1') retains its position,
 * whereas molecule 2 is transformed to fit the connection.
 * 
 * Precondition: 
 * atm1 and atm2 need to belong to separate BALL::AtomContainer. Between
 * both atoms (and thus between their respective AtomContainer) the bond
 * needs to already exist. So atm1 and atm2 also mark the connecting bond
 */
	void connect(BALL::Atom* atm1, BALL::Atom* atm2);

private:
	void loadTemplates(BALL::AtomContainer*& tmp, BALL::String &key);
	
	/*
	 * From an atom (given by 'atm') determine the site and the key for the site.
	 * The site contains the given atom at position 0 and all other direct 
	 * neighbors of 'atm' sorted according to their element and bond order, except
	 * for neighbors that belond to a different molecule/parent.
	 * The site 'key' on the other hand also contains these foreign neighbors
	 */
	void getSite(BALL::Atom* atm, AtmVec& site, BALL::String& key);
	
	/* 
	 * get transformation vector to move atom 2 so that it has the correct 
	 * distance to atom 1
	 */
	BALL::Vector3 getDiffVec(BALL::Atom* atm1, BALL::Atom* atm2);
	
	BALL::Atom* getMatchingAtomAll(BALL::Atom *center, AtmVec& mol, 
																 BALL::Element &elem, short bo);
	
	/*
	 * compare ("element+bondorder",Atom*)-pairs according to the string part
	 */
	static bool compare(std::pair<BALL::String,BALL::Atom*>& a, 
											std::pair<BALL::String,BALL::Atom*>& b);
	
	/// Class member:
	SiteFragmentDB* _connections;
	StarAligner     _star_aligner;
};

} // End Namespace "BALL"
#endif // MOLECULECONNECTOR_H
