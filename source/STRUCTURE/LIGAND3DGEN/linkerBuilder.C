// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/LIGAND3DGEN/linkerBuilder.h>

#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/MATHS/analyticalGeometry.h>

using namespace BALL;
using namespace std;

/**
 * buildLinker
 */
LinkerBuilder::LinkerBuilder(SiteMap& connection_templates )
	: _connection_templates(connection_templates)
{
	
} 

LinkerBuilder::~LinkerBuilder()
{
	
}

void LinkerBuilder::buildLinker(AtomContainer& linker_frag, ConnectList& result_rotors)
{
	int atom_cnt = linker_frag.countAtoms();
	
	// -1.) catch trivial cases:
	if(atom_cnt < 3)
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
			float b_len = at1->getElement().getCovalentRadius() + at2->getElement().getCovalentRadius();
			
			at1->setPosition( Vector3() ); at2->setPosition( Vector3(0,0, b_len ) );
			
			return;
		}
	}
	
	//0.) reset list of connected atoms, and find rotors
	_done.clear();
	
	//1.) Start with an atom that is NOT at the end of the molecule
	// I think this restraint can be removed now, because in the beginning
	// we had no site templates for sites that consisted only of 2 atoms
	Atom* at1 = 0;
	for(AtomIterator ati = linker_frag.beginAtom(); +ati; ++ati)
	{
		if( LigBase::countBondsInParent(*ati, linker_frag) > 1)
		{
			at1 = &*ati;
			break;
		}
	}
	// set the starting atom to a position of (0, 0, 0)
	at1->setPosition( Vector3() ); 
	_done.insert( at1 );
	
	//1.) get the connectionsite template for 'at1'
	AtmVec site;
	String key;
	getSite( *at1, site, key);
	
	AtomContainer& tmp = loadTemplate( key );
	_aligner.setMolecules( site, tmp );
		
	//2.) 'align'/translate so that the templates central atom also lies at (0,0,0)
	//    (in this first case we only have 1 coordinate, no need for alignment)
	TranslationProcessor transl_porcessor( -(tmp.beginAtom()->getPosition()) );
	tmp.apply(transl_porcessor);
	
	// iterate over all internal bonds of the starting atom
	// (repeat recusive on the partner side until all bonds were handled)
	// WON'T WORK WITH CYCLES!
	for( Atom::BondIterator bit = at1->beginBond(); +bit; ++bit)
	{
		Atom& partner = * bit->getBoundAtom(*at1); 
		
		if( linker_frag.isParentOf( partner ) ) // restrict to intra-fragment-bonds!
		{
			_done.insert( &partner );
			getPositionFromTemplate(site, tmp, partner);
			site.push_back( &partner );
			
			recurLinkerConnect( partner, linker_frag);
		}
	}
	
	// resolve all linker clashes:
	if( atom_cnt > 3)
	{
		return resolveLinkerClashes( linker_frag, result_rotors );
	}
	else
	{
		result_rotors.push_back( make_pair(at1, at1->getPartnerAtom(0) ) );
	}
}


AtomContainer& LinkerBuilder::loadTemplate( String& key )
{
	if( _connection_templates.find( key ) != _connection_templates.end() )
	{
		return * _connection_templates[ key ];
	}
	else
	{
		throw BALL::Exception::SiteTemplateNotFound("linkerBuilder.C", 125, key);
	}
}

/*
 * P R I V A T E
 * recurLinkerConnect
 */
void LinkerBuilder::recurLinkerConnect(Atom& atm, const Composite& parent)
{
	if( LigBase::countBondsInParent( atm, parent) < 2 )
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
	
	// 3.) iterate all bonds connected to 'atm'
	for( Atom::BondIterator bit = atm.beginBond(); +bit; ++bit)
	{
		Atom& partner = * bit->getBoundAtom( atm );
		
		if( ! _done.has( &partner ) )
		{
			if ( parent.isParentOf( partner ) )
			{
				_done.insert( &partner );
				
				//4.) get a position for the partner from the template
				getPositionFromTemplate(site, tmp, partner);
				site.push_back( &partner );
				
				//5.) descend recursive with partner as next atom
				recurLinkerConnect(partner, parent);
			}
		}
	}// end loop
}

/*
 * P R I V A T E
 * connectAtomToSite
 * 
 * 'site' connection site
 * 'partner' is the atom to be set to a new coordinate
 */ 
void LinkerBuilder::getPositionFromTemplate(AtmVec& site, AtomContainer& temp, Atom& partner)
{
	//1.) determine remaining atoms.
	AtmVec unassigned_atoms;
	_aligner.setMolecules( site, temp );
	_aligner.getRemainder( unassigned_atoms );
	
	//2.) find an atom matching to 'partner' in the remainder:
	Atom* atm_tmp;
	Element elem = partner.getElement();
	short bo = partner.getBond( *site[0])->getOrder();
	
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
	partner.setPosition( atm_tmp->getPosition() );
}

/*
 * P R I V A T E
 * getSite
 */
void LinkerBuilder::getSite(Atom& atm, AtmVec &site, String& key)
{
	// insert central atom for the site and the key
	site.push_back( &atm );
	key = atm.getElement().getSymbol();
	
	Composite& parent = * atm.getParent();
	
	// structure to sort the neighbors according to their names (element+BO)
	vector< pair<String,Atom*> > names_neighbors;
	
	// add all neighbors to 'elements' (central atom is not contained)
	for(Atom::BondIterator b_it = atm.beginBond(); +b_it; b_it++)
	{
		Atom* tmp_atm = b_it->getBoundAtom( atm ); // get neighbors of central 'atm'
		
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
				parent.isParentOf( *name_it->second ) &&
				_done.has( name_it->second )
			) 
			site.push_back( name_it->second );
	}
}

/*
 * P R I V A T E
 * compare
 */
bool LinkerBuilder::compare(pair<String,Atom*>& a, pair<String,Atom*>& b)
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
void LinkerBuilder::resolveLinkerClashes(AtomContainer &linker_frag, ConnectList& result_rotors)
{
	_rotors.clear();
	
	//1.) find an atom that is a 'hub' to start the recursion from
	Atom* at1 = 0;
	int current_cnt = 0;
	for(AtomIterator ati = linker_frag.beginAtom(); +ati; ++ati)
	{
		current_cnt = LigBase::countBondsInParent(*ati, linker_frag);
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
		recurResolveLinker( current_cnt, *bit, *next, 0, &linker_frag);
	}
	
	//4.) check for clashes and resolve
	_cresolv.setMolecule(linker_frag, _rotors );
	if(_cresolv.detect() != 0)
	{
		_cresolv.resolve();
	}
	
	// append the found rotors to the 'result_rotors'
	result_rotors.splice( result_rotors.end(), _rotors );
}

/*
 * P R I V A T E
 * recurFindRotors
 */
void LinkerBuilder::recurResolveLinker(int previous_cnt, Bond& bnd, 
																			Atom& curr_atm, int dist, Composite* parent)
{
	int current_cnt = LigBase::countBondsInParent( curr_atm, *parent );
	
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
		// not terminal (cnt>1): so set the bond to 'trans'
		setBondTrans( bnd );
		
		// if we also had a 'hub' (prev_cnt >2) add the bond to the rotors
		if( previous_cnt > 2)
		{
			dist = 1;
			_rotors.push_back( make_pair( bnd.getFirstAtom(), bnd.getSecondAtom() ) );
		}
		// current atom is a 'hub' 
		// guarantees that we get on chains of lenght > 2 between two hubs
		// two rotors before each hub.
		else if( current_cnt > 2)
		{
			if( dist > 1)// prev. atom was no hub!
				_rotors.push_back(make_pair( bnd.getFirstAtom(), bnd.getSecondAtom() ));
			
			dist = 0;
		}
		//simple chain atom, increase dist to 'hub'
		else if( current_cnt == 2 )
		{
			dist++;
		}

		// independent of hub or linker status: continue recursion with the remaining bonds
		for( AtomBondIterator bit = curr_atm.beginBond(); +bit; ++bit)
		{
			if( &*bit != &bnd)
			{
				if( bit->getBoundAtom(curr_atm)->getParent() == parent)
				{
					recurResolveLinker(current_cnt, *bit, *bit->getBoundAtom(curr_atm), dist, parent);
				}
			}
		}
	}
}

/*
 * P R I V A T E
 * setBondTrans
 */
void LinkerBuilder::setBondTrans(Bond &bnd)
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
	
	_roto.setAxis(*at1, *at2);
	_roto.rotate( Angle(Constants::PI)-is_angle );

}

/*
 * P R I V A T E
 * isTerminalBond
 */
bool LinkerBuilder::isTerminalBond(Bond &bnd, Composite &parent)
{
	Atom* at1 = bnd.getFirstAtom();
	Atom* at2 = bnd.getSecondAtom();
			
	if( LigBase::countBondsInParent(*at1, parent) == 1 )
		return true;
	else if (LigBase::countBondsInParent(*at2, parent) == 1 )
		return true;
	else
		return false;
}
