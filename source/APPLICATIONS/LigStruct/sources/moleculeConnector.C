// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include "moleculeConnector.h"

#include <BALL/KERNEL/PTE.h>
#include <BALL/FORMAT/SDFile.h>//#DEBUG

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

void MoleculeConnector::setConnectionLib(SiteMap &connectLib)
{
	_connections = &connectLib;
}

void MoleculeConnector::setBondLengthLib(BondLengthMap &bondLib)
{
	_bond_lengths = &bondLib;
}

void MoleculeConnector::setLibs(SiteMap &connectLib, BondLengthMap &bondLib)
{
	_connections = &connectLib;
	_bond_lengths = &bondLib;
}

/**  
 * connect molecules
 */
void MoleculeConnector::connect(Atom* atm1, Atom* atm2)
{
	SDFile outfile("./2OUT.sdf", std::ios::out);//#DEBUG
	outfile << *(Molecule*) &atm1->getRoot();//#DEBUG
	//1) get connection sites of the two atoms and the corresponding templates
	AtmVec site_frag1, site_frag2;
	String key1, key2;
	
	getSite(atm1, site_frag1, key1);
	getSite(atm2, site_frag2, key2);
	
	AtomContainer* templ1;
	AtomContainer* templ2;
	
	// load templates & only make a copy of a template if both sites are identical
	bool identical_templates = false;
	if( key1 != key2)
	{
		loadTemplates(templ1, key1);
		loadTemplates(templ2, key2);
	}
	else
	{
		loadTemplates(templ1, key1);
		templ2 = new AtomContainer( *templ1 );
		identical_templates = true;
	}

	//2) transfrom templ1 to match with frag1 (keep frag1 as it was)
	_star_aligner.setMolecules(site_frag1, *templ1);
	_star_aligner.align();

	AtmVec remain_tmp1;
	_star_aligner.getRemainder(remain_tmp1);

	//3) transfrom templ2 to match with frag2
	AtomContainer* frag2 = (AtomContainer*) &atm2->getRoot();

	_star_aligner.setMolecules( site_frag2, *templ2);
	_star_aligner.align();
	
	AtmVec remain_tmp2;
	_star_aligner.getRemainder(remain_tmp2);
	
	//4) project the connection bond determined for temp2 on to the one determined
	//   for temp1.
	Element elem2 = atm2->getElement();
	short bo2    = atm2->getBond(*atm1)->getOrder();
	
	Element elem1 = atm1->getElement();
	short bo1    = atm1->getBond(*atm2)->getOrder();

	Atom* atm1_partner = getMatchingAtomAll( &*templ1->beginAtom(), remain_tmp1, elem2, bo2);
	Atom* atm2_partner = getMatchingAtomAll( &*templ2->beginAtom(), remain_tmp2, elem1, bo1);

	cout<<"looking for: "<<elem1<<" "<< bo1<<endl;
	cout<<"I am the match: "<<atm2_partner->getElement().getSymbol()<<endl;
	
	outfile << *(Molecule*) templ1;//#DEBUG
	outfile << *(Molecule*) templ2;//#DEBUG
	Matrix4x4 bmatr = StarAligner::bondAlign(atm1, atm1_partner, atm2_partner, atm2);
	TransformationProcessor transformer;
	transformer.setTransformation( bmatr );
	
	outfile << *(Molecule*)frag2; //#DEBUG
	frag2->apply( transformer );
	outfile << *(Molecule*)frag2; //#DEBUG
	
	//5) set bond length to standard length
	Vector3 bond_fix = getDiffVec(atm1, atm2);
	TranslationProcessor t_later(bond_fix);
	
	frag2->apply(t_later);

	if( identical_templates )
		delete templ2;
	
	outfile << *(Molecule*) &atm1->getRoot();//#DEBUG
	outfile.close(); //#DEBUG
}

void MoleculeConnector::loadTemplates(AtomContainer*& tmp, String& key)
{
	if(_connections->find(key) != _connections->end() )
	{
		tmp = _connections->at(key);
	}
	else
	{//#TODO:####: implement as exception
		cout<<"ERROR in moleculeConnector: could not find connectionTemplate for: "
			  << key<<endl;
		exit(EXIT_FAILURE);
	}
}


/*
 * getSite
 */
void MoleculeConnector::getSite(Atom* atm, AtmVec &site, String& key)
{
	// insert central atom for the site and the key
	site.push_back(atm);
	key = atm->getElement().getSymbol();
	
	Composite* root = & atm->getRoot();
	
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
		
		if( root->isAncestorOf( *name_it->second ) ) 
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
Atom* MoleculeConnector::getMatchingAtomAll(Atom *center, AtmVec& mol, Element& elem, short bo)
{
//	cout<<"in getMatchingAll"<<endl;

	AVIter ati = mol.begin();
	for(; ati != mol.end(); ++ati)
	{
		if( (*ati)->getElement() == elem && (*ati)->getBond( *center )->getOrder() == bo )
			return *ati;
	}
	
	cout<<"ERROR: could not find a partner for: "<<elem.getSymbol()<<bo<<endl<<endl;
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
