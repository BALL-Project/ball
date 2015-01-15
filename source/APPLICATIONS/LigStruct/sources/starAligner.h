#ifndef STARALIGNER_H
#define STARALIGNER_H

#include <BALL/KERNEL/atomContainer.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/STRUCTURE/geometricTransformations.h>

using namespace BALL;
using namespace std;

typedef vector<Atom*> AtmVec;
typedef vector<Atom*>::iterator AVIter;

class StarAligner
{
public:
	StarAligner();
	
//	StarAligner(AtomContainer& reference, AtomContainer& query);
	
	~StarAligner();
	
	/**
	 * @brief align, transforms the coordinates of the 'query' to match with the
	 * 'reference'. The RMSD is returned.
	 * @return 
	 */
	void align();
	
	/**
	 * @brief bondAlign aligns two atom pairs such that
	 * A1 is moved onto B1 and A2 is rotated onto the ray B1-B2
	 * 
	 * The calculated transformation is applied to the query molecule
	 */
	void bondAlign(Atom* atA1, Atom* atA2, Atom* atB1, Atom* atB2);
	
	/*
	 * Get a list of atom-pointers of atoms that were not aligned (if 'query'
	 * contained more atoms than 'site').
	 */
	void getRemainder(AtmVec& remainder);
	
	void setMolecules(AtomContainer& reference, AtomContainer& query);
	void setMolecules(AtmVec& ref_site, AtomContainer& query);
	
//	float align(AtomContainer& reference, AtomContainer& query);
	
private:
	bool _delete_site;
	//	AtomContainer* _reference;
	AtmVec* _site;
	AtomContainer* _query;
	AtmVec _remainder;
	
	Matrix4x4 _matrix;
	TransformationProcessor _transformer;
	float _best_rmsd;
	
	void _calculateOptimalTransformation();
	void _alignCase3(AtmVec& site, AtomContainer &templ, Matrix4x4& trans_matrix);
	
	
	/*
	 * Check if two atoms of a star-molecule are identical in their element and
	 * the bond order to the central atom.
	 */
	bool atomsCompatible(Atom* at1,Atom* at2);
	
	/* 
	 * Find the first atom in 'mol', that has same element and bond order as 'atm'
	 * 
	 * Precondition:
	 */
	Atom* getMatchingAtom(Atom *center, AtomContainer *mol, String& elem, short bo);
	Atom* getMatchingAtom(Atom *center, AtmVec& mol, String& elem, short bo);
	
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
	float getMinRMSD(AtomContainer* mol1, AtomContainer* mol2);
	float getMinRMSD(AtmVec* vec1, AtomContainer* mol2);
	float getMinRMSD(AtmVec* vec1, AtmVec* vec2);
	
	/*
	 * Creates a vector (on heap mem) containing all unique atoms of mol1
	 */
	void getUniqueAtoms(AtomContainer* mol1, AtmVec& unique_atms);
	void getUniqueAtoms(AtmVec &mol1, AtmVec& unique_atms);
	
	/*
	 * Sets the 'global_sq_dist' the the lowest value found. All permutations are
	 * tested and 'global_sq_dist' is only updated if a permutation yields a lower
	 * sum of square distances.
	 * 
	 * Thus 'global_sq_dist' needs to initially point to a float that is set to
	 * numeric_limits<float>::max() in the surronding recursion wrapper.
	 */
	void sqdistPerPermutation(AVIter &ati, AVIter& end, AtmVec& atm_vec, int i, float loc_sq_dist, float* global_sq_dist);
	
	/*
	 * Swap two Atom-Pointer (by simply using references to pointer)
	 */
	void swapAtoms(Atom*& a, Atom*& b);
	
	/*
	 * twoPointMatch mapps:
	 *   n1 onto w1
	 *   n2 on the ray given by w1 - w2
	 */
	Matrix4x4 twoPointMatch(const Vector3& n1, const Vector3& n2, 
													const Vector3& w1, const Vector3& w2);
	
	// recursion for the 'getRemainder' function
	void matchPermutaions(Atom &center, AVIter& ati1, AVIter& end1, AtmVec& atm_vec, 
												int i, float loc_sq_dist, float* global_sq_dist, AtmVec& result);
	
};

#endif // STARALIGNER_H
