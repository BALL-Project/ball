// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "fragmentBuilder.h"
#include "base.h"

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/KERNEL/fragment.h>

#include <BALL/MATHS/matrix44.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
/**
 * buildLinker
 */
FragmentBuilder::FragmentBuilder( ConSiteMap& connection_templates )
	:_connection_templates(connection_templates)
{
	
}

FragmentBuilder::~FragmentBuilder()
{
	
}

void FragmentBuilder::buildLinker(Fragment& linker_frag)
{
	//0.) reset list of connected atoms:
	_done.clear();
	
	//1.) Start with an atom that is NOT at the end of the molecule
	Atom* at1 = 0;
	for(AtomIterator ati = linker_frag.beginAtom(); +ati; ++ati)
	{
		if( ati->countBonds() > 1)
		{
			at1 = &*ati;
			break;
		}
	}
	at1->setPosition( Vector3() ); // set first to (0, 0, 0)
	_done.insert( at1 );
	
	//1.) get the template for 'at1'
	AtmVec site;
	String key;
	getSite(at1, site, key);
	
	AtomContainer tmp( * _connection_templates[key] );
	_aligner.setMolecules( site, tmp );
		
	//2.) 'align' so that the templates central atom also lies at (0,0,0)
	TranslationProcessor trpl( -(tmp.beginAtom()->getPosition()) );
	tmp.apply(trpl);
	
	// iterate (recursive) over all internal (intra) bonds of the fragment, until
	// all bonds are selected (this algo won't work with cycles!!!)
	for( Atom::BondIterator bit = at1->beginBond(); +bit; ++bit)
	{
		Atom* partner = bit->getBoundAtom(*at1); 
		
		if( partner->getParent() == &linker_frag ) // restrict to intra-fragment-bonds!
		{
			_done.insert( partner );
			getPositionFromTemplate(site, tmp, partner);
			site.push_back( partner );
			
			recurLinkerConnect( partner, &linker_frag);
		}
	}
}

/*
 * P R I V A T E
 * recurLinkerConnect
 */
void FragmentBuilder::recurLinkerConnect(Atom* atm, const Composite * parent)
{
	if( atm->countBonds() < 2 )
	{
		return;
	}
	
	//1.) get the 'site' from 'atm'
	AtmVec site;
	String key;
	getSite(atm, site, key);
	
	//2.) get the template and align it to the 'site':
	AtomContainer tmp( * _connection_templates[key] );
	_aligner.setMolecules( site, tmp );
	_aligner.align();
	// iterate all bonds connected to 'atm'
	for( Atom::BondIterator bit = atm->beginBond(); +bit; ++bit)
	{
		//3.) get partner atom and...
		Atom* partner = bit->getBoundAtom(*atm);
		
		// ...proceed if this atom was not yet handled
		if( ! _done.has( partner ) )
		{
			// restrict to atoms of the same fragment
			if (partner->getParent() == parent)
			{
				_done.insert( partner );
				
				//4.) get a position for the partner from the template
				getPositionFromTemplate(site, tmp, partner);
				site.push_back( partner );
				
				//5.) descend recursive with partner as next atom
				recurLinkerConnect(partner, parent);
			}
		}
	}// end loop
}

/*
 * P R I V A T E
 * connectAtomToSite
 */ 
void FragmentBuilder::getPositionFromTemplate(AtmVec& site, AtomContainer& temp, Atom* partner)
{
	//1.) determine remaining atoms.
	AtmVec unassigned_atoms;
	_aligner.setMolecules( site, temp );
	_aligner.getRemainder( unassigned_atoms );
	
	//2.) find an atom matching to 'partner' in the remainder:
	Atom* atm_tmp;
	Element elem = partner->getElement();
	short bo = partner->getBond( *site[0])->getOrder();
	AVIter ati = unassigned_atoms.begin();
	for(; ati != unassigned_atoms.end(); ++ati)
	{
		if( 
			 (*ati)->getElement() == elem && 
			 (*ati)->getBond( *temp.beginAtom() )->getOrder() == bo 
			)
		{
			atm_tmp = *ati;
			break;
		}
	}
	
	//3.) transfer position from the found remainder atom of the template to
	//    the partner atom position:
	partner->setPosition( atm_tmp->getPosition() );
}


/*
 * P R I V A T E
 * getSite
 */
void FragmentBuilder::getSite(Atom* atm, AtmVec &site, String& key)
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
		key += name_it->first;
		
		if( 
				(*name_it).second->getParent() == parent &&
				_done.has( name_it->second )
			) 
			site.push_back( name_it->second );
	}
}

/*
 * P R I V A T E
 * compare
 */
bool FragmentBuilder::compare(pair<String,Atom*>& a, pair<String,Atom*>& b)
{
	return a.first < b.first;
}
