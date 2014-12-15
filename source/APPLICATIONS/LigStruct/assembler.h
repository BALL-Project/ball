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
	static void buildLinker(Fragment &linker_frag, ConnectionMap &link_lib);

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
//	static void starAlign(AtmVec& vec1, AtmVec& vec2, Matrix4x4& trans_matrix);
	static void starAlign(AtmVec& vec1, AtomContainer& mol2, Matrix4x4& trans_matrix);
	
	/**
	 * @brief bondAlign aligns two atom
	 */
	static void bondAlign(Atom* atA1, Atom* atA2, Atom* atB1, Atom* atB2, Matrix4x4& trans_matr);
	
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
	static float getMinRMSD(AtmVec* vec1, AtomContainer* mol2);
	static float getMinRMSD(AtmVec* vec1, AtmVec* vec2);
	
	
	
private:
	/*
	 * Fill an AtmVec with atom-pointers from an AtomContainer
	 */
	static void fromMoleculeToAtmVec(AtomContainer &in_mol, AtmVec& out_vec);
	
	/*
	 * Get a list of atom-pointer from 'templ' that do not match any atom
	 * in 'site'. Matching is done according to RMSD and a threshold of 'thresh'
	 */
	static void getRemaining(AtmVec& site, AtomContainer &templ, AtmVec& result);
	
	static void matchPermutaions(AVIter& ati1, AVIter& end1, 
																						AtmVec& atm_vec, int i, 
																						float loc_sq_dist, float* global_sq_dist,
																						AtmVec& result);
	
//	/*
//	 * Handle connections where at least one fragment contains only one atom
//	 */
//	static void handleSimpleConnections( Atom* atm1, Atom* atm2,
//																boost::unordered_map <String, float >& bondLib,
//																boost::unordered_map <String, Fragment* >& connectLib);
	
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
	 * Sets the 'global_sq_dist' the the lowest value found. All permutations are
	 * tested and 'global_sq_dist' is only updated if a permutation yields a lower
	 * sum of square distances.
	 * 
	 * Thus 'global_sq_dist' needs to initially point to a float that is set to
	 * numeric_limits<float>::max() in the surronding recursion wrapper.
	 */
	static void sqdistPerPermutation(AVIter &ati, AVIter& end, AtmVec& atm_vec, int i, float loc_sq_dist, float* global_sq_dist);

	/*
	 * From an atom (given by 'atm') determine the site and the key for the site.
	 * The site contains the given atom at position 0 and all other direct 
	 * neighbors of 'atm' sorted according to their element and bondorder
	 */
	static void getSite(Atom* atm, AtmVec& site, String& key);

	
	static void getSelectedSite(Atom* atm, Atom *partner, AtmVec& site, String& key);
	static void recurLinkerConnect(Atom* at, Composite * const parent, ConnectionMap& link_lib);
	static void connectAtomToSite(AtmVec& site, AtomContainer& temp, Atom* partner);
	
	/* 
	 * get transformation vector to move atom 2 so that it has the correct 
	 * distance to atom 1
	 */
	static Vector3 getDiffVec(Atom* atm1, Atom* atm2, BondLengthMap std_bonds);

	/*
	 *  Helper to get a single key component
	 */
	static String getBondName(Atom* atm, Atom* partner);
	
	/* 
	 * Find the first atom in 'mol', that has same element and bond order as 'atm'
	 */
	static Atom* getMatchingAtom(AtomContainer *mol, Atom* atm);
	static Atom* getMatchingAtom(AtmVec& mol, Atom* atm);
	
	/*
	 * Check if two atoms of a star-molecule are identical in their element and
	 * the bond order to the central atom.
	 */
	static bool atomsCompatible(Atom* at1,Atom* at2);
	
	/*
	 * Swap two Atom-Pointer (by simply using references to pointer)
	 */
	static void swapAtoms(Atom*& a, Atom*& b);
	
	static Matrix4x4 twoPointMatch(const Vector3& v1, const Vector3& v2, 
																 const Vector3& u1, const Vector3& u2);
};
#endif // ASSEMBLER_H
