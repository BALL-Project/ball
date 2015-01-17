// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include "moleculeConnector.h"
#include "starAligner.h"
#include "base.h"

#include <BALL/KERNEL/atomContainer.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/DATATYPE/string.h>
#include <BALL/KERNEL/fragment.h>
#include <boost/unordered_map.hpp>
using namespace BALL;
using namespace std;


MoleculeConnector::MoleculeConnector()
{
	_connections = 0;
	_bond_lengths = 0;
}

MoleculeConnector::~MoleculeConnector()
{
	
}

void MoleculeConnector::setConnectionLib(ConSiteMap &connectLib)
{
	_connections = &connectLib;
}

void MoleculeConnector::setBondLengthLib(BondLengthMap &bondLib)
{
	_bond_lengths = &bondLib;
}

void MoleculeConnector::setLibs(ConSiteMap &connectLib, BondLengthMap &bondLib)
{
	_connections = &connectLib;
	_bond_lengths = &bondLib;
}

/**  
 * connect molecules
 */
void MoleculeConnector::connect(Atom* atm1, Atom* atm2)
{
	StarAligner star_aligner;
	
	///1) get connection sites of the two atoms and the corresponding templates
	cout<<"####Step1"<<endl;
	AtmVec site_frag1, site_frag2;
	String key1, key2;
	
	getSite(atm1, site_frag1, key1);
	cout<<"####Step1 - Site1: "<<site_frag1.size()<<endl;
	
	getSite(atm2, site_frag2, key2);
	cout<<"####Step1 - Site2: "<<site_frag2.size()<<endl;
	
	cout<<"####Step1 - C-Lib lookup 1: "<<key1<<endl;
	AtomContainer* templ1 = new AtomContainer( * _connections->at(key1) );
	cout<<"####Step1 - C-Lib lookup 2: "<<key2<<endl;
	AtomContainer* templ2 = new AtomContainer( * _connections->at(key2) );
	
//	// DEBUG
//	cout<< "Site1: "<< printInlineMol(site_frag1)<<endl;
//	cout<< "Site2: "<< printInlineMol(site_frag2)<<endl;
//	cout<< "key1: "<< key1<<endl;
//	cout<< "key2: "<< key2<<endl;
//	cout<< "tmp1: "<< printInlineMol(templ1)<<endl;
//	cout<< "tmp2: "<< printInlineMol(templ2)<<endl;
//	// DEBUG - end

	cout<<"####Step1 - done"<<endl;
	
	///2) transfrom templ1 to match with frag1 (keep frag1 as it was)
	cout<<"####Step2"<<endl;
	
	cout<<"Got Site:"<<LigBase::printInlineStarMol(site_frag1)<<endl;
	cout<<"Got Connection:"<<LigBase::printInlineStarMol(templ1)<<endl;
	star_aligner.setMolecules(site_frag1, *templ1);
	cout<<"####Step2: did set molecules"<<endl;
	star_aligner.align();
	cout<<"####Step2: did align"<<endl;
	
	AtmVec remain_tmp1;
	star_aligner.getRemainder(remain_tmp1);
	cout<<"####Step2: got remainder"<<endl;
	
	///3) transfrom templ2 to match with frag2
	cout<<"####Step3"<<endl;
	
	// get all atoms that are in the same container as atm2:
	AtomContainer* frag2 = (AtomContainer*) &atm2->getRoot();
//	cout<<"    got partent"<<endl;
	
//	cout<<
	star_aligner.setMolecules( site_frag2, *templ2);
	star_aligner.align();
//	cout<<"    aligned"<<endl;
	
	AtmVec remain_tmp2;
	star_aligner.getRemainder(remain_tmp2);
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
	
	cout<<"found partner"<<endl;
	star_aligner.setMolecules(*frag2, *frag2);
	star_aligner.bondAlign(atm1, atm1_partner, atm2_partner, atm2);
	cout<<"aligned"<<endl;
	cout<<"transformed"<<endl;
	
	///5) set bond length to standard length
	cout<<"####Step5"<<endl;
	Vector3 bond_fix = getDiffVec(atm1, atm2);
//	cout<<"bond fix: "<<bond_fix<<endl;
	TranslationProcessor t_later(bond_fix);
	
	frag2->apply(t_later);
	delete templ1;
	delete templ2;
}


/*
 * getSite
 */
void MoleculeConnector::getSite(Atom* atm, AtmVec &site, String& key)
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
 * getDiffVec
 */
Vector3 MoleculeConnector::getDiffVec(Atom* atm1, Atom* atm2)
{
	String key = atm1->getElement().getSymbol();
	key += atm2->getElement().getSymbol();
	float bond_len = _bond_lengths->at(key);
	
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
 * getMatchingAtomAll
 */
Atom* MoleculeConnector::getMatchingAtomAll(Atom *center, AtmVec& mol, String& elem, short bo)
{
//	cout<<"in getMatchingAll"<<endl;

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
 * compare
 */
bool MoleculeConnector::compare(pair<String,Atom*>& a, pair<String,Atom*>& b)
{
	return a.first < b.first;
}
