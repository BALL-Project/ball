// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef STARALIGNER_H
#define STARALIGNER_H

#include <BALL/KERNEL/atomContainer.h>
#include <BALL/STRUCTURE/geometricTransformations.h>

typedef std::vector<BALL::Atom*> AtmVec;
typedef std::vector<BALL::Atom*>::iterator AVIter;

class StarAligner
{
public:
	StarAligner();
	
	~StarAligner();
	
	/**
	 * @brief align, transforms the coordinates of the 'query' to match with the
	 * 'reference'. The RMSD is returned.
	 * @return 
	 */
	void align();
	
	float bestRMSD();
	
	/**
	 * @brief bondAlign aligns two atom pairs such that
	 * A1 is moved onto B1 and A2 is rotated onto the ray B1-B2
	 * 
	 * The calculated transformation is applied to the query molecule
	 */
	static BALL::Matrix4x4 bondAlign(BALL::Atom* atA1, BALL::Atom* atA2, BALL::Atom* atB1, 
												BALL::Atom* atB2);
	
	/*
	 * Get a list of atom-pointers of atoms that were not aligned (if 'query'
	 * contained more atoms than 'site').
	 */
	void getRemainder(AtmVec& remainder);
	
	void setMolecules(BALL::AtomContainer& reference, BALL::AtomContainer& query);
	void setMolecules(AtmVec& ref_site, BALL::AtomContainer& query);
	
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
//	float getMinRMSD(BALL::AtomContainer* mol1, BALL::AtomContainer* mol2);
	float getMinRMSD(AtmVec* vec1, BALL::AtomContainer* mol2);
//	float getMinRMSD(AtmVec* vec1, AtmVec* vec2);
	
private:
	bool _delete_site;
	//	AtomContainer* _reference;
	AtmVec* _site;
	BALL::AtomContainer* _query;
//	AtmVec _remainder;
	
	BALL::Matrix4x4 _matrix;
	BALL::TransformationProcessor _transformer;
	bool _got_transformation;
	float _best_rmsd;
	
	void _calculateOptimalTransformation();
	void _alignCase3(AtmVec& site);
	
	
	/*
	 * Check if two atoms of a star-molecule are identical in their element and
	 * the bond order to the central atom.
	 */
	bool atomsCompatible(BALL::Atom* at1, BALL::Atom* at2);
	
	/* 
	 * Find the first atom in 'mol', that has same element and bond order as 'atm'
	 * 
	 * Precondition:
	 */
	BALL::Atom* getMatchingAtom(BALL::Atom *center, BALL::AtomContainer *mol, 
															BALL::String& elem, short bo);
	
	/*
	 * Creates a vector (on heap mem) containing all unique atoms of mol1
	 */
	void getUniqueAtoms(BALL::AtomContainer* mol1, AtmVec& unique_atms);
	void getUniqueAtoms(AtmVec &mol1, AtmVec& unique_atms);
	
	/*
	 * Sets the 'global_sq_dist' the the lowest value found. All permutations are
	 * tested and 'global_sq_dist' is only updated if a permutation yields a lower
	 * sum of square distances.
	 * 
	 * Thus 'global_sq_dist' needs to initially point to a float that is set to
	 * numeric_limits<float>::max() in the surronding recursion wrapper.
	 */
	void sqdistPerPermutation(AVIter &ati, AVIter& end, AtmVec& atm_vec, int i, 
														float loc_sq_dist, float* global_sq_dist);
	
	/*
	 * Swap two Atom-Pointer (by simply using references to pointer)
	 */
	void swapAtoms(BALL::Atom*& a, BALL::Atom*& b);
	
	/*
	 * twoPointMatch mapps:
	 *   n1 onto w1
	 *   n2 on the ray given by w1 - w2
	 */
	static BALL::Matrix4x4 twoPointMatch(
			const BALL::Vector3& n1, const BALL::Vector3& n2, const BALL::Vector3& w1,
			const BALL::Vector3& w2);
	
	/*
	 * Is a towPointMatching that respects the need for planarity of neighbors
	 * connected to the bond if we are twoPointmatching a double bond.
	 */
	BALL::Matrix4x4 doubleBondCorrection(BALL::Atom &tem1, BALL::Atom &tem2, 
																			 BALL::Atom &sit1, BALL::Atom &sit2);
	
	// recursion for the 'getRemainder' function
	void matchPermutaions(BALL::Atom &center, AVIter& ati1, AVIter& end1, 
												AtmVec& atm_vec, int i, float loc_sq_dist, 
												float* global_sq_dist, AtmVec& result);
	
};

#endif // STARALIGNER_H
