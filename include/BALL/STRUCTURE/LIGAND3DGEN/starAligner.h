// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef STARALIGNER_H
#define STARALIGNER_H

#include <BALL/STRUCTURE/LIGAND3DGEN/ligand3DBase.h>

#include <BALL/KERNEL/atomContainer.h>
#include <BALL/STRUCTURE/geometricTransformations.h>

namespace BALL 
{

/**
 * @brief The StarAligner class - This class tries to align the coordinates
 * of two identical starlike molecules.
 * It is also possible that the reference star molecule has less neighbors.
 *
 * Star like molecules have a central atom which forms bonds to neighbor atoms.
 * Other bonds than the ones originating from the central atom may not exist,
 * or are not part of the star molecule which is considered.
 *
 */
class StarAligner
{
public:
	StarAligner();
	
	~StarAligner();
	
	/**
	 * @brief align, transforms the coordinates of the 'query' to match with the
	 * 'reference'
	 * @return 
	 */
	void align();
	
	/**
	 * @brief bestRMSD returns the lowest RMSD between  'query' and 'reference'
	 * @return
	 */
	float bestRMSD();
	
	/**
	 * @brief bondAlign aligns two atom pairs such that
	 * A1 is moved onto B1 and A2 is rotated onto the ray B1-B2
	 * 
	 * The calculated transformation is applied to the molecule in 'this->_query'
	 */
	static BALL::Matrix4x4 bondAlign(BALL::Atom* atA1, BALL::Atom* atA2, 
																	 BALL::Atom* atB1, BALL::Atom* atB2);
	
	/**
	 * @brief getRemainder Get a list of atom-pointers of atoms that were not
	 * aligned (if 'this->_query' contained more atoms than 'this->_site').
	 */
	void getRemainder(BALL::AtmVec& remainder);
	
	/**
	 * @brief setMolecules inits the StarAligner
	 * @param reference this star molecule is kept at it original position
	 * @param query this star molecule is transformed such that it matches the
	 * references position best
	 */
	void setMolecules(BALL::AtomContainer& reference, BALL::AtomContainer& query);
	void setMolecules(AtmVec& ref_site, BALL::AtomContainer& query);
	
	/**
	 * ONLY for star like molecules!!!
	 *
	 * Get the minimal RMSD between two molecules by testing all meaningful
	 * mappings of mol1 to mol2. Atom element and bond type need to match and 
	 * the mapping is an unambigious projection (each atom of mol1 may only be
	 * once assigned to an atom of mol2 per mapping and the other way around).
	 * 
	 * The central atom is not tested, because it is the translation 
	 * center and thus should always have distance 0
	 */
	float getMinRMSD(AtmVec& vec1, BALL::AtomContainer& mol2);
	
private:
	
	bool _delete_site;
	bool _got_transformation;
	float _best_rmsd;
	
	AtmVec* _site;
	BALL::AtomContainer* _query;
	
	BALL::Matrix4x4 _matrix;
	BALL::TransformationProcessor _transformer;
	
	void _calculateOptimalTransformation();
	void _alignCase3(AtmVec& site);
	
	/* 
	 * Find the first atom in 'mol', that has same element and bond order as 'atm'
	 * 
	 * Precondition:
	 */
	BALL::Atom* getMatchingAtom(BALL::Atom *center, BALL::AtomContainer *mol, 
															BALL::String& elem, short bo);
	
	/*
	 * Fills an AtmVec 'unique_atms' with all unique atoms of 'mol1'
	 */
	void getUniqueAtoms(AtmVec &mol1, AtmVec& unique_atms);
	
	/*
	 * Sets the 'global_sq_dist' to the lowest value found. All permutations are
	 * tested and 'global_sq_dist' is only updated if a permutation yields a lower
	 * sum of square distances.
	 * 
	 * Thus 'global_sq_dist' needs to initially point to a float that is set to
	 * numeric_limits<float>::max() in the surronding recursion wrapper.
	 */
	void sqdistPerPermutation(AtmVec& vec1, 
														int     pos1, 
													  AtmVec& vec2, 
													  int     pos2, 
													  float   loc_sq_dist, 
													  float&  global_sq_dist);
	
	/*
	 * Swap two Atom-Pointer (by simply using references to pointer)
	 */
	void swapAtoms(BALL::Atom*& a, BALL::Atom*& b);
	
	/* 
	 * @brief toAtmVec - Translate the AtomContainer 'fromMol' into an AtmVec '
	 * toMol'
	 * @param fromMol
	 * @param toMol
	 */
	static void toAtmVec( BALL::AtomContainer& fromMol, AtmVec& toMol);
	
	/*
	 * twoPointMatch mapps:
	 *   n1 onto w1
	 *   n2 on the ray given by w1 - w2
	 */
	static BALL::Matrix4x4 twoPointMatch(
			const BALL::Vector3& n1, const BALL::Vector3& n2, const BALL::Vector3& w1,
			const BALL::Vector3& w2);
	
	/*
	 * Is a towPointMatch'ing that respects the need for planarity of neighbors
	 * connected to the bond if we are twoPointmatching a double bond for example
	 */
	BALL::Matrix4x4 doubleBondCorrection(BALL::Atom &tem1, BALL::Atom &tem2, 
																			 BALL::Atom &sit1, BALL::Atom &sit2);
	
	// recursion for the 'getRemainder' function
	void matchPermutaions(BALL::Atom &center, AVIter& ati1, AVIter& end1, 
												AtmVec& atm_vec, int i, float loc_sq_dist, 
												float* global_sq_dist, AtmVec& result);
	
};

} // End Namespace "BALL"
#endif // STARALIGNER_H
