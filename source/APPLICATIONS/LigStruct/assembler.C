#ifndef ASSEMBLER_H
#include "assembler.h"
#endif

#ifndef BALL_MOLMEC_COMMON_ATOMVECTOR_H
#include <BALL/MOLMEC/COMMON/atomVector.h>
#endif


using namespace OpenBabel;
using namespace BALL;
using namespace std;

/**
 * buildLinker
 */
void AssemblerFunctions::buildLinker(Fragment& linker_frag, 
																		 ConnectionMap& link_lib)
{
	/// Init the very first atom:
	Atom* at1 = &*linker_frag.beginAtom();
	at1->setPosition( Vector3() ); // init to (0, 0, 0)

	// iterate (recursive) over all internal (intra) bonds of the fragment, until
	// all bonds are selected (this algo won't work with cycles!!!)
	for( Atom::BondIterator bit = at1->beginBond(); +bit; ++bit)
	{
		Atom* partner = bit->getBoundAtom(*at1); 
		
		if( partner->getParent() == &linker_frag ) // restrict to intra-fragment-bonds!
			recurLinkerConnect( partner, &linker_frag, link_lib);
	}
}

/*
 * recurLinkerConnect
 */
void AssemblerFunctions::recurLinkerConnect(Atom* at, Composite * const parent, ConnectionMap& link_lib)
{
	for( Atom::BondIterator bit = at->beginBond(); +bit; ++bit)
	{
		if( !(bit->isSelected()) )
		{
			//0.) get partner atom, check if it is an intra bond, iff yes: select bond
			Atom* partner = bit->getBoundAtom(*at);
			
			if (partner->getParent() != parent) // restrict to intra-fragment-bonds!
				continue;
			
			bit->select();
			
			//1.) getSelectedSite from 'at'
			AtmVec site;
			String key;
			getSelectedSite(at, partner, site, key);
			
			//2.) connect single Atom 'partner' to site
			connectAtomToSite(site, *(link_lib[key]), partner);
			
			//3.) descend recursion with partner
			recurLinkerConnect(partner, parent, link_lib);
		}
		
	}// end loop
}


/*
 * connectSingle
 */ 
void AssemblerFunctions::connectAtomToSite(AtmVec& site, AtomContainer& temp, Atom* partner)
{
	//1.) align templ with site
	Matrix4x4 tr_matr;
	TransformationProcessor tr_proc;
	starAlign( site, temp, tr_matr );
	
	tr_proc.setTransformation(tr_matr);
	temp.apply(tr_proc);
	
	//2.) determine remaining atoms. take the first of these that is compatible with 'partner'
	AtmVec unassigned_atoms;
	getRemaining(site, temp, unassigned_atoms);
	
	Atom* tmp = getMatchingAtom(unassigned_atoms, partner);
	partner->setPosition( tmp->getPosition() );
}

/*
 * getSelectedSite
 * 
 * key codes for the complete template (all connected atoms of the fragment)
 * 
 * site contains only neighbors with selected bonds
 */
void AssemblerFunctions::getSelectedSite(Atom* atm, Atom *partner, AtmVec& site, String& key)
{
	// insert central atom for the site and the key
	site.push_back(atm);
	key = atm->getElement().getSymbol();
	
	Composite* parent = atm->getParent();
	
	// structure to sort the neighbors according to their names (element+BO)
	vector< pair<String,Atom*> > names_neighbors;
	
	// add all neighbors to 'elements' (central atom is not contained)
	for(Atom::BondIterator b_it = atm->beginBond(); +b_it; b_it++)
	{
		Atom* tmp_atm = b_it->getBoundAtom(*atm); // get neighbors of central 'atm'
		
		if(tmp_atm->getParent() == parent) // restrict to atoms in this fragment
		{
			String elem = tmp_atm->getElement().getSymbol();
			elem += String(b_it->getOrder());
			
			names_neighbors.push_back( make_pair( elem, tmp_atm) );
		}
	}
	
	// sort identifers of neighbors
	vector< pair<String, Atom*> >::iterator name_it = names_neighbors.begin();
	sort( name_it, names_neighbors.end(), compare );
	
	// create the key, and add sorted neighbors to the site
	for(name_it = names_neighbors.begin(); name_it !=names_neighbors.end(); name_it++)
	{
		key += ( (*name_it).first );

		if( ! (*name_it).second->getBond(*atm)->isSelected() ) // Do not include the partner atom into the site
			continue;
		
		site.push_back( (*name_it).second );
	}
}

/**  
 * connectFragments
 */
void AssemblerFunctions::connectFragments(Atom* atm1, Atom* atm2,
																					ConnectionMap& connectLib,
																					BondLengthMap& bondLib)
{
	///1) get connection sites of the two atoms and the corresponding templates
	AtmVec site_frag1, site_frag2;
	String key1, key2;
	getSite(atm1, site_frag1, key1);
	getSite(atm2, site_frag2, key2);
	
	AtomContainer* templ1 = connectLib[key1];
	AtomContainer* templ2 = connectLib[key2];

	Matrix4x4 trans_matr;
	TransformationProcessor transformer;
	
	///3) transfrom templ1 to match with frag1 (keep frag1 as it was)
	AtmVec vec_temp1;
	for(AtomIterator ati = templ1->beginAtom(); +ati; ++ati)
		vec_temp1.push_back(&*ati);
	
	starAlign( site_frag1, *templ1, trans_matr );
	transformer.setTransformation( trans_matr );
	
	templ1->apply( transformer );

	AtmVec remain_tmp1;
	getRemaining(site_frag1, *templ1, remain_tmp1);
	
	///4) transfrom frag2 to match with templ2
	AtomContainer* frag2 = (AtomContainer*)atm2->getParent();
	
	AtmVec vec_temp2;
	for(AtomIterator ati = templ2->beginAtom(); +ati; ++ati)
		vec_temp2.push_back(&*ati);
	
	starAlign( site_frag2, *templ2, trans_matr );
	transformer.setTransformation( trans_matr );
	
	frag2->apply( transformer );
	
	AtmVec remain_tmp2;
	getRemaining(site_frag2, *templ2, remain_tmp2);
	
	///5) transfrom the connection bond determined for temp2 to the one determined
	///   for temp1.
	bondAlign(atm1, remain_tmp1[0], remain_tmp2[0], atm2, trans_matr);
	transformer.setTransformation( trans_matr );
	
	frag2->apply( transformer );
	
	///6) set bond length to standard length
	Vector3 bond_fix = getDiffVec(atm1, atm2, bondLib);
	TranslationProcessor t_later(bond_fix);
	
	frag2->apply(t_later);
}

/** 
 * starAlign, finds:
 * - transformation matrix that fits best
 * 
 * preconditions:
 * - vec1 and vec2 are of at least size 1
 * - vec1 and vec2 contain at pos 0 the central atom of a star molecule
 * - vec1 may be smaller than vec2 (but not the other way around)
 */
void AssemblerFunctions::starAlign(AtmVec& vec1, AtomContainer &mol2, Matrix4x4& trans_matrix)
{
	
}

//Atom *AssemblerFunctions::starAlign(AtmVec& mol1, Atom* partner, AtomContainer* mol2, Matrix4x4& trans_matrix)
//{
//	AtomContainer mol2_copy(*mol2); /// We need a copy if we test several transformations
	
//	Vector3 frag2, frag3, tem2, tem3; //the vectors to calc the transformaion
//	TransformationProcessor transformer;
	
//	// set the two center atoms (which are always the first atoms):
//	Vector3& frag1 = mol1[0]->getPosition();
//	Vector3& tem1  = mol2_copy->beginAtom()->getPosition();
	
//	/// 0.) simple solutions for only one or two neighbors
//	// the site (mol1) does not contain the connection atom and thus has one atom less than in mol2
//	int num_atoms = mol1.size();
	
//	/// 0.2) TODO: Case of no neighbor?!
//	if(num_atoms == 2)
//	{
//		///0.1) only the center and another atom exists in the mother fragment.
//		///     in this case simply use that other atom on the frag1 side.
//		///     For frag2 find the atom matching that atom. Return the remaining atom in frag2 as partner
//		if(num_atoms == 2) /// WARNING: could not yet test this case!!!
//		{
//			// position 0 has always the center, leaves pos 1 as the only possible partner
//			frag2 = mol1[1]->getPosition();
//			tem2  = mol2_copy->getAtom(1)->getPosition();
			
//			// create dummys to use the 3 point matching
//			frag3 = Vector3();
//			tem3  = Vector3();
//		}
//		/// TODO: there has to be a decision which mapping is better for the two alternatives!!!
//		/// (if both non-central-atoms are identical, otherwise the current solution is okay)
		
//		///0.2) Beside the center atom, 2 others exist. perhaps this is not really a simple case.
//		///     remove this part and fit the surronding if-clause!
//		else if(num_atoms == 3)
//		{
//			/// MAL GANZ IM ERNST: das geht so alles nit!!!! damüssma nomma ran!
//			// get correct atom assignment:
//			Atom *temp_atA2, *temp_atA3, *temp_atB2, *temp_atB3;
			
//			temp_atA2 = mol1[1];
//			temp_atB2 = getMatchingAtom(mol2_copy, temp_atA2);
			
//			temp_atA3 = mol1[2];
//			// the respective remaining atom in mol2 is the one we need for 'temp_atB3'
//			temp_atB3 = mol2_copy->getAtom(2);
//			if(temp_atB3 == temp_atB2)
//				temp_atB3 = mol2_copy->getAtom(1);
			
//			// get positions:
//			frag2 = temp_atA2->getPosition();
//			frag3 = temp_atA3->getPosition();
//			tem2  = temp_atB2->getPosition();
//			tem3  = temp_atB3->getPosition();
//		}
//		trans_matrix = StructureMapper::matchPoints(frag1, frag2, frag3, tem1, tem2, tem3);
		
//		///TODO: find the real one that is compatible with 'partner'
//		return mol2->getAtom(3);
//	}
	
//	/// 1.) More than 1 neighbor is given. Find all unique atoms in mol1
//	/// TODO: remember to include the partner atom in the uniqueness search!
//	AtmVec unique_atms;
//	unique_atms.reserve(8);
//	getUniqueAtoms(mol1, unique_atms);

//	///2.) Case of at last two unique atoms beside the central atom. In this case 
//	/// we may simply use these and are done!
//	if (unique_atms.size() > 1)
//	{
//		frag1 = mol1[0]->getPosition(); 
//		frag2 = unique_atms[0]->getPosition();
//		frag3 = unique_atms[1]->getPosition();
		
//		tem1 = mol2_copy->getAtom(0)->getPosition();
//		tem2 = getMatchingAtom( mol2_copy, unique_atms[0] )->getPosition();
//		tem3 = getMatchingAtom( mol2_copy, unique_atms[1] )->getPosition();
		
//		// get transformation:
//		trans_matrix = StructureMapper::matchPoints(frag1, frag2, frag3, tem1, tem2, tem3);
		
//		return;
//	}

//	///3.) Just one unique atom was found, here we need to test some transformations
//	///    to find the best match for the second neighbor atom.
//	if (unique_atms.size() == 1) { 
//		// if at least one mol1 atom was unique, it is curcial to use it:
//		Atom* uniqueA = unique_atms[0];
//		Atom* uniqueB = getMatchingAtom( mol2_copy, uniqueA );
		
//		// get next non unique atom from mol1:
//		Atom* sel=0;
//		AtmVec::iterator ato = mol1.begin();
//		for(ato++; ato!=mol1.end(); ato++)
//		{
//			if(*ato != uniqueA){
//				sel = *ato;
//				break;
//			}
//		}
		
//		// test all possible assignments of the selected neighbor atom of mol1
//		// to an atom of mol2
//		float best_rmsd = numeric_limits<float>::max();
		
//		AtomIterator ati = mol2_copy->beginAtom();
//		for(ati++; +ati; ati++)
//		{
//			float rmsd = numeric_limits<float>::max();
//			if(&*ati != uniqueB)
//			{
//				Atom* selectionA1 = mol1[0];Atom* selectionB1 = mol2_copy->getAtom(0);
//				frag1 = selectionA1->getPosition(); tem1  = selectionB1->getPosition();
//				frag2 = uniqueA->getPosition();
//				tem2  = uniqueB->getPosition();
//				frag3 = sel->getPosition();
//				tem3  = ati->getPosition();
				
//				trans_matrix = StructureMapper::matchPoints( tem1, tem2, tem3, frag1, frag2, frag3);
//				transformer.setTransformation(trans_matrix);
//				mol2_copy->apply(transformer);
				
//				rmsd = getMinRMSD(&mol1, mol2_copy);
//				if (rmsd < best_rmsd)
//					best_rmsd = rmsd;
//			}
//		}
//		return;
//	}
//	///4.) No unique atom was found. Here we have to check every possible
//	///    assignment for the two remaining atoms...
//	float best_rmsd = numeric_limits<float>::max();
//	Atom* selectionA1 = mol1[0];
//	Atom* selectionA2 = mol1[1];
//	Atom* selectionA3 = mol1[2];
	
//	Atom* selectionB1 = mol2_copy->getAtom(0);
//	Atom* selectionB2 = 0;
//	Atom* selectionB3 = 0;
	
//	// test all possible assignments to 'selectionA2'
//	AtomIterator ati = mol2_copy->beginAtom();
//	Matrix4x4 best_trans;
//	for(ati++; +ati; ati++)
//	{
//		selectionB2 = &*ati;
//		if( atomsCompatible(selectionA2, selectionB2) )
//		{
//			// test all possible assignments to 'selectionA3'
//			AtomIterator ato = mol2_copy->beginAtom();
//			for(ato++; +ato; ato++)
//			{
//				selectionB3 = &*ato;
//				if( (selectionB2 == selectionB3) || !(atomsCompatible(selectionA3, selectionB3)) )
//					continue;
				
//				float rmsd = numeric_limits<float>::max();
//				frag1 = selectionA1->getPosition(); tem1  = selectionB1->getPosition();
//				frag2 = selectionA2->getPosition(); tem2  = selectionB2->getPosition();
//				frag3 = selectionA3->getPosition(); tem3  = selectionB3->getPosition();
					
//				trans_matrix = StructureMapper::matchPoints( tem1, tem2, tem3, frag1, frag2, frag3);
//				transformer.setTransformation(trans_matrix);
//				mol2_copy->apply(transformer);
				
//				rmsd = getMinRMSD(&mol1, mol2_copy);
//				if (rmsd < best_rmsd)
//				{
//					best_rmsd = rmsd;
//					best_trans = trans_matrix;
//				}
//			}
//		}
//	}
//	trans_matrix = best_trans;
//	return;
//}

/**
 * bondAlign
 */
void AssemblerFunctions::bondAlign(Atom* atA1, Atom* atA2, 
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

/**
 * getMinRMSD (1)
 */
float AssemblerFunctions::getMinRMSD(AtomContainer* mol1, AtomContainer* mol2)
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
float AssemblerFunctions::getMinRMSD(AtmVec* vec1, AtomContainer* mol2)
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
float AssemblerFunctions::getMinRMSD(AtmVec* vec1, AtmVec* vec2)
{
	float min_sq_dist = numeric_limits<float>::max();
	
	AtmVec::iterator ati = vec1->begin(); ++ati;
	AtmVec::iterator end1 = vec1->end();
	sqdistPerPermutation( ati, end1, *vec2, 1, 0, &min_sq_dist);
	
	min_sq_dist = sqrt( min_sq_dist / (float)(vec1->size() - 1) );
	return min_sq_dist;
}

/*
 * append all atoms in result that 'remain'. Meaning atoms of templ that do not
 * optimally match with any atom in site
 */
void AssemblerFunctions::getRemaining(AtmVec& site, AtomContainer& templ, AtmVec& result)
{
	AtmVec vec2;
	fromMoleculeToAtmVec(templ, vec2);
	
	// the 'sum of all square distances' for the best (minimal) permutation:
	float min_sq_dist = numeric_limits<float>::max(); 
	
	AtmVec::iterator ati = site.begin(); ++ati;  // start with second atom (first is central atom)
	AtmVec::iterator end1 = site.end();
	matchPermutaions( ati, end1, vec2, 1, 0, &min_sq_dist, result);
}

/*
 * matchPermutaions
 */
void AssemblerFunctions::matchPermutaions(AVIter& ati1, AVIter& end1, 
																					AtmVec& atm_vec, int i, 
																					float loc_sq_dist, float* global_sq_dist,
																					AtmVec& result)
{
	// end recursion case: 
	// everything was permuted so check how good the square dist was and perhaps
	// update the global sq_dist
	if( ati1 == end1 )
	{
		if( (*global_sq_dist) > loc_sq_dist)
		{
			*global_sq_dist = loc_sq_dist;
			
			// insert into cleared result vector
			result.clear();
			for(int k = i; k < atm_vec.size(); k++)
				result.push_back( atm_vec[k] );
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
			if(  atomsCompatible( *ati1, atm_vec[j] )  )
			{
				sq_dist_update = (*ati1)->getPosition().getSquareDistance( atm_vec[j]->getPosition() );
				
				swapAtoms(atm_vec[i], atm_vec[j]); // permute the vector entries
				
				AtmVec::iterator ati2 = ati1; // create new atom iterator for next recursion
				matchPermutaions( ++ati2, end1, atm_vec, (i+1), (loc_sq_dist + sq_dist_update),
													global_sq_dist, result);
				
				swapAtoms(atm_vec[i], atm_vec[j]); // undo the swap for next itertation
			}
		} // end for-loop
	}
}

/*
 * sqdistForPermutations
 */
void AssemblerFunctions::sqdistPerPermutation(AVIter& ati1, AVIter& end1,
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
 * fromMoleculeToAtmVec
 */
void AssemblerFunctions::fromMoleculeToAtmVec(AtomContainer& in_mol, AtmVec& out_vec)
{
	for(AtomIterator ati = in_mol.beginAtom(); +ati; ++ati)
	{
		out_vec.push_back(&*ati);
	}
	
}

/*
 * getSite
 */
void AssemblerFunctions::getSite(Atom* atm, AtmVec &site, String& key)
{
	// insert central atom for the site and the key
	site.push_back(atm);
	key = atm->getElement().getSymbol();
	
	Composite* parent = atm->getParent();
	
	// structure to sort the neighbors according to their names (element+BO)
	vector< pair<String,Atom*> > names_neighbors;
	
	// add all neighbors to 'elements' (central atom is not contained)
	for(Atom::BondIterator b_it = atm->beginBond(); +b_it; b_it++)
	{
		Atom* tmp_atm = b_it->getBoundAtom(*atm); // get neighbors of central 'atm'
		
		String elem = tmp_atm->getElement().getSymbol();
		elem += String(b_it->getOrder());
			
		names_neighbors.push_back( make_pair( elem, tmp_atm) );
	}
	
	// sort identifers of neighbors
	vector< pair<String, Atom*> >::iterator name_it = names_neighbors.begin();
	sort( name_it, names_neighbors.end(), compare );
	
	// create the key, and add sorted neighbors to the site
	for(name_it = names_neighbors.begin(); name_it !=names_neighbors.end(); name_it++)
	{
		key += ( (*name_it).first );
		
		if( (*name_it).second->getParent() != parent) // Don't include partner atoms into the site
			continue;
		
		site.push_back( (*name_it).second );
	}
}

///*
// * handleSimpleConnections
// */
//void AssemblerFunctions::handleSimpleConnections( Atom* atm1, Atom* atm2,
//															boost::unordered_map <String, float >& bondLib,
//															boost::unordered_map <String, Fragment* >& connectLib)
//{
//	Fragment* frag1 = (Fragment*)atm1->getParent();
//	Fragment* frag2 = (Fragment*)atm2->getParent();
	
//	Matrix4x4 trans;
//	// create a new bond between the two atoms:
//	Bond* bnd = new Bond();
//	bnd->setOrder(1);
//	atm1->createBond(*bnd, *atm2);
	
//	AtmVec site; String site_key; int pos = -1; AtomContainer* templ;
	
//	// both fragments have just one atom:
//	// keep atom1 and move atom2 in just one direction about the std bond length
//	if( frag1->countAtoms() + frag2->countAtoms() == 2)
//	{
//		String bond_key = atm1->getElement().getSymbol();
//		bond_key += atm2->getElement().getSymbol();
		
//		atm2->setPosition( atm1->getPosition() );
//		atm2->getPosition().x += bondLib[bond_key];

//		return; // return early
//	}
//	// frag 2 is a single atom
//	else if(frag2->countAtoms() == 1)
//	{
//		pos = getSiteWithPos(atm1, site, atm2, site_key);
//		templ = new AtomContainer(*connectLib[site_key]);
		
//		atm2->setPosition( templ->getAtom(pos)->getPosition() );
//		templ->remove( *templ->getAtom(pos) );
		
//		// rotate the single_frag so that it aligns with the template
//		starAlign(site, templ, trans);
//		TransformationProcessor transformer(trans);
//		frag1->apply(transformer);
//	}
//	// frag 1 is a single atom (should seldomly occur)
//	else
//	{
//		pos = getSiteWithPos(atm2, site, atm1, site_key);
//		templ = new AtomContainer(*connectLib[site_key]);
		
//		atm1->setPosition( templ->getAtom(pos)->getPosition() );
//		templ->remove( *templ->getAtom(pos) );

//		// rotate the single_frag so that it aligns with the template
//		starAlign(site, templ, trans);
//		TransformationProcessor transformer(trans);
//		frag2->apply(transformer);
//	}
//}

/*
 * getUniqueAtoms
 */
///TODO: test!!!
void AssemblerFunctions::getUniqueAtoms(AtomContainer* mol1, AtmVec& unique_atms)
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
 * getUniqueAtoms
 */
///TODO: test!!!
void AssemblerFunctions::getUniqueAtoms(AtmVec& mol1, AtmVec& unique_atms)
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
 * getDiffVec
 */
Vector3 AssemblerFunctions::getDiffVec(Atom* atm1, Atom* atm2, BondLengthMap std_bonds)
{
	String key = atm1->getElement().getSymbol();
	key += atm2->getElement().getSymbol();
	float bond_len = std_bonds[key];
	
	Vector3 diff_vec = atm1->getPosition() - atm2->getPosition();
	float diff_len = diff_vec.getLength() - bond_len;
	
	diff_vec.normalize();
	return (diff_vec * diff_len);
}

/*
 * compare
 */
bool AssemblerFunctions::compare(pair<String,Atom*>& a, pair<String,Atom*>& b)
{
	return a.first < b.first;
}

/*
 * atomsCompatible
 */
bool AssemblerFunctions::atomsCompatible(Atom* at1,Atom* at2)
{
	return ( 
					 ( at1->getElement()            == at2->getElement() ) && 
					 ( at1->beginBond()->getOrder() == at2->beginBond()->getOrder()) 
				 );
}

/*
 * swapAtoms
 */
void AssemblerFunctions::swapAtoms(Atom*& a, Atom*& b)
{
	Atom* tmp = a;
	a = b;
	b = tmp;
}

/* 
 * getMatchingAtom(1)
 */
Atom* AssemblerFunctions::getMatchingAtom(AtomContainer* mol, Atom* atm)
{
	const String& elem = atm->getElement().getSymbol();
	short bo = atm->beginBond()->getOrder(); //should have only one bond

	AtomIterator ati = mol->beginAtom();
	ati++; // first atom is the center atom, which we never want
	for(; +ati; ++ati)
	{
		if( ati->getElement().getSymbol() == elem && ati->beginBond()->getOrder() == bo )
			return &*ati;
	}
	
	cout<<"ERROR: could not find a partner Atom"<<endl;
	exit(EXIT_FAILURE);
}

/* 
 * getMatchingAtom(2)
 */
Atom* AssemblerFunctions::getMatchingAtom(AtmVec& mol, Atom* atm)
{
	const String& elem = atm->getElement().getSymbol();
	short bo = atm->beginBond()->getOrder(); //should have only one bond

	AVIter ati = mol.begin();
	ati++; // first atom is the center atom, which we never want
	for(; ati != mol.end(); ++ati)
	{
		if( (*ati)->getElement().getSymbol() == elem && (*ati)->beginBond()->getOrder() == bo )
			return *ati;
	}
	
	cout<<"ERROR: could not find a partner Atom"<<endl;
	exit(EXIT_FAILURE);
}
/*
 * getBondName
 */
String AssemblerFunctions::getBondName(Atom* atm, Atom* partner)
{
	String name = atm->getElement().getSymbol();
	name += atm->getBond(*partner)->getOrder();
	return name;
}

/*
 * twoPointMatch
 * TODO: reimplement more efficiently
 */
Matrix4x4 AssemblerFunctions::twoPointMatch(const Vector3& v1, 
																									 const Vector3& v2, 
																									 const Vector3& u1,
																									 const Vector3& u2)
{
	return StructureMapper::matchPoints(v1, v2, Vector3(), u1, u2, Vector3());
}

