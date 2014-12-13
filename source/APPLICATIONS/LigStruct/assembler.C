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
	AtomContainer* frag1 = (AtomContainer*)atm1->getParent();
	AtomContainer* frag2 = (AtomContainer*)atm2->getParent();
	
//	cout<<"Connecting Atoms "<<con1<< " and "<< con2<<endl;
//	cout<<"Connecting Fragments "<<frag1<< " and "<< frag2<<endl;
	
	///0) Check for trivial cases of one being a single atom or both being single atoms
	if(frag1->countAtoms() == 1 || frag2->countAtoms() == 1)
		handleSimpleConnections(atm1, atm2, bondLib, connectLib);
	
//	cout<<"searching sites..."<<endl<<endl;
	///1) find connection sites of the two atom pointers
	///   (pos1/2 tells us where in a connection template the connected atom lies)
	AtmVec site_frag1, site_frag2;
	String key1, key2;
	getSite(atm1, atm2, site_frag1, key1);
	getSite(atm2, atm1, site_frag2, key2);
	
//	cout<<"site key1: "<<key1<<endl;
//	cout<<"site key2: "<<key2<<endl<<endl;
//	cout<<"......done!"<<endl<<endl;
	
//	cout<<"searching templates..."<<endl;
	///2) find the corresponding templates
	// create working_copies to keep the originals save!
	AtomContainer* templ1 = connectLib[key1];
	AtomContainer* templ2 = connectLib[key2];
	
//	cout<< "found templates: "<< connectLib[key1] <<" "<<templ1->countAtoms() <<" - "<<connectLib[key2]<<" "<<templ2->countAtoms()<<endl;
//	cout<<"......done!"<<endl<<endl;
	
//	cout<<"creating connected template..."<<endl;
	Matrix4x4 trans_matr;
	TransformationProcessor transformer;
	
	///3) transfrom templ1 to match with frag1 (keep frag1 as it was)
	Atom* tmp1_atm2 = starAlign( site_frag1, templ1, trans_matr );
	transformer.setTransformation( trans_matr );
	templ1->apply( transformer );

	///4) transfrom frag2 to match with templ2
	Atom* tmp2_atm1 = starAlign(  site_frag2, templ2, trans_matr );
	transformer.setTransformation( trans_matr );
	frag2->apply( transformer );
	
	///5) transfrom the connection bond determined for temp2 to the one determined
	///   for temp1.
	bondAlign(atm1, tmp1_atm2, tmp2_atm1, atm2, bondLib, trans_matr);
	transformer.setTransformation( trans_matr );
	templ2->apply( transformer );
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
void AssemblerFunctions::starAlign(AtmVec& mol1, AtomContainer* mol2, Matrix4x4& trans_matrix)
{
	Vector3 frag2, frag3, tem2, tem3; //the vectors to calc the transformaion
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
		
		return;
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
		
		return;
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
		return;
	}
	///4.) No unique atom was found. Here we have to check every possible
	///    assignment for the two remaining atoms...
	// if at least one mol1 atom was unique, it is curcial to use it:
	float best_rmsd = numeric_limits<float>::max();
	Atom* selectionA1 = mol1[0];
	Atom* selectionA2 = mol1[1];
	Atom* selectionA3 = mol1[2];
	
	Atom* selectionB1 = mol2->getAtom(0);
	Atom* selectionB2 = 0;
	Atom* selectionB3 = 0;
	
	// test all possible assignments to 'selectionA2'
	AtomIterator ati = mol2->beginAtom();
	Matrix4x4 best_trans;
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
				
				float rmsd = numeric_limits<float>::max();
				frag1 = selectionA1->getPosition(); tem1  = selectionB1->getPosition();
				frag2 = selectionA2->getPosition(); tem2  = selectionB2->getPosition();
				frag3 = selectionA3->getPosition(); tem3  = selectionB3->getPosition();
					
				trans_matrix = StructureMapper::matchPoints( tem1, tem2, tem3, frag1, frag2, frag3);
				transformer.setTransformation(trans_matrix);
				mol2->apply(transformer);
				
				rmsd = getMinRMSD(&mol1, mol2);
				if (rmsd < best_rmsd)
				{
					best_rmsd = rmsd;
					best_trans = trans_matrix;
				}
			}
		}
	}
	trans_matrix = best_trans;
	return;
}

/**
 * starAlign (for AtomContainer-Site)
 */
void AssemblerFunctions::starAlign(AtomContainer* mol1, AtmVec& mol2, Matrix4x4& trans_matrix)
{
	// generate vector from AtomContainer
	AtmVec vec;
	vec.reserve(8);
	
	for(AtomIterator ati = mol1->beginAtom(); +ati; ++ati)
		vec.push_back( &*ati );
	
	// generate an atomContainer from vector
	AtomContainer atc;
	for(int i = 0; i < mol2.size(); i++)
		atc.insert( *(new Atom( *mol2[i] )) );
	
	starAlign(vec, &atc, trans_matrix);
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
void AssemblerFunctions::getSite(Atom* atm, Atom* partner, AtmVec &site, String& key)
{
	// insert central atom for the site and the key
	site.push_back(atm);
	key = atm->getElement().getSymbol();
	
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
	
	// create the key, and add sorted neighbors to the site, but do not add the
	// partner atom, because its coordinates are not connected to the rest of the
	// site
	for(name_it = names_neighbors.begin(); name_it !=names_neighbors.end(); name_it++)
	{
		key += ( (*name_it).first );
		
		if( (*name_it).second == partner)
			continue;
		
		site.push_back( (*name_it).second );
	}
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
	
	Matrix4x4 trans;
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
		pos = getSiteWithPos(atm1, site, atm2, site_key);
		templ = new AtomContainer(*connectLib[site_key]);
		
		atm2->setPosition( templ->getAtom(pos)->getPosition() );
		templ->remove( *templ->getAtom(pos) );
		
		// rotate the single_frag so that it aligns with the template
		starAlign(site, templ, trans);
		TransformationProcessor transformer(trans);
		frag1->apply(transformer);
	}
	// frag 1 is a single atom (should seldomly occur)
	else
	{
		pos = getSiteWithPos(atm2, site, atm1, site_key);
		templ = new AtomContainer(*connectLib[site_key]);
		
		atm1->setPosition( templ->getAtom(pos)->getPosition() );
		templ->remove( *templ->getAtom(pos) );

		// rotate the single_frag so that it aligns with the template
		starAlign(site, templ, trans);
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

