// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "starAligner.h"
#include "base.h"

#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/structureMapper.h>

#include <limits>

using namespace BALL;
using namespace std;

StarAligner::StarAligner()
{
	_delete_site = false;
	_site = 0;
	_query = 0;
	_best_rmsd = numeric_limits<float>::max();
	_got_transformation=false;
}

StarAligner::~StarAligner()
{
	if(_delete_site)
		delete _site;
}

void StarAligner::setMolecules(AtomContainer &reference, AtomContainer &query)
{
	if(_delete_site)
		delete _site;
	
	_got_transformation=false;
	_delete_site = true;
	_site = new AtmVec;
	LigBase::toAtmVec(reference, *_site);
	
	_query = &query;
}

void StarAligner::setMolecules(AtmVec &ref_site, AtomContainer &query)
{
	if(_delete_site)
		delete _site;
	
	_got_transformation=false;
	_delete_site = false;
	_site = &ref_site;
	_query = &query;
}

void StarAligner::align()
{
	_calculateOptimalTransformation();
	
	_transformer.setTransformation(_matrix);
	
	_query->apply(_transformer);
}

float StarAligner::bestRMSD()
{
	if( !_got_transformation )
		_calculateOptimalTransformation();

	_transformer.setTransformation(_matrix);
	
	AtomContainer temp( *_query );
	temp.apply(_transformer);
	
	_best_rmsd = getMinRMSD( *_site, temp);
	return _best_rmsd;
}


void StarAligner::_calculateOptimalTransformation()
{
	AtmVec& site = *_site;
	/// Case 1) 'site' has size 1, thus we can only translate the center 
	if ( site.size() == 1)
	{
		Vector3 transl = site[0]->getPosition() - _query->beginAtom()->getPosition();
		_matrix.setTranslation(transl);
	}
	/// Case 2) 'site' contains 1 neighbor. Find best match for it in 'templ' and 2pointMatch.
	else if ( site.size() == 2)
	{
		Vector3& sit1 = site[0]->getPosition();
		Vector3& sit2 = site[1]->getPosition();
		
		AtomIterator ati = _query->beginAtom();
		Vector3& tem1 = ati->getPosition();
		Atom* tem2_atm = 0;
		
		const Element& elem = site[1]->getElement();
		short bo = site[1]->getBond( *site[0] )->getOrder();
		
		// Find a compatible atom in 'templ'
		for(++ati; +ati; ++ati) 
		{
			if( ati->getElement() == elem && _query->beginAtom()->getBond(*ati)->getOrder() == bo) // at least 1 will be compatible
			{
				tem2_atm = &*ati;
			}
		}

		if ( (bo == 2 || bo == 5) && site[1]->countBonds() > 1)
		{
			_matrix = doubleBondCorrection( * _query->beginAtom(), * tem2_atm,
																			*site[0], * site[1]);
		}
		else
		{
			_matrix = twoPointMatch(tem1, tem2_atm->getPosition(), sit1, sit2);
		}
	}
	/// Case 3) 'site' has at least 2 neighbors
	else
	{
		_alignCase3(site);
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
void StarAligner::_alignCase3(AtmVec& site)
{
	// find all unique atoms in 'site' (according to BO to cental atom and element type)
	AtmVec unique_atms;
	unique_atms.reserve(8);

	getUniqueAtoms(site, unique_atms);
	
	/// Case 1) at least two unique atoms, straight computation of transformation
	if( unique_atms.size() > 1 )
	{
		Vector3& sit1 = site[0]->getPosition(); 
		Vector3& sit2 = unique_atms[0]->getPosition();
		Vector3& sit3 = unique_atms[1]->getPosition();
		
		Vector3& tem1 = _query->beginAtom()->getPosition();
		
		String elem2 = unique_atms[0]->getElement().getSymbol();
		short    bo2 = unique_atms[0]->getBond( *site[0] )->getOrder();
		String elem1 = unique_atms[1]->getElement().getSymbol();
		short    bo1 = unique_atms[1]->getBond( *site[0] )->getOrder();
		
		Vector3& tem2 = getMatchingAtom( &*_query->beginAtom(), _query, elem2, bo2 )->getPosition();
		Vector3& tem3 = getMatchingAtom( &*_query->beginAtom(), _query, elem1, bo1 )->getPosition();
		
		// get transformation:
		_matrix = StructureMapper::matchPoints( tem1, tem2, tem3, sit1, sit2, sit3);
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
		for(unsigned int i = 1; i < site.size(); ++i)
		{
			if( site[i] != unique_atms[0] )
			{
				site3_atm = site[i];
				break;
			}
		}
		
		Vector3& sit3 = site3_atm->getPosition();
		
		Vector3& tem1 = _query->beginAtom()->getPosition();
		String   elem = unique_atms[0]->getElement().getSymbol();
		short      bo = unique_atms[0]->getBond( *site[0] )->getOrder();
		
		Atom* tem2_atm = getMatchingAtom(&*_query->beginAtom(), _query, elem, bo );
		Vector3&  tem2 = tem2_atm->getPosition();
		
		// test all possible assignments of an 'templ'-atm to the 'site3_atm'
		float best_rmsd = numeric_limits<float>::max();
		Matrix4x4 test_matrix; TransformationProcessor transformer;
		
		bool bo_equal = false;
		bool elem_equal = false;
		
		AtomIterator ati = _query->beginAtom();
		for(++ati; +ati; ati++)
		{
			float rmsd = numeric_limits<float>::max();
			
			bo_equal = site3_atm->getBond( *site[0] )->getOrder() == ati->getBond(*_query->beginAtom())->getOrder();
			elem_equal = site3_atm->getElement() == ati->getElement();
			
			// all atoms that match 'site3_atm' and are not identical to 'stie2_atm'
			if(&*ati != tem2_atm && bo_equal && elem_equal) 
			{
				Vector3& tem3 = ati->getPosition();
				
				// get transformation and apply it for RMSD-testing:
				test_matrix = StructureMapper::matchPoints( tem1, tem2, tem3, sit1, sit2, sit3);
				transformer.setTransformation(test_matrix);
				
				// test transformation with a dummy_molecule:
				AtomContainer test_mol( *_query );
				test_mol.apply(transformer);

				rmsd = getMinRMSD( site, test_mol);

				if (rmsd < best_rmsd)
				{
					best_rmsd = rmsd;
					_matrix.set( test_matrix );
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
		
		Vector3& tem1 = _query->beginAtom()->getPosition();
		Atom* tem_2_atm = 0;
		Atom* tem_3_atm = 0;
		
		// test all possible assignments to 'selectionA2'
		bool bo_equal = false;
		bool elem_equal = false;
		AtomIterator ati = _query->beginAtom();
		Matrix4x4 temp_trans; TransformationProcessor transformer;
		
		for(ati++; +ati; ati++)
		{
			tem_2_atm = &*ati;
			
			bo_equal = site2_atm->getBond( *site[0] )->getOrder() ==  _query->beginAtom()->getBond(*tem_2_atm)->getOrder();
			elem_equal = site2_atm->getElement() == tem_2_atm->getElement();
			
			if( elem_equal && bo_equal)
			{
				Vector3& tem2 = tem_2_atm->getPosition(); //assign first neighbor
				
				// test all possible assignments to 'selectionA3'
				AtomIterator ato = _query->beginAtom();
				for(ato++; +ato; ato++)
				{
					tem_3_atm = &*ato;
					
					bo_equal = site3_atm->getBond( *site[0] )->getOrder() == _query->beginAtom()->getBond(*tem_3_atm)->getOrder();
					elem_equal = site2_atm->getElement() == tem_2_atm->getElement();
					
					if( (tem_2_atm != tem_3_atm) && ( elem_equal && bo_equal) )
					{
						float rmsd = numeric_limits<float>::max();
						AtomContainer test_mol( *_query );
						Vector3& tem3 = tem_3_atm->getPosition(); //assign second neighbor
						
						temp_trans = StructureMapper::matchPoints( tem1, tem2, tem3, sit1, sit2, sit3);
						transformer.setTransformation(temp_trans);
						test_mol.apply(transformer);
						
						rmsd = getMinRMSD( site, test_mol);
						if (rmsd < best_rmsd)
						{
							best_rmsd = rmsd;
							_matrix.set( temp_trans );
						}
					}
				}
			}
		}// loop-end
	}
}


/**
 * bondAlign
 */
Matrix4x4 StarAligner::bondAlign(Atom* atA1, Atom* atA2, Atom* atB1, Atom* atB2)
{
	Vector3& vA1 = atA1->getPosition();
	Vector3& vA2 = atA2->getPosition();
	
	Vector3& vB1 = atB1->getPosition();
	Vector3& vB2 = atB2->getPosition();
	
	// map the second pair onto the first pair:
	return twoPointMatch( vB1, vB2, vA1, vA2 );
}


/**
 * getRemainder
 * 
 * append all atoms in 'result' that remain. Meaning atoms of 'templ' that do not
 * optimally match with any atom in 'site'
 */
void StarAligner::getRemainder( AtmVec& remainder )
{
	AtmVec vec2;
	LigBase::toAtmVec(*_query, vec2);
	
	// the 'sum of all square distances' for the best (minimal) permutation:
	float min_sq_dist = numeric_limits<float>::max(); 
	
	if(_site->size() > 1)
	{
		// start with second atom (the first one is the central atom)
		AtmVec::iterator ati = _site->begin(); ++ati;
		AtmVec::iterator end1 = _site->end();
	
		matchPermutaions(*_site->at(0), ati, end1, vec2, 1, 0, &min_sq_dist, remainder);
	}
	else
	{
		remainder.assign(++vec2.begin(), vec2.end());
	}
}

/* 
 * getMatchingAtom
 */
Atom* StarAligner::getMatchingAtom(Atom* center, AtomContainer* mol, String &elem,  short bo)
{
	AtomIterator ati = mol->beginAtom();
	ati++; // first atom is the center atom, which we never want
	for(; +ati; ++ati)
	{
		if( ati->getElement().getSymbol() == elem && ati->getBond( *center )->getOrder() == bo )
			return &*ati;
	}
	
	cout<<"SEVERE ERROR: could not find a partner Atom!"<<endl;
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


/**
 * getMinRMSD
 */
float StarAligner::getMinRMSD(AtmVec& vec1, AtomContainer& mol2)
{
	AtmVec vec2;
	LigBase::toAtmVec(mol2, vec2);
	
	// the 'sum of all square distances' for the best (minimal) permutation:
	float global_sq_dist = numeric_limits<float>::max(); 
	
	sqdistPerPermutation( vec1, 1, vec2, 1, 0, global_sq_dist);
	
	global_sq_dist = sqrt( global_sq_dist / (float)(vec1.size() - 1) );
	
	return global_sq_dist;
}

/**
 * sqdistPerPermutations
 */
void StarAligner::sqdistPerPermutation(AtmVec& vec1, 
																			 int     pos1, 
																			 AtmVec& vec2, 
																			 int     pos2, 
																			 float   loc_sq_dist, 
																			 float&  global_sq_dist)
{
	// end case: a permutation was generated, check the RMSD
	if( pos1 == vec1.size() )
	{
		if( loc_sq_dist < global_sq_dist )
		{
			global_sq_dist = loc_sq_dist;
		}
		return;
	}
	// recursion case: 
	// test all remaining possible pertubations/mappings of atoms from mol2 
	// (the vector entry) to the next atom of mol1 (the atom iterator)
	else
	{
		float sq_dist_update;// the square distance for the current atom pair
		
		bool bo_compatible   = false;
		bool elem_compatible = false;
		
		const Element& elem_1 = vec1[pos1]->getElement();
		short  bo_1 = vec1[pos1]->getBond( * vec1[0] )->getOrder();
		
		for(unsigned int i = pos2; i < vec2.size(); i++)
		{
			// test if element and bondtype fit for this assignment
			bo_compatible = bo_1 == vec2[i]->getBond( * vec2[0] )->getOrder();
			elem_compatible = elem_1 == vec2[i]->getElement();
			
			if(  bo_compatible && elem_compatible)
			{
				sq_dist_update = vec1[pos1]->getPosition().getSquareDistance( vec2[i]->getPosition() );
				
				// permute the vector entries
				swapAtoms(vec2[pos2], vec2[i]); // use 'i' instead of 'pos2'
				
				// next recursion
				sqdistPerPermutation( vec1, (pos1+1), vec2, (pos2+1), (loc_sq_dist + sq_dist_update), global_sq_dist);
				
				swapAtoms(vec2[pos2], vec2[i]); // undo the swap for next itertation
			}
		} // end for-loop
	}
}

/*
 * getUniqueAtoms
 */
void StarAligner::getUniqueAtoms(AtmVec& mol1, AtmVec& unique_atms)
{
	AtmVec::iterator atmi = mol1.begin();
	short bo = 0;
	for (++atmi; atmi != mol1.end(); ++atmi)
	{
		bool isUnique = true;
		bo = (*atmi)->getBond( *mol1[0] )->getOrder();
		
		AtmVec::iterator atmk = mol1.begin();
		for(++atmk; atmk != mol1.end(); atmk++)
		{
			if( 
					atmk != atmi &&
					(*atmi)->getElement() == (*atmk)->getElement() &&
					bo == (*atmk)->getBond( *mol1[0] )->getOrder()
				)
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
 * #TODO: reimplement more efficiently
 */
Matrix4x4 StarAligner::twoPointMatch(const Vector3& n1, const Vector3& n2, 
																		 const Vector3& w1, const Vector3& w2)
{
	return StructureMapper::matchPoints(n1, n2, Vector3(), w1, w2, Vector3());
}

/*
 * doubleBondCorrection
 * make sure that if a site contains a double bond, that planarity with
 * all adjacent atoms is garanteed.
 */
Matrix4x4 StarAligner::doubleBondCorrection(Atom &tem1, Atom &tem2,
																						Atom &sit1, Atom &sit2)
{
	const Vector3& s1 = sit1.getPosition();
	const Vector3& s2 = sit2.getPosition();
	Atom* sit3 = 0;
	
	// get the first site atom that is connected to the double bond
	for (Atom::BondIterator bit = sit2.beginBond(); +bit; ++bit)
	{
		sit3 = bit->getBoundAtom( sit2 );
		if ( &sit1 != sit3)
			break;
	}
	
	const Vector3& t1 = tem1.getPosition();
	const Vector3& t2 = tem2.getPosition();
	Atom* tem3 = 0;
	
	// get the first atom of the fragment that is connected to the double bond
	for( Atom::BondIterator bit = tem1.beginBond(); +bit; ++bit)
	{
		tem3 = bit->getBoundAtom( tem1 );
		if ( &tem1 != tem3 )
			break;
	}
	// if the double bond has connected atoms on each end, match so that these
	// atoms are planar with the bond
	if ( tem3 != 0 && sit3 != 0)
	{
		Vector3& s3 = sit3->getPosition();
		Vector3& t3 = tem3->getPosition();

		return StructureMapper::matchPoints(t1, t2, t3, s1, s2, s3);
	}
	// otherwise a simple two point match is sufficient
	else
	{
		return StructureMapper::matchPoints(t1, t2, Vector3(), s1, s2, Vector3());
	}
}


/*
 * matchPermutaions
 */
void StarAligner::matchPermutaions(Atom& center, 
																	 AVIter& ati1, 
																	 AVIter& end1, 
																	 AtmVec& atm_vec, 
																	 int i, 
																	 float loc_sq_dist, 
																	 float* global_sq_dist, 
																	 AtmVec& result)
{
	/* 
	 * end recursion case: 
	 * everything was permuted so check how good the square dist was and perhaps
	 * update the global sq_dist
	 */
	if( ati1 == end1 )
	{
		if( loc_sq_dist < (*global_sq_dist) )
		{
			*global_sq_dist = loc_sq_dist;
			
			// insert into cleared result vector
			result.clear();
			for(unsigned int k = i; k < atm_vec.size(); ++k)
				result.push_back( atm_vec[k] );
		}
		return;
	}
	
	/*
	 *  recursion case:
	 * test all remaining possible pertubations/mappings of atoms from mol2 
	 * (the vectorentry) to the next atom of mol1 (the atom iterator)
	 */
	else
	{
		float sq_dist_update;// the square distance for the current atom pair
		for(unsigned int j = i; j < atm_vec.size(); j++)
		{
			// test if element and bondtype fit for this assignment
			// (this is rather for correctness than for speed)
			if( (*ati1)->getElement().getSymbol() == atm_vec[j]->getElement().getSymbol() &&
					(*ati1)->getBond( center )->getOrder()  == atm_vec[j]->beginBond()->getOrder() )
			{
				sq_dist_update = (*ati1)->getPosition().getSquareDistance( atm_vec[j]->getPosition() );
				
				swapAtoms(atm_vec[i], atm_vec[j]); // permute the vector entries
				
				AtmVec::iterator ati2 = ati1; // create new atom iterator for next recursion
				
				matchPermutaions( center, ++ati2, end1, atm_vec, (i+1), (loc_sq_dist + sq_dist_update),
													global_sq_dist, result);
				
				swapAtoms(atm_vec[i], atm_vec[j]); // undo the swap for next itertation
			}
		} // end for-loop
	}
}
