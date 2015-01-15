#ifndef ASSEMBLER_H
#include "assembler.h"
#endif

#ifndef BALL_MOLMEC_COMMON_ATOMVECTOR_H
#include <BALL/MOLMEC/COMMON/atomVector.h>
#endif


using namespace OpenBabel;
using namespace BALL;
using namespace std;

String printInlineMol(Composite* mol)
{
	
	String tmp = "";
	for(AtomIterator ati = ((AtomContainer*)mol)->beginAtom(); +ati; ++ati)
	{
		tmp += ati->getElement().getSymbol();
		if( &*ati != &*((AtomContainer*)mol)->beginAtom())
			tmp += String(ati->getBond( *((AtomContainer*)mol)->beginAtom() )->getOrder());
	}
	return tmp;
}

String printInlineMol(AtmVec& mol)
{
	
	String tmp = "";
	Atom* center = *mol.begin();
	for(AVIter ati = mol.begin(); ati != mol.end(); ++ati)
	{
		tmp += (*ati)->getElement().getSymbol();
		if( *ati != center )
			tmp += String((*ati)->getBond( *center )->getOrder());
	}
	return tmp;
}
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
			
			if (partner->getParent() == parent) // restrict to intra-fragment-bonds!
			{
				bit->select();
				
				//1.) getSelectedSite from 'at'
				AtmVec site;
				String key;
				getSelectedSite(at, site, key);
				
				//2.) connect single Atom 'partner' to site
				connectAtomToSite(site, *(link_lib[key]), partner);
				
				//3.) descend recursion with partner
				recurLinkerConnect(partner, parent, link_lib);
			}
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
	
	//2.) determine remaining atoms. take the first one that is compatible with 'partner'
	AtmVec unassigned_atoms;
	getRemaining(site, temp, unassigned_atoms);
	
	String elem = partner->getElement().getSymbol();
	short bo = partner->getBond( *site[0])->getOrder();
	Atom* tmp = getMatchingAtom( &*temp.beginAtom(), unassigned_atoms, elem, bo );
	partner->setPosition( tmp->getPosition() );
}

/*
 * getSelectedSite
 * 
 * key codes for the complete template (all connected atoms of the fragment)
 * 
 * site contains only neighbors with selected bonds
 */
void AssemblerFunctions::getSelectedSite(Atom* atm, AtmVec& site, String& key)
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
	cout<<"####Step1"<<endl;
	AtmVec site_frag1, site_frag2;
	String key1, key2;
	cout<<"####Step1 - Site1"<<endl;
	getSite(atm1, site_frag1, key1);
	cout<<"####Step1 - Site2"<<endl;
	getSite(atm2, site_frag2, key2);
	
	cout<<"####Step1 - C-Lib lookup 1: "<<key1<<endl;
	AtomContainer* templ1 = new AtomContainer( *connectLib[key1]);
	cout<<"####Step1 - C-Lib lookup 2: "<<key2<<endl;
	AtomContainer* templ2 = new AtomContainer( *connectLib[key2] );
	
//	// DEBUG
//	cout<< "Site1: "<< printInlineMol(site_frag1)<<endl;
//	cout<< "Site2: "<< printInlineMol(site_frag2)<<endl;
//	cout<< "key1: "<< key1<<endl;
//	cout<< "key2: "<< key2<<endl;
//	cout<< "tmp1: "<< printInlineMol(templ1)<<endl;
//	cout<< "tmp2: "<< printInlineMol(templ2)<<endl;
//	// DEBUG - end

	Matrix4x4 trans_matr;
	TransformationProcessor transformer;
	cout<<"####Step1 - done"<<endl;
	
	///2) transfrom templ1 to match with frag1 (keep frag1 as it was)
	cout<<"####Step2"<<endl;
	
	starAlign( site_frag1, *templ1, trans_matr );
	transformer.setTransformation( trans_matr );
	
	templ1->apply( transformer );
	
	AtmVec remain_tmp1;
	getRemaining(site_frag1, *templ1, remain_tmp1);
	
	///3) transfrom templ2 to match with frag2
	cout<<"####Step3"<<endl;
	AtomContainer* frag2 = (AtomContainer*)atm2->getParent();
//	cout<<"    got partent"<<endl;
	
	starAlign( site_frag2, *templ2, trans_matr );
//	cout<<"    aligned"<<endl;
	transformer.setTransformation( trans_matr );
	templ2->apply( transformer );
//	cout<<"    transformed"<<endl;
	
	AtmVec remain_tmp2;
	getRemaining(site_frag2, *templ2, remain_tmp2);
//	cout<<"    got remaining"<<endl;
	
	///4) transfrom the connection bond determined for temp2 to the one determined
	///   for temp1.
	cout<<"####Step4"<<endl;
	String elem2 = atm2->getElement().getSymbol();
	short bo2    = atm2->getBond(*atm1)->getOrder();
	
	String elem1 = atm1->getElement().getSymbol();
	short bo1    = atm1->getBond(*atm2)->getOrder();
	cout<<"Step4:bond align"<<endl;
	Atom* atm1_partner = getMatchingAtomAll( &*templ1->beginAtom(), remain_tmp1, elem2, bo2);
	Atom* atm2_partner = getMatchingAtomAll( &*templ2->beginAtom(), remain_tmp2, elem1, bo1);
	
//	//DEBUG
//	SDFile tmp_out("/Users/pbrach/SITE_"+String((unsigned long)atm1)+".sdf", ios::out);
//	tmp_out << *((AtomContainer*)atm1->getParent());
//	tmp_out << *templ1;
//	Molecule dummy1; dummy1.insert(*(new Atom(*atm1_partner)));
//	tmp_out << dummy1;
	
//	tmp_out << *((AtomContainer*)atm2->getParent());
//	tmp_out << *templ2;
//	Molecule dummy2; dummy2.insert(*(new Atom(*atm2_partner)));
//	tmp_out << dummy2;
//	tmp_out.close();
//	//DEBUG - END
	cout<<"found partner"<<endl;
	bondAlign(atm1, atm1_partner, atm2_partner, atm2, trans_matr);
	cout<<"aligned"<<endl;
	transformer.setTransformation( trans_matr );
	frag2->apply( transformer );
	cout<<"transformed"<<endl;
	
	///5) set bond length to standard length
	cout<<"####Step5"<<endl;
	Vector3 bond_fix = getDiffVec(atm1, atm2, bondLib);
//	cout<<"bond fix: "<<bond_fix<<endl;
	TranslationProcessor t_later(bond_fix);
	
	frag2->apply(t_later);
//	cout<<endl<<endl<<" -   F i n i s h e d   - "<<endl;
	delete templ1;
	delete templ2;
}

/** 
 * starAlign, finds:
 * - matrix that transforms 'templ' onto 'site'
 * - the transformation is selected that fits best
 * 
 * preconditions:
 * - 'site' and 'templ' are of at least size 1
 * - 'site' and 'templ' contain at pos 0 the central atom of a star molecule
 * - 'site' may be smaller than 'templ' (but not the other way around)
 */
void AssemblerFunctions::starAlign(AtmVec& site, AtomContainer &templ, Matrix4x4& trans_matrix)
{
	/// Case 1) 'site' has size 1, thus we can only translate the center 
	if ( site.size() == 1)
	{
//		cout<<"Align: got case1"<<endl;
		Vector3 transl = site[0]->getPosition() - templ.beginAtom()->getPosition();
		trans_matrix.setTranslation(transl);
	}
	/// Case 2) 'site' contains 1 neighbor. Find best match for it in 'templ' and 2pointMatch.
	else if ( site.size() == 2)
	{
//		cout<<"Align: got case2"<<endl;
		Vector3& sit1 = site[0]->getPosition();
		Vector3& sit2 = site[1]->getPosition();
		float sq_dist12 = sit1.getSquareDistance( sit2 ); // get dist to single neighbor in site
		
		AtomIterator ati = templ.beginAtom();
		Vector3& tem1 = ati->getPosition();
		Vector3 tem2;
		
		const Element& elem = site[1]->getElement();
		short     bo = site[1]->getBond( *site[0] )->getOrder();
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
		trans_matrix = twoPointMatch(tem1, tem2, sit1, sit2);
	}
	/// Case 3) 'site' has at least 2 neighbors
	else
	{
//		cout<<"Align: got case3"<<endl;
		alignCase3(site, templ, trans_matrix);
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
void AssemblerFunctions::alignCase3(AtmVec& site, AtomContainer &templ, Matrix4x4& trans_matrix)
{
	// find all unique atoms in 'site' (according to BO, cental atom and element)
	AtmVec unique_atms;
	unique_atms.reserve(8);
//	cout<<"getUnique"<<endl;
	getUniqueAtoms(site, unique_atms);
//	cout<<"got them"<<endl;
	
	/// SubCase 1) at least two unique atoms, straight computation of transformation
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
	
	
	/// SubCase 2) Only one unique atm exists, use this one and find best 
	///            assignment for a second neighbor from 'site'
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
	
	
	/// SubCase 3) No unique atom exists. Find best assignment for two neighbors
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
 * append all atoms in 'result' that remain. Meaning atoms of 'templ' that do not
 * optimally match with any atom in 'site'
 */
/// TODO: the recursion can be made nicer if using indices and 'site'
void AssemblerFunctions::getRemaining(AtmVec& site, AtomContainer& templ, AtmVec& result)
{
	AtmVec vec2;
	fromMoleculeToAtmVec(templ, vec2);
	
	// the 'sum of all square distances' for the best (minimal) permutation:
	float min_sq_dist = numeric_limits<float>::max(); 
	
	AtmVec::iterator ati = site.begin(); ++ati;  // start with second atom (first is central atom)
	AtmVec::iterator end1 = site.end();
	matchPermutaions(*site[0], ati, end1, vec2, 1, 0, &min_sq_dist, result);
	
	
	// D E B U G :
	cout<<endl<<"'getRemaining':"<<endl;
	cout<<"templ    : "<<printInlineMol(&templ)<<endl;
	cout<<"site     : "<<printInlineMol(site)<<endl;
	cout<<"remainder: ";
	for(AVIter at = result.begin(); at != result.end(); ++at)
	{
		Atom* atm = *at;
		Atom* center = &*templ.beginAtom();
		cout<< atm->getElement().getSymbol();
		if(atm != center )
			cout<< atm->getBond( *center )->getOrder();
	}
	cout<<endl;
	cout<<"sq dist: "<< min_sq_dist <<endl<<endl;
}

/*
 * matchPermutaions
 */
void AssemblerFunctions::matchPermutaions(Atom& center,
																					AVIter& ati1, AVIter& end1, 
																					AtmVec& atm_vec, int i, 
																					float loc_sq_dist, float* global_sq_dist,
																					AtmVec& result)
{
//	cout<<i;
	// end recursion case: 
	// everything was permuted so check how good the square dist was and perhaps
	// update the global sq_dist
	if( ati1 == end1 )
	{
		if( loc_sq_dist < (*global_sq_dist) )
		{
			*global_sq_dist = loc_sq_dist;
			
			// insert into cleared result vector
			result.clear();
			for(int k = i; k < atm_vec.size(); ++k)
				result.push_back( atm_vec[k] );
//			cout<<endl<<"SD: "<<  loc_sq_dist<<endl;
//			cout<<printInlineMol(atm_vec)<<endl;
			
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
			if( (*ati1)->getElement().getSymbol() == atm_vec[j]->getElement().getSymbol() &&
					(*ati1)->getBond( center )->getOrder()  == atm_vec[j]->beginBond()->getOrder() )
			{
//				cout<<" MATCH "<<(*ati1)->beginBond()->getOrder()<<" - "<<atm_vec[j]->beginBond()->getOrder()<<endl;
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
		key += (*name_it).first;
		
		if( (*name_it).second->getParent() == parent) 
			site.push_back( (*name_it).second );
	}
}

/*
 * getUniqueAtoms (1)
 */
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
 * getUniqueAtoms (2)
 */
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
	
	if( diff_vec.isZero() )
	{
		Log<<"ERROR: distance between connection site seems to be zero"<<endl;
	}
	else
	{
		diff_vec.normalize();
	}
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
 * TODO: improve this version. Perhaps by using selections for sites and 
 * restricting to bonds that are selected?
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
 * getMatchingAtomAll(3)
 */
Atom* AssemblerFunctions::getMatchingAtomAll(Atom *center, AtmVec& mol, String& elem, short bo)
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

/* 
 * getMatchingAtom(1)
 */
Atom* AssemblerFunctions::getMatchingAtom(Atom *center, AtmVec& mol, String& elem, short bo)
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
Atom* AssemblerFunctions::getMatchingAtom(Atom* center, AtomContainer* mol, String &elem,  short bo)
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
Matrix4x4 AssemblerFunctions::twoPointMatch(const Vector3& n1, 
																									 const Vector3& n2, 
																									 const Vector3& w1,
																									 const Vector3& w2)
{
	return StructureMapper::matchPoints(n1, n2, Vector3(), w1, w2, Vector3());
}

