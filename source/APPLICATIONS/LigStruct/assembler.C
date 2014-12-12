#ifndef ASSEMBLER_H
#include "assembler.h"
#endif

using namespace OpenBabel;
using namespace BALL;
using namespace std;

/**
 * buildLinker
 */
void AssemblerFunctions::buildLinker(vector< Fragment* >& linker_lst)
{
	
}

/**  
 * connectFragments
 */
void AssemblerFunctions::connectFragments(Atom* atm1, Atom* atm2,
																					ConnectionMap& connectLib,
																					BondLengthMap& bondLib)
{
	Fragment* frag1 = (Fragment*)atm1->getParent();
	Fragment* frag2 = (Fragment*)atm2->getParent();
	
//	cout<<"Connecting Atoms "<<con1<< " and "<< con2<<endl;
//	cout<<"Connecting Fragments "<<frag1<< " and "<< frag2<<endl;
	
	///0) Check for trivial cases of one being a single atom or both being single atoms
	if(frag1->countAtoms() == 1 || frag2->countAtoms() == 1)
	{
		handleSimpleConnections(atm1, atm2, bondLib, connectLib);
	}
	
//	cout<<"searching sites..."<<endl<<endl;
	///1) find connection sites from the two atom pointers
	///   (pos1/2 tells us where in a connection template the connected atom lies)
	AtmVec site1, site2;
	String key1, key2;
	int pos1 = getSite(atm1, site1, atm2, key1);
	int pos2 = getSite(atm2, site2, atm1, key2);
	
//	cout<<"site key1: "<<key1<<endl;
//	cout<<"site key2: "<<key2<<endl<<endl;
//	cout<<"......done!"<<endl<<endl;
	
//	cout<<"searching templates..."<<endl;
	///2) find the corresponding templates
	// create working_copies to keep the originals save!
	AtomContainer* templ1 = new AtomContainer(*connectLib[key1]);
	AtomContainer* templ2 = new AtomContainer(*connectLib[key2]);
	
//	cout<< "found templates: "<< connectLib[key1] <<" "<<templ1->countAtoms() <<" - "<<connectLib[key2]<<" "<<templ2->countAtoms()<<endl;
//	cout<<"......done!"<<endl<<endl;
	
//	cout<<"creating connected template..."<<endl;
	///3) connect the two templates to one new template
	mergeTemplates(templ1, pos1, templ2, pos2, bondLib);
//	cout<<"......done!"<<endl<<endl;
	
//	cout<<"calculating transformations for fragments to the connected template..."<<endl;
	///4) rotate both sites to match the template
	Matrix4x4 trans1 = align(site1, templ1);
	Matrix4x4 trans2 = align(site2, templ2);
//	cout<<"......done!"<<endl<<endl;
	
//	cout<<"applying the transformations to both fragments..."<<endl;
	TransformationProcessor transformer;
	transformer.setTransformation(trans1);
	frag1->apply(transformer);
	
	transformer.setTransformation(trans2);
	frag2->apply(transformer);
//	cout<<"......done!"<<endl<<endl;
	
	
//	cout<<"writing correct molecule type..."<<endl;
	///5) finishing molecule connection on data-type level:
	// form new bond:
	Bond* bnd = new Bond;
	bnd->setOrder(1);
	atm1->createBond( *bnd,*(atm2) );
	
	///6) clean up:
	delete templ1;
	delete templ2;
}

/** 
 * starAlign
 */
/*
 * My current solution is:
 *   1.) Catch trivial cases of: 1 central atom and 1 neighbor, or 2 neighbors
 *       (here we directly know the optimal transformation)
 *   
 *   2.) Find unique atoms. If two could be found, solve the second trivial case
 *   
 *   3.) Selection of first neighbor:
 *       Check if a single unique atom exists, if so use it otherwise select 
 *       the first neighbor atom (second atom entry) 
 *   
 *       Find in the other molecule the first atom with identical Element+BO
 *   
 *   4.) Selection of second neighbor:
 *       Check if another unique atom exists among the remaining atoms. If so
 *       use it (and the respecctive partner in mol2) and you are done. 
 *   
 *       Otherwise, select any not selected atom from mol1 and try all possible 
 *       pairings with the remaining atoms in mol2. Select the pairing that 
 *       yields the lowest RMSD.
 */
Matrix4x4 AssemblerFunctions::starAlign(AtmVec& mol1, AtomContainer* mol2)
{
	Vector3 frag2, frag3, tem2, tem3; //the vectors to calc the transformaion
	Matrix4x4 trans_matrix;
	TransformationProcessor transformer;
	
	// set the two center atoms (which are always the first atoms):
	Vector3& frag1 = mol1[0]->getPosition();
	Vector3& tem1  = mol2->beginAtom()->getPosition();
	
	/// 0.) simple solutions for only one or two neighbors
	int num_atoms = mol1.size();
	if(num_atoms < 4)
	{
		if(num_atoms == 2) /// WARNING: could not yet test this case!!!
		{
			// position 0 has always the center, leaves pos 1 as the only possible partner
			frag2 = mol1[1]->getPosition();
			tem2  = mol2->getAtom(1)->getPosition();
			
			// create dummys to use the 3 point matching
			frag3 = Vector3();
			tem3  = Vector3();
		}
		/// TODO: there has to be a decision which mapping is better for the two alternatives!!!
		/// (if both non-central-atoms are identical, otherwise the current solution is okay)
		else if(num_atoms == 3)
		{
			// get correct atom assignment:
			Atom *temp_atA2, *temp_atA3, *temp_atB2, *temp_atB3;
			
			temp_atA2 = mol1[1];
			temp_atB2 = getMatchingAtom(mol2, temp_atA2);
			
			temp_atA3 = mol1[2];
			// the respective remaining atom in mol2 is the one we need for 'temp_atB3'
			temp_atB3 = mol2->getAtom(2);
			if(temp_atB3 == temp_atB2)
				temp_atB3 = mol2->getAtom(1);
			
			// get positions:
			frag2 = temp_atA2->getPosition();
			frag3 = temp_atA3->getPosition();
			tem2  = temp_atB2->getPosition();
			tem3  = temp_atB3->getPosition();
		}
		trans_matrix = StructureMapper::matchPoints(frag1, frag2, frag3, tem1, tem2, tem3);
		
		return trans_matrix;
	}
	
	/// 1.) More than 3 neighbors are given. Find all unique atoms in mol1
	AtmVec unique_atms;
	unique_atms.reserve(8);
	getUniqueAtoms(mol1, unique_atms);

	///2.) Case of at last two unique atoms beside the central atom. In this case 
	/// we may simply use these and are done!
	if (unique_atms.size() > 1)
	{
		Atom* selectionA1 = mol1[0];
		frag1 = selectionA1->getPosition(); 
		frag2 = unique_atms[0]->getPosition();
		frag3 = unique_atms[1]->getPosition();
		
		Atom* selectionB1 = mol2->getAtom(0);
		tem1  = selectionB1->getPosition();
		tem2 = getMatchingAtom( mol2, unique_atms[0] )->getPosition();
		tem3 = getMatchingAtom( mol2, unique_atms[1] )->getPosition();
		
		// get transformation:
		trans_matrix = StructureMapper::matchPoints(frag1, frag2, frag3, tem1, tem2, tem3);
		
		return trans_matrix;
	}

	///3.) Just one unique atom was found, here we need to test some transformations
	///    to find the best one. Try all possibilities for the third atom:
	if (unique_atms.size() == 1) { 
		// if at least one mol1 atom was unique, it is curcial to use it:
		Atom* uniqueA = unique_atms[0];
		Atom* uniqueB = getMatchingAtom( mol2, uniqueA );
		
		// get next non unique atom from mol1:
		Atom* sel=0;
		AtmVec::iterator ato = mol1.begin();
		for(ato++; ato!=mol1.end(); ato++)
		{
			if(*ato != uniqueA){
				sel = *ato;
				break;
			}
		}
		
		// test all possible assignments of the selected third atom of mol1
		// to an atom of mol2
		float best_rmsd = numeric_limits<float>::max();
		
		AtomIterator ati = mol2->beginAtom();
		for(ati++; +ati; ati++)
		{
			float rmsd = numeric_limits<float>::max();
			if(&*ati != uniqueB)
			{
				Atom* selectionA1 = mol1[0];Atom* selectionB1 = mol2->getAtom(0);
				frag1 = selectionA1->getPosition(); tem1  = selectionB1->getPosition();
				frag2 = uniqueA->getPosition();
				tem2  = uniqueB->getPosition();
				frag3 = sel->getPosition();
				tem3  = ati->getPosition();
				
				trans_matrix = StructureMapper::matchPoints( tem1, tem2, tem3, frag1, frag2, frag3);
				transformer.setTransformation(trans_matrix);
				mol2->apply(transformer);
				
				rmsd = getMinRMSD(&mol1, mol2);
				if (rmsd < best_rmsd)
					best_rmsd = rmsd;
			}
		}
		return trans_matrix;
	}
	///4.) No unique atom was found. Here we have to check every possible
	///    assignment for the two remaining atoms...
	// if at least one mol1 atom was unique, it is curcial to use it:
	float best_rmsd = 100;
	Atom* selectionA1 = mol1[0];
	Atom* selectionA2 = mol1[1];
	Atom* selectionA3 = mol1[2];
	
	Atom* selectionB1 = mol2->getAtom(0);
	Atom* selectionB2 = 0;
	Atom* selectionB3 = 0;
	
	// test all possible assignments to 'selectionA2'
	AtomIterator ati = mol2->beginAtom();
	for(ati++; +ati; ati++)
	{
		selectionB2 = &*ati;
		if( atomsCompatible(selectionA2, selectionB2) )
		{
			// test all possible assignments to 'selectionA3'
			AtomIterator ato = mol2->beginAtom();
			for(ato++; +ato; ato++)
			{
				selectionB3 = &*ato;
				if(selectionB2 == selectionB3)
					continue;
				
				float rmsd = 1000;
				frag1 = selectionA1->getPosition(); tem1  = selectionB1->getPosition();
				frag2 = selectionA2->getPosition(); tem2  = selectionB2->getPosition();
				frag3 = selectionA3->getPosition(); tem3  = selectionB3->getPosition();
					
				trans_matrix = StructureMapper::matchPoints( tem1, tem2, tem3, frag1, frag2, frag3);
				transformer.setTransformation(trans_matrix);
				mol2->apply(transformer);
				
				rmsd = getMinRMSD(&mol1, mol2);
				if (rmsd < best_rmsd)
					best_rmsd = rmsd;
			}
		}
	}
	return trans_matrix;
}

/**
 * align
 */
Matrix4x4 AssemblerFunctions::align(AtmVec &site, AtomContainer* templ)
{
	Matrix4x4 result;
	Vector3 frag1, frag2, frag3, tem1, tem2, tem3;
	
	// set the two center atoms:
	frag1 = site[0]->getPosition();
	tem1  = templ->getAtom(0)->getPosition();
	
	/// simple solution for only one neighbor:
	if(site.size() == 2)
	{
		frag2 = site[1]->getPosition();
		tem2  = templ->getAtom(1)->getPosition();
		return StructureMapper::matchPoints(frag1, frag2, Vector3(), tem1, tem2, Vector3());
	}
	
	/// see if some atoms differ in element and order
	// find the unique elements, via their 'bondName'
	boost::unordered_map<String, int> el_map;
	for (int i = 1; i < site.size(); i++)
	{
		el_map[ getBondName(site[i], site[0]) ] += 1;
	}
	
	/// try to assign the unique atom if one exists:
	if (el_map.size() != 1)
	{
		AtmVec unique_atms;
		vector< int > unique_pos;
		vector< String > unique_names;
		boost::unordered_map<String, int>::iterator mit = el_map.begin();
		
		// find all unique identifiers:
		for (; mit != el_map.end(); mit++)
		{
			if(mit->second == 1)
				unique_names.push_back(mit->first);
		}
		
		/// a not yet captured case, but examples for this are probably very rare,
		/// because the connection to the partner is excluded, thus for all carbons
		/// we either have 3 identical bonds or at least one bond that is unique.
		if(unique_names.size() < 1)
		{
			cout<<"ERROR: currently we can't handle cases where different but"<<endl;
			cout<<" not unique bonds to a central atom exist"<<endl<<endl;
			exit(EXIT_FAILURE);
		}
		
		/// get all unique atoms (element+order) and positions 
		/// (for the matches in the template)
		for(int j = 0; j < unique_names.size(); j++)
		{
			for (int i = 1; i < site.size(); i++)
			{
				if ( getBondName(site[i], site[0]) == unique_names[j])
				{
					unique_atms.push_back(site[i]);
					unique_pos.push_back(i);
				}
			}
		}
		
		// Find mapping using the found unique atoms.
		// Take the first two, get matrix and return early:
		if(unique_atms.size() >= 2)
		{
			frag2 = unique_atms[0]->getPosition();
			frag3 = unique_atms[1]->getPosition();
			
			tem2 = templ->getAtom(unique_pos[0])->getPosition();
			tem3 = templ->getAtom(unique_pos[1])->getPosition();
			
			return StructureMapper::matchPoints(frag1, frag2, frag3, tem1, tem2, tem3);
		}
		// only one unique does exist, set it and continue with the last step
		else
		{
			frag2 = unique_atms[0]->getPosition();
			tem2 = templ->getAtom(unique_pos[0])->getPosition();
		}
	}
	
	/// all atoms have same element and order:
	/// this means we may choose one match arbitrarily and then test the 
	/// assignment for a second one iteratively
	/// OR: it might be that in the previous step we found already a single unique
	/// atom mapping
	if( frag2.isZero() ) // if frag2 was not yet initialized, arbitrarily choose
											 // the two second atoms to as matching pair
	{
		frag2 = site[1]->getPosition();
		tem2  = templ->getAtom(1)->getPosition();
	}
	
	frag3 = site[2]->getPosition();
	
	/// test all remaining assignments:
	// create test molecules:
	Molecule* dummy_frag;
	Molecule ref_frag;
	for(int i = 0; i < site.size(); i++)
		ref_frag.insert( *(new Atom (*site[i])) );

	dummy_frag = new Molecule(ref_frag, true);
	
	// assignment search loop (because a canonical ordering is not always possible for star-graphs)
	AtomIterator ati = templ->beginAtom();
	ati++; // we used first atom already
	for(; ati != templ->endAtom(); ati++ )
	{
		tem3 = ati->getPosition();
		
		if(tem2 == tem3) // it's possible that our unique assignment did set these coordinates already for tmp2
			continue;      // in that case cycle!
		
		result = StructureMapper::matchPoints(frag1, frag2, frag3, tem1, tem2, tem3);
		TransformationProcessor transformer(result);
		dummy_frag->apply(transformer);
		
		if ( 0.2 > getMinRMSD(dummy_frag, templ) )
		{
			delete dummy_frag;
			return result;
		}
		else
		{
			delete dummy_frag;
			dummy_frag = new Molecule(ref_frag, true);
		}
	}
	
	cout<<"ERROR: could not match"<<endl;
	exit(EXIT_FAILURE);
	return result;
}

/**
 * getMinRMSD (1)
 */
float AssemblerFunctions::getMinRMSD(AtomContainer* mol1, AtomContainer* mol2)
{
	
	// create a vector to allow easy swapping of atoms
	AtmVec atm_vec;
	for(AtomIterator ati = mol2->beginAtom(); +ati; ++ati)
		atm_vec.push_back( &*ati );
	
	// the 'sum of all square distances' for the best (minimal) permutation:
	float min_sq_dist = numeric_limits<float>::max(); 
	
	AtomIterator ati = mol1->beginAtom(); ++ati;  // start with second atom (first is central atom)
	sqdistForPermutations( mol1->getAtom(0), ati, atm_vec, 1, 0, &min_sq_dist);
	
	min_sq_dist = sqrt( min_sq_dist / (float)(atm_vec.size() - 1) );
	return min_sq_dist;
}

/**
 * getMinRMSD (2)
 */
float AssemblerFunctions::getMinRMSD(AtmVec* mol1, AtomContainer* mol2)
{
	// the 'sum of all square distances' for the best (minimal) permutation:
	float min_sq_dist = numeric_limits<float>::max(); 
	
	AtomIterator ati = mol2->beginAtom(); ++ati;  // start with second atom (first is central atom)
	sqdistForPermutations( mol2->getAtom(0), ati, *mol1, 1, 0, &min_sq_dist);
	
	min_sq_dist = sqrt( min_sq_dist / (float)(mol1->size() - 1) );
	return min_sq_dist;
}

/*
 * sqdistForPermutations
 */
void AssemblerFunctions::sqdistForPermutations(
		Atom* center1, AtomIterator& ati, AtmVec& atm_vec, int i, 
		float loc_sq_dist, float* global_sq_dist)
{
	// end recursion case: 
	// everything was permuted so check how good the square dist was and perhaps
	// update the global sq_dist
	if( i == atm_vec.size() )
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
			if(  atomsCompatible( &*ati, atm_vec[j] )  )
			{
				sq_dist_update = ati->getPosition().getSquareDistance( atm_vec[j]->getPosition() );
				
				swapAtoms(atm_vec[i], atm_vec[j]); // permute the vector entries
				
				AtomIterator ati2 = ati; // create new atom iterator for next recursion
				sqdistForPermutations( center1, ++ati2, atm_vec, (i+1), (loc_sq_dist + sq_dist_update), global_sq_dist);
				
				swapAtoms(atm_vec[i], atm_vec[j]); // undo the swap for next itertation
			}
		} // end for-loop
	}
}

/*
 * merge two connection templates to a final template
 * 
 * the final template will only contain 6 atoms, 3 for each end
 * starting at position 0 and 3 with the molecules that are to be
 * connected, and then the ordered next two neighbors
 */
void AssemblerFunctions::mergeTemplates(
		AtomContainer* mol1, int pos1, AtomContainer* mol2, int pos2,
										boost::unordered_map<String, float> std_bonds)
{
	Atom* aTarget = mol1->getAtom(pos1);
	Atom* bTarget = mol2->getAtom(pos2);
	Atom* atm1 = mol1->getAtom(0);
	Atom* atm2 = mol2->getAtom(0);

	// got no time to take care of the possible sign errors thus simply use 3-point-matching:
	Vector3& ptA1 = atm1->getPosition();
	Vector3& ptA2 = aTarget->getPosition();
	Vector3& ptB1 = bTarget->getPosition();
	Vector3& ptB2 = atm2->getPosition();

	Matrix4x4 rot_matrix = StructureMapper::matchPoints(ptB1, ptB2, Vector3(), ptA1, ptA2, Vector3());
	
	// transfrom the 2nd template
	TransformationProcessor transformer(rot_matrix);
	mol2->apply(transformer);
	
	// fix bond length
	Vector3 bond_fix = getDiffVec(atm1, atm2, std_bonds);
	TranslationProcessor t_later(bond_fix);
	mol2->apply(t_later);

	// remove the two connection partner atoms, they are redundant now:
	mol1->remove(*aTarget);
	mol2->remove(*bTarget);
}

/*
* getSite
*/
int AssemblerFunctions::getSite(Atom* atm, AtmVec &site, Atom* partner, String& key)
{
	// insert central atom first:
	site.push_back(atm);
	
	key = atm->getElement().getSymbol();
	
	// determine the key and the order for atoms
	vector< pair<String,Atom*> > elements;
	for(Atom::BondIterator b_it = atm->beginBond(); +b_it; b_it++)
	{
		Atom* tmp_atm = b_it->getBoundAtom(*atm); // get neighbors
		
		String elem = tmp_atm->getElement().getSymbol();
		elem += String(b_it->getOrder());
		
		elements.push_back( make_pair( elem, tmp_atm) );
	}
	
	// also add the partner atom for the new bond:
	elements.push_back( make_pair( String(partner->getElement().getSymbol()) + String(1), partner) );
	
	// sort identifers, but keep central atom as first:
	vector< pair<String, Atom*> >::iterator el_it = elements.begin(); ++el_it;
	sort( el_it, elements.end(), compare);
	
	// set the next two atoms that are NOT the partner
	// as additional points for a 3 point match site:
	int pos = -1;
	for(int i = 0; i < elements.size(); i++)
	{
		Atom* tmp = elements[i].second;
		
		if(tmp != partner)
			site.push_back(tmp);
		
		if(tmp == partner)
		{
			pos = i+1;
		}
	}
	
	// create the key:
	for(el_it = elements.begin(); el_it !=elements.end(); el_it++)
	{
		key += ((*el_it).first);
	}
	return pos;
}

/*
 * handleSimpleConnections
 */
void AssemblerFunctions::handleSimpleConnections( Atom* atm1, Atom* atm2,
															boost::unordered_map <String, float >& bondLib,
															boost::unordered_map <String, Fragment* >& connectLib)
{
	Fragment* frag1 = (Fragment*)atm1->getParent();
	Fragment* frag2 = (Fragment*)atm2->getParent();
	
	// create a new bond between the two atoms:
	Bond* bnd = new Bond();
	bnd->setOrder(1);
	atm1->createBond(*bnd, *atm2);
	
	AtmVec site; String site_key; int pos = -1; AtomContainer* templ;
	
	// both fragments have just one atom:
	// keep atom1 and move atom2 in just one direction about the std bond length
	if( frag1->countAtoms() + frag2->countAtoms() == 2)
	{
		String bond_key = atm1->getElement().getSymbol();
		bond_key += atm2->getElement().getSymbol();
		
		atm2->setPosition( atm1->getPosition() );
		atm2->getPosition().x += bondLib[bond_key];

		return; // return early
	}
	// frag 2 is a single atom
	else if(frag2->countAtoms() == 1)
	{
		pos = getSite(atm1, site, atm2, site_key);
		templ = new AtomContainer(*connectLib[site_key]);
		
		atm2->setPosition( templ->getAtom(pos)->getPosition() );
		templ->remove( *templ->getAtom(pos) );
		
		// rotate the single_frag so that it aligns with the template
		Matrix4x4 trans = align(site, templ);
		TransformationProcessor transformer(trans);
		frag1->apply(transformer);
	}
	// frag 1 is a single atom (should seldomly occur)
	else
	{
		pos = getSite(atm2, site, atm1, site_key);
		templ = new AtomContainer(*connectLib[site_key]);
		
		atm1->setPosition( templ->getAtom(pos)->getPosition() );
		templ->remove( *templ->getAtom(pos) );

		// rotate the single_frag so that it aligns with the template
		Matrix4x4 trans = align(site, templ);
		TransformationProcessor transformer(trans);
		frag2->apply(transformer);
	}
}

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
			if( atmi->getElement() == atmk->getElement() 
					&& atmi->beginBond()->getOrder() == atmk->beginBond()->getOrder())
//					&& &*atmi != &*atmk 
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
			if( (*atmi)->getElement() == (*atmk)->getElement() 
					&& (*atmi)->beginBond()->getOrder() == (*atmk)->beginBond()->getOrder() )
//					&& &*atmi != &*atmk 
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
 * getMatchingAtom
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
 * getBondName
 */
String AssemblerFunctions::getBondName(Atom* atm, Atom* partner)
{
	String name = atm->getElement().getSymbol();
	name += atm->getBond(*partner)->getOrder();
	return name;
}

