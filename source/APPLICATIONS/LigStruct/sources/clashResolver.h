// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef CLASHRESOLVER_H
#define CLASHRESOLVER_H

#include "base.h"
#include "clashBase.h"

#include <BALL/MATHS/angle.h>

/// C l a s s   C l a s h R e s o l v e r
/// ############################################################################
class ClashResolver: public ClashDetector
{
public:
	ClashResolver( float tolerance = 1.3, int max_rotors = 20);
	~ClashResolver();
	
	/**
	 * @brief setMolecule
	 * @param molecule
	 * @param connections
	 */
	void setMolecule(BALL::AtomContainer& molecule, ConnectList& rotors);
	
	int resolve();
	
	/**
	 * Solve optimally under the constraint of discretized angles. ('steps'
	 * is the number by which we devide 360° to get discrete angles)
	 * 
	 * Rotates all bridging bonds in 'small' and 'large'
	 */ 
	int resolveAllCombinations( const int& steps = 3 );
	
	/**
	 * @brief detect ONLY clashes that occur between the two fragments NOT within
	 * a fragment (we assume that the two should start as clash free). Is a 
	 * wrapper for the call "detectBetweenMolecules( *_large_root, *_small_root)"
	 * 
	 * @return number of clashes found BETWEEN both fragments
	 */
	int detect();
	
protected:
	BALL::AtomContainer* _molecule;

	ConnectList* _all_rotors;
	
private:
	int allCombinationsRecur(const ConnectList::iterator& p, 
													const ConnectList::iterator &p_end, 
													BALL::Angle& angle, const int& steps );
	

	
	int _max_rotations; // maximum number of bonds to rotate
};





/// C l a s s   C o n n e c t i o n R e s o l v e r
/// ############################################################################
class ConnectionResolver: public ClashDetector
{
public:
	ConnectionResolver( float tolerance = 1.3, int max_rotors = 2);
	~ConnectionResolver();
	
	/**
	 * Preconditions: 
	 * 1.) The roots of 'atm1' and 'atm2' are two different BALL::AtomContainers 
	 * 2.) A bond between 'atm1' and 'atm2' exists.
	 * 3.) 'connections' contains all (rotable) fragment-bridging bonds
	 * 3.) The AtomContainer of 'atm1' is the larger one.
	 * 4.) Optimally '_root_large' should not be transformed.
	 * @brief setMolecule, sets the Resolver up to analyze and resolve clashes
	 *        between and within the fragments behind atm1 and atm2.
	 * @param atm1 part of the larger fragment
	 * @param atm2 part of the smaller fragment
	 */
	void setMolecule(BALL::Atom& atm1, BALL::Atom& atm2, 
									 ConnectList& connection_rotors, ConnectList *linker_rotors=0);
	
	/**
	 * @brief resolve -  does not to handle old clashes that existed already. 
	 * Always returns the correct complet clash-count, after optimization.
	 * NOTE: each resolveXYZ() call gives only clash counts for the fragment parts
	 * that should be solved but not the total clash count.
	 * 
	 * @param optimal
	 * @param conserve_large
	 * @return 
	 */
	std::pair<int, bool> resolve(bool optimal = false);
	
	/**
	 * @brief detect - Detects ONLY clashes that occur between the two fragments 
	 * that are to be connected. NOT clashes within any fragment (we assume that 
	 * given clashes can't be resolved by repeated attempts of ConnectionResolver)
	 * 
	 * @return number of clashes found BETWEEN both fragments
	 */
	int detect();
	
private:
	/*
	 * Try to resolve clashes between large and small fragment by rotating along
	 * the connecting bond.
	 */
	int resolveConnection();
	
	/*
	 * Rotate bonds within fragment 'frag' to remove inter and intra clashes.
	 * 'frag' needs to be either '_large_root' or '_small_root'.
	 */
	int resolveFragment(BALL::AtomContainer& frag , ConnectList &clist, const int &given_clashes = 0);
	
	int resolveLarge(const int &given_clashes = 0);
	
	/*
	 * Solve optimally under the constraint of discretized angles. ('steps'
	 * gives the number of angles that are to be tested for each bond)
	 * 
	 * Rotates all bridging bonds in 'small' and 'large'
	 */ 
	int resolveAllCombinations( const int& steps );
	
	int allCombinationsRecur(const ConnectList::iterator& p, 
													 const ConnectList::iterator &p_end, 
													 BALL::Angle& angle, 
													 const int& steps , 
													 int &best_cnt);
	
	int detectAll();
	
	BALL::Atom* atm_large;
	BALL::Atom* atm_small;
	
	BALL::AtomContainer* _large_root;
	BALL::AtomContainer* _small_root;

	ConnectList* _small_rotors;
	ConnectList* _large_rotors;
	ConnectList* _all_rotors;
	
	TemplateCoord* _save_large;
	TemplateCoord* _save_small;
	
	int _max_rotations;     // maximum number of bonds to rotate
};
#endif // CLASHRESOLVER_H
