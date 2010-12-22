// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: connectivityDescriptors.C,v 1.1.28.1 2007/03/16 00:06:46 bertsch Exp $
// 

#include <BALL/QSAR/connectivityDescriptors.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/fragment.h>

namespace BALL
{

	//////////////////
	// Zagreb index
	//////////////////
	ZagrebIndex::ZagrebIndex()
  	: ConnectivityBase("ZagrebIndex", "")
	{
	}

	ZagrebIndex::ZagrebIndex(const ZagrebIndex& zi)
		:	ConnectivityBase(zi)
	{
	}

	ZagrebIndex::~ZagrebIndex()
	{
	}

	ZagrebIndex& ZagrebIndex::operator = (const ZagrebIndex& zi)
	{
		this->setName(zi.getName());
		this->setUnit(zi.getUnit());
		return *this;
	}

	double ZagrebIndex::compute(AtomContainer& ac)
	{
		//if (!isValid_(ac))
		//{
		// do nothing...
		//}
		double sum = 0;
		AtomConstIterator atom_it = ac.beginAtom();
		BALL_FOREACH_ATOM (ac, atom_it)
		{
			if (atom_it->getElement() != PTE[Element::H])
			{
				Atom::BondConstIterator bond_it;
				int delta = 0; // heavy degree
				for (bond_it = atom_it->beginBond(); bond_it != atom_it->endBond(); bond_it++)
				{
					if (bond_it->getBoundAtom(*atom_it)->getElement() != PTE[Element::H])
					{
						delta++;
					}
				}
				sum += delta*delta;
			}
		}
		//ac.setProperty("ZagrebIndex", sum);
		return sum;
	}


	/////////////////////
	// Balabans index j
	////////////////////
	BalabanIndexJ::BalabanIndexJ()
		:	ConnectivityBase("BalabanIndexJ", "")
	{
	}

	BalabanIndexJ::BalabanIndexJ(const BalabanIndexJ& bij)
		:	ConnectivityBase(bij)
	{
	}

	BalabanIndexJ::~BalabanIndexJ()
	{
	}

	BalabanIndexJ& BalabanIndexJ::operator = (const BalabanIndexJ& bij)
	{
		this->setName(bij.getName());
		this->setUnit(bij.getUnit());
		return *this;
	}

} //namespace BALL

