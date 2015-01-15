
#include "starAligner.h"

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/STRUCTURE/structureMapper.h>
// using namespace OpenBabel;
// using namespace BALL;
// using namespace std;

void StarAligner::align()
{
	_calculateOptimalTransformation();
	
	_transformer.setTransformation(_matrix);
	
	_query->apply(_transformer);
}
	





void StarAligner::_calculateOptimalTransformation()
{
	AtmVec site;
	/// Case 1) 'site' has size 1, thus we can only translate the center 
	if ( site.size() == 1)
	{
//		cout<<"Align: got case1"<<endl;
		Vector3 transl = site[0]->getPosition() - _query->beginAtom()->getPosition();
		_matrix.setTranslation(transl);
	}
	/// Case 2) 'site' contains 1 neighbor. Find best match for it in 'templ' and 2pointMatch.
	else if ( site.size() == 2)
	{
//		cout<<"Align: got case2"<<endl;
		Vector3& sit1 = site[0]->getPosition();
		Vector3& sit2 = site[1]->getPosition();
		float sq_dist12 = sit1.getSquareDistance( sit2 ); // get dist to single neighbor in site
		
		AtomIterator ati = _query->beginAtom();
		Vector3& tem1 = ati->getPosition();
		Vector3 tem2;
		
		const Element& elem = site[1]->getElement();
		short bo = site[1]->getBond( *site[0] )->getOrder();
		// Find a compatible atom in 'templ' that has the most similar sq_distance to sq_dist12
		float min_diff = numeric_limits<float>::max();
		for(++ati; +ati; ++ati) // SPEEDUP: theoretically it should be sufficiently effective if only take the first matching atom
		{
			if( ati->getElement() == elem && ati->beginBond()->getOrder() == bo) // at least 1 will be compatible
			{
				// calc dist:
				float diff = tem1.getSquareDistance( ati->getPosition() ) - sq_dist12;
				if( abs(diff) < min_diff)
					tem2.set( ati->getPosition() );
			}
		}
		_matrix = twoPointMatch(tem1, tem2, sit1, sit2);
	}
	/// Case 3) 'site' has at least 2 neighbors
	else
	{
//		cout<<"Align: got case3"<<endl;
		_alignCase3(site, *_query, _matrix);
	}
}






/*
 * alignCase3, finds:
 * - matrix that transforms 'templ' onto 'site'
 * - the transformation is selected that fits best
 * 
 * precondition extension to starAlign:
 * - 'site' and 'templ' are of at least size 3
 */
void StarAligner::_alignCase3(AtmVec& site, AtomContainer &templ, Matrix4x4& trans_matrix)
{
	// find all unique atoms in 'site' (according to BO to cental atom and element type)
	AtmVec unique_atms;
	unique_atms.reserve(8);
//	cout<<"getUnique"<<endl;
	getUniqueAtoms(site, unique_atms);
//	cout<<"got them"<<endl;
	
	/// Case 1) at least two unique atoms, straight computation of transformation
	if( unique_atms.size() > 1 )
	{
//		cout<<"align3case: subcase 1"<<endl;
		Vector3& sit1 = site[0]->getPosition(); 
		Vector3& sit2 = unique_atms[0]->getPosition();
		Vector3& sit3 = unique_atms[1]->getPosition();
		
		Vector3& tem1 = templ.beginAtom()->getPosition();
//		cout<<"before getMatching"<<endl;
//		cout<<&*templ.beginAtom()<<" "<<&templ <<" "<<unique_atms[0]<<endl;
		
		String elem2 = unique_atms[0]->getElement().getSymbol();
		short    bo2 = unique_atms[0]->getBond( *site[0] )->getOrder();
		String elem1 = unique_atms[1]->getElement().getSymbol();
		short    bo1 = unique_atms[1]->getBond( *site[0] )->getOrder();
		
		Vector3& tem2 = getMatchingAtom( &*templ.beginAtom(), &templ, elem2, bo2 )->getPosition();
		Vector3& tem3 = getMatchingAtom( &*templ.beginAtom(), &templ, elem1, bo1 )->getPosition();
//		cout<<tem1<<endl;
//		cout<<tem2<<endl;
		// get transformation:
		trans_matrix = StructureMapper::matchPoints( tem1, tem2, tem3, sit1, sit2, sit3);
	}
	
	
	/// Case 2) Only one unique atm exists, use this one and find best assignment 
	///         for a second neighbor from 'site'
	else if( unique_atms.size() == 1)
	{
		// if at least one mol1 atom was unique, it is curcial to use it:
		Vector3& sit1 = site[0]->getPosition();
		Vector3& sit2 = unique_atms[0]->getPosition();
		// get next non unique atom from site:
		Atom* site3_atm = 0;
		for(int i = 1; i < site.size(); ++i)
		{
			if( site[i] != unique_atms[0] )
			{
				site3_atm = site[i];
				break;
			}
		}
		Vector3& sit3 = site3_atm->getPosition();
		
		Vector3& tem1 = templ.beginAtom()->getPosition();
		String elem = unique_atms[0]->getElement().getSymbol();
		short    bo = unique_atms[0]->getBond( *site[0] )->getOrder();
		Atom* tem2_atm = getMatchingAtom(&*templ.beginAtom(), &templ, elem, bo );
		Vector3& tem2 = tem2_atm->getPosition();
		
		// test all possible assignments of an 'templ'-atm to the 'site3_atm'
		float best_rmsd = numeric_limits<float>::max();
		Matrix4x4 test_matrix; TransformationProcessor transformer;
		
		AtomIterator ati = templ.beginAtom();
		for(ati++; +ati; ati++)
		{
			float rmsd = numeric_limits<float>::max();
			if(&*ati != tem2_atm) // all atoms that are not yet assigned
			{
				Vector3& tem3 = ati->getPosition();
				
				// get transformation and apply it for RMSD-testing:
				test_matrix = StructureMapper::matchPoints( tem1, tem2, tem3, sit1, sit2, sit3);
				transformer.setTransformation(test_matrix);
				
				// test transformation with a dummy_molecule:
				AtomContainer test_mol( templ ); //SPEEDUP: efficient storing of original coordinates (FlexibleMolecule, ConformationSet, AtmVec-Overloading)
				test_mol.apply(transformer);
				rmsd = getMinRMSD(&site, &test_mol);
				if (rmsd < best_rmsd)
				{
					best_rmsd = rmsd;
					trans_matrix.set( test_matrix );
				}
				
			}
		} // loop-end
	}
	
	
	/// Case 3) No unique atom exists. Find best assignment for two neighbors
	else 
	{
		float best_rmsd = numeric_limits<float>::max();
		Atom* site2_atm = site[1];
		Atom* site3_atm = site[2];
		
		Vector3& sit1 = site[0]->getPosition();
		Vector3& sit2 = site2_atm->getPosition();
		Vector3& sit3 = site3_atm->getPosition();
		
		Vector3& tem1 = templ.beginAtom()->getPosition();
		Atom* tem_2_atm = 0;
		Atom* tem_3_atm = 0;
		
		// test all possible assignments to 'selectionA2'
		AtomIterator ati = templ.beginAtom();
		Matrix4x4 temp_trans; TransformationProcessor transformer;
		for(ati++; +ati; ati++)
		{
			tem_2_atm = &*ati;
			///TODO: 'atomsCompatible' won't work, use with a site atom, replace!
			if( atomsCompatible(site2_atm, tem_2_atm) ) 
			{
				Vector3& tem2 = tem_2_atm->getPosition(); //assign first neighbor
				
				// test all possible assignments to 'selectionA3'
				AtomIterator ato = templ.beginAtom();
				for(ato++; +ato; ato++)
				{
					tem_3_atm = &*ato;
					///TODO: 'atomsCompatible' won't work, use with a site atom, replace!
					if( (tem_2_atm != tem_3_atm) && (atomsCompatible(site3_atm, tem_3_atm)) )
					{
						float rmsd = numeric_limits<float>::max();
						AtomContainer test_mol( templ ); //SPEEDUP: efficient storing of original coordinates (FlexibleMolecule, ConformationSet, AtmVec-Overloading)
						Vector3& tem3 = tem_3_atm->getPosition(); //assign second neighbor
						
						temp_trans = StructureMapper::matchPoints( tem1, tem2, tem3, sit1, sit2, sit3);
						transformer.setTransformation(temp_trans);
						test_mol.apply(transformer);
						
						rmsd = getMinRMSD(&site, &test_mol);
						if (rmsd < best_rmsd)
						{
							best_rmsd = rmsd;
							trans_matrix = trans_matrix;
						}
					}
				}
			}
		}// loop-end
	}
}


/*
 * atomsCompatible
 * TODO: improve this version. Perhaps by using selections for sites and 
 * restricting to bonds that are selected?
 */
bool StarAligner::atomsCompatible(Atom* at1,Atom* at2)
{
	return ( 
					 ( at1->getElement()            == at2->getElement() ) && 
					 ( at1->beginBond()->getOrder() == at2->beginBond()->getOrder()) 
				 );
}


/**
 * bondAlign
 */
void StarAligner::bondAlign(Atom* atA1, Atom* atA2, 
																	 Atom* atB1, Atom* atB2, 
																	 Matrix4x4& trans_matr)
{
	Vector3& vA1 = atA1->getPosition();
	Vector3& vA2 = atA2->getPosition();
	
	Vector3& vB1 = atB1->getPosition();
	Vector3& vB2 = atB2->getPosition();
	
	// map the second pair onto the first pair:
	trans_matr = twoPointMatch( vB1, vB2, vA1, vA2 );
}

/*
 * fromMoleculeToAtmVec
 */
void StarAligner::fromMoleculeToAtmVec(AtomContainer& in_mol, AtmVec& out_vec)
{
	for(AtomIterator ati = in_mol.beginAtom(); +ati; ++ati)
	{
		out_vec.push_back(&*ati);
	}
	
}

/* 
 * getMatchingAtom(1)
 */
Atom* StarAligner::getMatchingAtom(Atom *center, AtmVec& mol, String& elem, short bo)
{
	cout<<"in getMatching 2"<<endl;

	AVIter ati = mol.begin();
	ati++; // first atom is the center atom, which we never want
	for(; ati != mol.end(); ++ati)
	{
		cout<< (*ati)->getElement().getSymbol()<<(*ati)->getBond( *center )->getOrder()<<" ";
		cout<<((*ati)->getElement().getSymbol() == elem)<<" ";
		cout<< ( (*ati)->getBond( *center )->getOrder() == bo )<<endl;
		if( (*ati)->getElement().getSymbol() == elem && (*ati)->getBond( *center )->getOrder() == bo )
			return *ati;
	}
	
	cout<<"ERROR: could not find a partner for: "<<elem<<bo<<endl<<endl;
	cout<<"Molecule had:"<<endl;
	for(AVIter at = mol.begin(); at != mol.end(); ++at)
	{
		cout<<(*at)->getElement().getSymbol();
		if(*at != center)
			cout<<(*at)->getBond( *center )->getOrder();
		cout<<" ";
	}
	cout<<endl;
	exit(EXIT_FAILURE);
}

/* 
 * getMatchingAtom(2)
 */
Atom* StarAligner::getMatchingAtom(Atom* center, AtomContainer* mol, String &elem,  short bo)
{
	cout<<"in getMatching 1"<<endl;

	AtomIterator ati = mol->beginAtom();
	ati++; // first atom is the center atom, which we never want
	for(; +ati; ++ati)
	{
		if( ati->getElement().getSymbol() == elem && ati->getBond( *center )->getOrder() == bo )
			return &*ati;
	}
	
	cout<<"ERROR: could not find a partner Atom!"<<endl;
	cout<<"Was looking for: "<<elem<<"-"<<bo<<endl<<endl;
	cout<<"Molecule contained:"<<endl;
	for(AtomIterator at = mol->beginAtom(); +at; ++at)
	{
		cout<<at->getElement().getSymbol();
		if(&*at != center )
			cout<<at->getBond( *center )->getOrder();
		cout<<", ";
	}
	cout<<endl;
	exit(EXIT_FAILURE);
}

/* 
 * getMatchingAtomAll(3)
 */
Atom* StarAligner::getMatchingAtomAll(Atom *center, AtmVec& mol, String& elem, short bo)
{
	cout<<"in getMatchingAll 3"<<endl;

	AVIter ati = mol.begin();
	for(; ati != mol.end(); ++ati)
	{
//		cout<< (*ati)->getElement().getSymbol()<<(*ati)->getBond( *center )->getOrder()<<" ";
//		cout<<((*ati)->getElement().getSymbol() == elem)<<" ";
//		cout<< ( (*ati)->getBond( *center )->getOrder() == bo )<<endl;
		if( (*ati)->getElement().getSymbol() == elem && (*ati)->getBond( *center )->getOrder() == bo )
			return *ati;
	}
	
	cout<<"ERROR: could not find a partner for: "<<elem<<bo<<endl<<endl;
	cout<<"Molecule had:"<<endl;
	for(AVIter at = mol.begin(); at != mol.end(); ++at)
	{
		cout<<(*at)->getElement().getSymbol();
		if(*at != center)
			cout<<(*at)->getBond( *center )->getOrder();
		cout<<" ";
	}
	cout<<endl;
	exit(EXIT_FAILURE);
}


/**
 * getMinRMSD (1)
 */
float StarAligner::getMinRMSD(AtomContainer* mol1, AtomContainer* mol2)
{
	AtmVec vec1;
	AtmVec vec2;
	fromMoleculeToAtmVec(*mol1, vec1);
	fromMoleculeToAtmVec(*mol2, vec2);
	
	// the 'sum of all square distances' for the best (minimal) permutation:
	float min_sq_dist = numeric_limits<float>::max(); 
	
	AtmVec::iterator ati = vec1.begin(); ++ati;  // start with second atom (first is central atom)
	AtmVec::iterator end1 = vec1.end();
	sqdistPerPermutation( ati, end1, vec2, 1, 0, &min_sq_dist);
	
	min_sq_dist = sqrt( min_sq_dist / (float)(vec1.size() - 1) );
	return min_sq_dist;
}

/**
 * getMinRMSD (2)
 */
float StarAligner::getMinRMSD(AtmVec* vec1, AtomContainer* mol2)
{
	AtmVec vec2;
	fromMoleculeToAtmVec(*mol2, vec2);
	
	// the 'sum of all square distances' for the best (minimal) permutation:
	float min_sq_dist = numeric_limits<float>::max(); 
	
	AtmVec::iterator ati = vec1->begin(); ++ati;  // start with second atom (first is central atom)
	AtmVec::iterator end1 = vec1->end();
	sqdistPerPermutation( ati, end1, vec2, 1, 0, &min_sq_dist);
	
	min_sq_dist = sqrt( min_sq_dist / (float)(vec1->size() - 1) );
	return min_sq_dist;
}

/**
 * getMinRMSD (3)
 */
float StarAligner::getMinRMSD(AtmVec* vec1, AtmVec* vec2)
{
	float min_sq_dist = numeric_limits<float>::max();
	
	AtmVec::iterator ati = vec1->begin(); ++ati;
	AtmVec::iterator end1 = vec1->end();
	sqdistPerPermutation( ati, end1, *vec2, 1, 0, &min_sq_dist);
	
	min_sq_dist = sqrt( min_sq_dist / (float)(vec1->size() - 1) );
	return min_sq_dist;
}

/*
 * getUniqueAtoms (1)
 */
void StarAligner::getUniqueAtoms(AtomContainer* mol1, AtmVec& unique_atms)
{
	AtomIterator atmi = mol1->beginAtom();
	for (atmi++; +atmi; ++atmi)
	{
		bool isUnique = true;
		AtomIterator atmk = atmi; //mol1->beginAtom();
		for(atmk++; +atmk; ++atmk)
		{
			if( atomsCompatible( &*atmi, &*atmk) )
			{
				isUnique = false;
				break;
			}
		}
		if(isUnique)
		{
			unique_atms.push_back(&*atmi);
		}
	}
}

/*
 * getUniqueAtoms (2)
 */
void StarAligner::getUniqueAtoms(AtmVec& mol1, AtmVec& unique_atms)
{
	AtmVec::iterator atmi = mol1.begin();
	for (atmi++; atmi != mol1.end(); atmi++)
	{
		bool isUnique = true;
		AtmVec::iterator atmk = atmi; //mol1->beginAtom();
		for(atmk++; atmk != mol1.end(); atmk++)
		{
			if( atomsCompatible(*atmi, *atmk) )
			{
				isUnique = false;
				break;
			}
		}
		if(isUnique)
		{
			unique_atms.push_back(*atmi);
		}
	}
}

/*
 * sqdistForPermutations
 */
void StarAligner::sqdistPerPermutation(AVIter& ati1, AVIter& end1,
							AtmVec& atm_vec, int i, float loc_sq_dist, float* global_sq_dist)
{
	// end recursion case: 
	// everything was permuted so check how good the square dist was and perhaps
	// update the global sq_dist
	if( ati1 == end1 )
	{
		if( (*global_sq_dist) > loc_sq_dist)
		{
			*global_sq_dist = loc_sq_dist;
		}
		return;
	}
	// recursion case:
	// test all remaining possible pertubations/mappings of atoms from mol2 
	// (the vectorentry) to the next atom of mol1 (the atom iterator)
	else
	{
		float sq_dist_update;// the square distance for the current atom pair
		for(int j = i; j < atm_vec.size(); j++)
		{
			// test if element and bondtype fit for this assignment
			// (this is rather for correctness than for speed)
			///TODO: illegal use of 'atomsCompatible'
			if(  atomsCompatible( *ati1, atm_vec[j] )  )
			{
				sq_dist_update = (*ati1)->getPosition().getSquareDistance( atm_vec[j]->getPosition() );
				
				swapAtoms(atm_vec[i], atm_vec[j]); // permute the vector entries
				
				AtmVec::iterator ati2 = ati1; // create new atom iterator for next recursion
				sqdistPerPermutation( ++ati2, end1, atm_vec, (i+1), (loc_sq_dist + sq_dist_update), global_sq_dist);
				
				swapAtoms(atm_vec[i], atm_vec[j]); // undo the swap for next itertation
			}
		} // end for-loop
	}
}

/*
 * swapAtoms
 */
void StarAligner::swapAtoms(Atom*& a, Atom*& b)
{
	Atom* tmp = a;
	a = b;
	b = tmp;
}

/*
 * twoPointMatch
 * TODO: reimplement more efficiently
 */
Matrix4x4 StarAligner::twoPointMatch(const Vector3& n1, const Vector3& n2, 
																		 const Vector3& w1, const Vector3& w2)
{
	return StructureMapper::matchPoints(n1, n2, Vector3(), w1, w2, Vector3());
}
