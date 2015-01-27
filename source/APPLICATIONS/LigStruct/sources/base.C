// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "base.h"


#include <BALL/DATATYPE/string.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/atomContainer.h>

#include <BALL/KERNEL/forEach.h>
#include <vector>


/// Name Spaces
using namespace BALL;
using namespace std;

String LigBase::printInlineMol(Composite *mol)
{
	String tmp = "";
	AtomIterator pre = ((AtomContainer*)mol)->endAtom();
	--pre;
	for(AtomIterator ati = ((AtomContainer*)mol)->beginAtom(); +ati; ++ati)
	{
		tmp += ati->getElement().getSymbol();
		tmp += String( countBondsAndOrder(*ati ) );
		if( ati != pre )
			tmp += "-";
	}
	return tmp;
}

String LigBase::printMol(Composite *mol)
{
	String tmp = "";
	for(AtomIterator ati = ((AtomContainer*)mol)->beginAtom(); +ati; ++ati)
	{
		tmp += ati->getElement().getSymbol();
		tmp += " " + String( ati->countBonds() );
		tmp += "\n";
	}
	return tmp;
}

String LigBase::printInlineStarMol(Composite* mol)
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


String LigBase::printInlineStarMol(AtmVec& mol)
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

int LigBase::countBondsAndOrder(Atom &atm)
{
	int cnt = 0;
	for( Atom::BondIterator bit = atm.beginBond(); +bit; ++bit )
	{
		cnt += bit->getOrder();
	}
	return cnt;
}

int LigBase::countBondsInPartent(Atom &atm, const Composite &parent)
{
	int cnt = 0;
	for( Atom::BondIterator bit = atm.beginBond(); +bit; ++bit )
	{
		if( bit->getBoundAtom( atm )->getParent() == &parent)
			cnt++;
	}
	return cnt;
}

const int LigBase::getAtomPosition(Atom *atm, AtomContainer *mol)
{
	AtomIterator ati = mol->beginAtom();
	for (int i = 0; +ati; ati++, i++)
	{
		if(&*ati == atm)
			return i;
	}
	return -1;
}

// Translate the AtomContainer 'fromMol' into an AtmVec 'toMol'
void LigBase::toAtmVec(AtomContainer &fromMol, AtmVec &toMol)
{
	for(AtomIterator ati = fromMol.beginAtom(); +ati; ++ati)
		toMol.push_back(&*ati);
}

void LigBase::transferMolecule(AtomContainer *toMol, AtomContainer *fromMol)
{
	int num_atm = fromMol->countAtoms();
	for(int i = 0; i < num_atm; i++)
		toMol->insert( *fromMol->beginAtom() ); // insert auto removes from its old parent
}

void LigBase::clearExternalBonds(AtomContainer *mol)
{
	Atom::BondIterator bit;
	AtomIterator ait;

	BALL_FOREACH_INTERBOND(*mol, ait, bit)
	{
		bit->destroy();
	}
}

void LigBase::copyMoleculeProperies(AtomContainer &orig, AtomContainer &cop)
{
	NamedPropertyIterator  it;
	for(it = orig.beginNamedProperty (); it !=orig.endNamedProperty(); it++)
	{
		cop.setProperty(*it);
	}
	cop.setName(orig.getName());
}


