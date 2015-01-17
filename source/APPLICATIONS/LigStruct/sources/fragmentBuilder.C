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
	/// Init the very first atom:
	Atom* at1 = &*linker_frag.beginAtom();
	at1->setPosition( Vector3() ); // init to (0, 0, 0)

	// iterate (recursive) over all internal (intra) bonds of the fragment, until
	// all bonds are selected (this algo won't work with cycles!!!)
	for( Atom::BondIterator bit = at1->beginBond(); +bit; ++bit)
	{
		Atom* partner = bit->getBoundAtom(*at1); 
		
		if( partner->getParent() == &linker_frag ) // restrict to intra-fragment-bonds!
			recurLinkerConnect( partner, &linker_frag);
	}
}

/*
 * P R I V A T E
 * recurLinkerConnect
 */
void FragmentBuilder::recurLinkerConnect(Atom* at, Composite * const parent)
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
//				getSelectedSite(at, site, key);
				
				//2.) connect single Atom 'partner' to site
				connectAtomToSite(site, *(_connection_templates[key]), partner);
				
				//3.) descend recursion with partner
				recurLinkerConnect(partner, parent);
			}
		}
		
	}// end loop
}


/*
 * P R I V A T E
 * connectSingle
 */ 
void FragmentBuilder::connectAtomToSite(AtmVec& site, AtomContainer& temp, Atom* partner)
{
	//1.) align templ with site
	Matrix4x4 tr_matr;
	TransformationProcessor tr_proc;
//	starAlign( site, temp, tr_matr );
	
	tr_proc.setTransformation(tr_matr);
	temp.apply(tr_proc);
	
	//2.) determine remaining atoms. take the first one that is compatible with 'partner'
	AtmVec unassigned_atoms;
//	getRemaining(site, temp, unassigned_atoms);
	
	String elem = partner->getElement().getSymbol();
	short bo = partner->getBond( *site[0])->getOrder();
	Atom* tmp;
//	tmp = getMatchingAtom( &*temp.beginAtom(), unassigned_atoms, elem, bo );
	partner->setPosition( tmp->getPosition() );
}
