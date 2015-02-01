// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "starAligner.h"
#include "base.h"

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/STRUCTURE/structureMapper.h>

#include <limits>


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
	
	_best_rmsd = getMinRMSD(_site, &temp);
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
		Atom* tem2_atm;
		
		const Element& elem = site[1]->getElement();
		short bo = site[1]->getBond( *site[0] )->getOrder();
		
		// Find a compatible atom in 'templ'
		for(++ati; +ati; ++ati) 
		{
			if( ati->getElement() == elem && ati->beginBond()->getOrder() == bo) // at least 1 will be compatible
			{
				tem2_atm = &*ati;
			}
		}
		if ( (bo == 2 || bo == 5) && site[1]->countBonds() > 1)
			_matrix = doubleBondCorrection( * _query->beginAtom(), * tem2_atm,
																			*site[0], * site[1]);
		else
			_matrix = twoPointMatch(tem1, tem2_atm->getPosition(), sit1, sit2);
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
		for(int i = 1; i < site.size(); ++i)
		{
			if( site[i] != unique_atms[0] )
			{
				site3_atm = site[i];
				break;
			}
		}
		Vector3& sit3 = site3_atm->getPosition();
		
		Vector3& tem1 = _query->beginAtom()->getPosition();
		String elem = unique_atms[0]->getElement().getSymbol();
		short    bo = unique_atms[0]->getBond( *site[0] )->getOrder();
		Atom* tem2_atm = getMatchingAtom(&*_query->beginAtom(), _query, elem, bo );
		Vector3& tem2 = tem2_atm->getPosition();
		
		// test all possible assignments of an 'templ'-atm to the 'site3_atm'
		float best_rmsd = numeric_limits<float>::max();
		Matrix4x4 test_matrix; TransformationProcessor transformer;
		
		AtomIterator ati = _query->beginAtom();
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
				AtomContainer test_mol( *_query );
				test_mol.apply(transformer);
				rmsd = getMinRMSD(&site, &test_mol);
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
		AtomIterator ati = _query->beginAtom();
		Matrix4x4 temp_trans; TransformationProcessor transformer;
		for(ati++; +ati; ati++)
		{
			tem_2_atm = &*ati;
			///TODO: 'atomsCompatible' won't work, use with a site atom, replace!
			if( atomsCompatible(site2_atm, tem_2_atm) ) 
			{
				Vector3& tem2 = tem_2_atm->getPosition(); //assign first neighbor
				
				// test all possible assignments to 'selectionA3'
				AtomIterator ato = _query->beginAtom();
				for(ato++; +ato; ato++)
				{
					tem_3_atm = &*ato;
					///TODO: 'atomsCompatible' won't work, use with a site atom, replace!
					if( (tem_2_atm != tem_3_atm) && (atomsCompatible(site3_atm, tem_3_atm)) )
					{
						float rmsd = numeric_limits<float>::max();
						AtomContainer test_mol( *_query );
						Vector3& tem3 = tem_3_atm->getPosition(); //assign second neighbor
						
						temp_trans = StructureMapper::matchPoints( tem1, tem2, tem3, sit1, sit2, sit3);
						transformer.setTransformation(temp_trans);
						test_mol.apply(transformer);
						
						rmsd = getMinRMSD(&site, &test_mol);
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
void StarAligner::bondAlign(Atom* atA1, Atom* atA2, Atom* atB1, Atom* atB2)
{
	Vector3& vA1 = atA1->getPosition();
	Vector3& vA2 = atA2->getPosition();
	
	Vector3& vB1 = atB1->getPosition();
	Vector3& vB2 = atB2->getPosition();
	
	// map the second pair onto the first pair:
	_matrix = twoPointMatch( vB1, vB2, vA1, vA2 );
	
	// apply the calculated transformation onto the 'query' molecule
	_transformer.setTransformation(_matrix);
	_query->apply(_transformer);
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


/**
 * getMinRMSD (1)
 */
float StarAligner::getMinRMSD(AtomContainer* mol1, AtomContainer* mol2)
{
	AtmVec vec1;
	AtmVec vec2;
	LigBase::toAtmVec(*mol1, vec1);
	LigBase::toAtmVec(*mol2, vec2);
	
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
	LigBase::toAtmVec(*mol2, vec2);
	
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
		return StructureMapper::matchPoints(t1, t2, Vector3(), s1, s2, Vector3());
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
			for(int k = i; k < atm_vec.size(); ++k)
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
		for(int j = i; j < atm_vec.size(); j++)
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
