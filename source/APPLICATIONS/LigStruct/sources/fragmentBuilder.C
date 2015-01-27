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
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/fragment.h>

#include <BALL/MATHS/matrix44.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
/**
 * buildLinker
 */
FragmentBuilder::FragmentBuilder(ConSiteMap& connection_templates , BondLengthMap &bonds)
	: _connection_templates(connection_templates),
		_bond_lengths(bonds)
{
	
}

FragmentBuilder::~FragmentBuilder()
{
	
}

void FragmentBuilder::buildLinker(Fragment& linker_frag)
{
	int atom_cnt = linker_frag.countAtoms();
	
	// -1.) catch trivial cases:
	if(atom_cnt<3)
	{
		if(atom_cnt == 0)
		{
			return;
		}
		else if(atom_cnt == 1)
		{
			linker_frag.beginAtom()->setPosition( Vector3() );
			return;
		}
		else if (atom_cnt == 2)
		{
			Atom* at1 = linker_frag.getAtom(0); Atom* at2 = linker_frag.getAtom(1);
			
			String key = at1->getElement().getSymbol() + at2->getElement().getSymbol();
			float b_len = _bond_lengths[key];
			
			at1->setPosition( Vector3() ); at2->setPosition( Vector3(0,0, b_len ) );
			
			return;
		}
	}
	
	//0.) reset list of connected atoms, and find rotors
	_done.clear();
	
	//1.) Start with an atom that is NOT at the end of the molecule
	Atom* at1 = 0;
	for(AtomIterator ati = linker_frag.beginAtom(); +ati; ++ati)
	{
		if( LigBase::countBondsInPartent(*ati, linker_frag) > 1)
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
	
	// resolve all linker clashes:
	if( atom_cnt > 3)
	{
		findRotors( linker_frag );
	}
}

/*
 * P R I V A T E
 * recurLinkerConnect
 */
void FragmentBuilder::recurLinkerConnect(Atom* atm, const Composite * parent)
{
	if( LigBase::countBondsInPartent(*atm, *parent) < 2 )
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

/*
 * P R I V A T E
 * findRotors
 * A 'hub' is an atom that connects with more than two other atoms and thus may 
 * connect 3 chains.
 * 
 * A 'chain' consist solely of atoms with one or two partners. It is terminated 
 * by 'hubs' or 'terminal atoms'.
 * 
 * A 'link' is an atom from a chain being connected to exactly two other atoms
 * 
 * A 'terminal atom' is connected to a single atom and is only part of a 'chain'
 * if it is connected to a 'link'. Is it connected to a hub it will be ignored
 */
void FragmentBuilder::findRotors(Fragment &linker_frag)
{
	_rotors.clear();
	
	//1.) find an atom that is a 'hub' to start the recursion from
	Atom* at1 = 0;
	int current_cnt = 0;
	for(AtomIterator ati = linker_frag.beginAtom(); +ati; ++ati)
	{
		current_cnt = LigBase::countBondsInPartent(*ati, linker_frag);
		if( current_cnt > 2)
		{
			at1 = &*ati;
			break;
		}
	}
	
	//2.) if there was no hub found (we have only a single chain) solve trivially
	//    (ignore the '_rotor' list, we simply set all to trans and are safe
	if( at1 == 0)
	{
		AtomIterator ati;
		AtomBondIterator bit;
		
		BALL_FOREACH_INTRABOND(linker_frag, ati, bit)
		{
			if( ! isTerminalBond(*bit, linker_frag) )
				setBondTrans( *bit);
		}

		return;
	}

	//3.) for all connected atoms/bonds start the recursion
	Atom* next = 0;
	for(AtomBondIterator bit = at1->beginBond(); +bit; ++bit)
	{
		next = bit->getBoundAtom( *at1 );
		recurFindRotors( current_cnt, *bit, *next, &linker_frag);
	}
}

/*
 * P R I V A T E
 * recurFindRotors
 */
void FragmentBuilder::recurFindRotors(int previous_cnt, Bond& bnd, 
																			Atom& curr_atm, Composite* parent)
{
	int current_cnt = LigBase::countBondsInPartent( curr_atm, *parent );
	
	// A terminal atom (thus also a terminal bond) was found: 
	// TERMINATE recursion
	if( current_cnt == 1)
	{
		return;
	}
	// A linker or hub atom was found: recurse
	else
	{
		// bond is non terminal, because we alwas start from hubs and this atom is
		// not terminal: so set the bond to 'trans'
		setBondTrans( bnd );
		
		// if we also have a 'hub' (prev_cnt >2) add the bond to the rotors
		if( previous_cnt > 2)
		{
			_rotors.push_back( &bnd );
		}

		// independent of hub or linker status: continue recursion with the remaining bonds
		for( AtomBondIterator bit = curr_atm.beginBond(); +bit; ++bit)
		{
			if( &*bit != &bnd)
			{
				if( bit->getBoundAtom(curr_atm)->getParent() == parent)
				{
					recurFindRotors(current_cnt, *bit, *bit->getBoundAtom(curr_atm), parent);
				}
			}
		}
	}
}

void FragmentBuilder::setBondTrans(Bond &bnd)
{
	Atom* at1 = bnd.getFirstAtom();
	Atom* at2 = bnd.getSecondAtom();
	
	// find "the other bond" that is not bnd:
	Atom* at1_p = 0;
	for(AtomBondIterator bit = at1->beginBond(); +bit; ++bit)
	{
		if( &*bit != &bnd){
			at1_p = bit->getBoundAtom( *at1 );
			break;
		}
	}
	Atom* at2_p = 0;
	for(AtomBondIterator bit = at2->beginBond(); +bit; ++bit)
	{
		if( &*bit != &bnd){
			at2_p = bit->getBoundAtom( *at2 );
			break;
		}
	}
	
	Vector3& p1 = at1_p->getPosition();
	Vector3& p2 = at1->getPosition();
	Vector3& p3 = at2->getPosition();
	Vector3& p4 = at2_p->getPosition();
	
	Angle is_angle = getTorsionAngle(p1.x, p1.y, p1.z, 
																	 p2.x, p2.y, p2.z, 
																	 p3.x, p3.y, p3.z, 
																	 p4.x, p4.y, p4.z );
	
	_cresolv.rotate(*at1, *at2, Angle(Constants::PI)-is_angle);

}

bool FragmentBuilder::isTerminalBond(Bond &bnd, Composite &parent)
{
	Atom* at1 = bnd.getFirstAtom();
	Atom* at2 = bnd.getSecondAtom();
			
	if( LigBase::countBondsInPartent(*at1, parent) == 1 )
		return true;
	else if (LigBase::countBondsInPartent(*at2, parent) == 1 )
		return true;
	else
		return false;
}
