#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#ifndef BASIC_H
#include "basic.h"
#endif

using namespace OpenBabel;
using namespace BALL;
using namespace std;

/**
 * @brief The AssemblerFunctions class, static class providing functions for 
 * the geometrically correct 3D connection of BALL::AtomContainer
 */
class AssemblerFunctions
{
public:
	/**
	 *
	 * @brief buildLinker a flexible linker fragment
	 * @param linker_lst
	 */
	static void buildLinker(vector< Fragment* >& linker_lst);

	/**  
	 * connectFragments connects the two given fragments (by their child atoms)
	 * in such a way, that molecule1 (given by 'atm1') retains its position,
	 * whereas molecule 2 is transformed to fit the connection.
	 */
	static void connectFragments(Atom* atm1, Atom* atm2,ConnectionMap& connectLib,
															 BondLengthMap& bondLib);
	
	/** 
	 * Structurally align a star-graph molecule: one central atom with any number 
	 * of direct neighbors, but without second degree neighbors (of dist 2).
	 * 
	 * The problem here is, that it is not in all cases possible to calculate for 
	 * star molecules an unique ordering on the atoms. 
	 * (e.g.: center atoms with 4 identical neighbors and bond types) 
	 * 
	 * In these problematic cases we simply have to try all 3-point-mappings and
	 * check the resulting RMSD (after transformation). In the end we use the
	 * transformation giving the lowest RMSD.
	 */
	static Matrix4x4 starAlign(AtmVec &mol1, AtomContainer *mol2);
	
	/**
	 * (Structurally) align a connection site to a template and return the 
	 * transformation matrix
	 * 
	 * What it does: finds a transformation from 'site' to 'templ', that 
	 * fits all atoms in both sets. For that a 3 point match searched that 
	 * fulfills the condition
	 */
	static Matrix4x4 align(AtmVec& site, AtomContainer* templ);
	
	/**
	 * ONLY for star like molecules!!!
	 *
	 * Get the minimal RMSD between two molecules by testing all meaningful
	 * mappings of mol1 to mol2. Atom element and bond type need to match and 
	 * the mapping is an unambigious projection (each atom of mol1 may only be once 
	 * assigned to an atom of mol2 per mapping and the other way around).
	 * 
	 * The central atom is not tested, because it is the translation 
	 * center and thus should always have distance 0
	 */
	static float getMinRMSD(AtomContainer* mol1, AtomContainer* mol2);
	static float getMinRMSD(AtmVec* mol1, AtomContainer* mol2);
	
	
	
private:
	/*
	 * Handle connections where at least one fragment contains only one atom
	 */
	static void handleSimpleConnections( Atom* atm1, Atom* atm2,
																boost::unordered_map <String, float >& bondLib,
																boost::unordered_map <String, Fragment* >& connectLib);
	
	/*
	 * compare ("element+bondorder",Atom*)-pairs according to the string part
	 */
	static bool compare(pair<String,Atom*>& a, pair<String,Atom*>& b);
	
	/*
	 * Creates a vector (on heap mem) containing all unique atoms of mol1
	 */
	static void getUniqueAtoms(AtomContainer* mol1, AtmVec& unique_atms);
	static void getUniqueAtoms(AtmVec &mol1, AtmVec& unique_atms);
	/*
	 * Check if two atoms of a star-molecule are identical in their element and
	 * the bond order to the central atom.
	 */
	static bool atomsCompatible(Atom* at1,Atom* at2);
	
	/*
	 * Swap two Atom-Pointer (by simply using references to pointer)
	 */
	static void swapAtoms(Atom*& a, Atom*& b);

	/*
 * Sets the 'global_sq_dist' the the lowest value found. All permutations are
 * tested and 'global_sq_dist' is only updated if a permutation yields a lower
 * sum of square distances.
 * 
 * Thus 'global_sq_dist' needs to initially point to a float that is set to
 * numeric_limits<float>::max() in the surronding recursion wrapper.
 */
	static void sqdistForPermutations(Atom* center1, AtomIterator& ati, AtmVec& atm_vec, int i, float loc_sq_dist, float* global_sq_dist);

	/* 
	 * Find the first atom in 'mol', that has same element and bond order as 'atm'
	 */
	static Atom* getMatchingAtom(AtomContainer *mol, Atom* atm);

	/*
 * get connection site
 * 'atm' the central atom spanning the site and 'partner' the atom
 * of the other fragment to whom a bond is to be formed.
 */
	static int getSite(Atom* atm, AtmVec& site, Atom* partner, String& key);

	/* 
	 * get transformation vector to move atom 2 so that it has the correct 
	 * distance to atom 1
	 */
	static Vector3 getDiffVec(Atom* atm1, Atom* atm2, BondLengthMap std_bonds);

	/*
	 * merge two connection templates to a final template
	 * 
	 * the final template will only contain 6 atoms, 3 for each end starting at 
	 * position 0 and 3 with the molecules that are to be connected, and then the
	 * ordered next two neighbors
	 */
	static void mergeTemplates(AtomContainer* mol1, int pos1, AtomContainer* mol2, int pos2,
														 boost::unordered_map<String, float> std_bonds);

	/*
	 *  Helper to get a single key component
	 */
	static String getBondName(Atom* atm, Atom* partner);

};
#endif // ASSEMBLER_H
