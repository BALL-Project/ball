// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyAtomList.C,v 1.5.8.1 2007/03/25 21:37:18 oliver Exp $
//
// Author:
//   Oliver Kohlbacher
//

#include <BALL/PYTHON/pyAtomList.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/expression.h>
#include <BALL/KERNEL/atomContainer.h>

namespace BALL
{

	PyAtomList::PyAtomList()
		: AtomList()
	{
	}

	PyAtomList::PyAtomList(const PyAtomList& atom_list)
		: AtomList(atom_list)
  {
	}

	PyAtomList::PyAtomList(const AtomList& atom_list)
		: AtomList(atom_list)
  {
	}

	PyAtomList::~PyAtomList()
		throw()
	{
	}

	PyAtomList::PyAtomList(const AtomContainer& fragment, const String& expression)
	{
		set(fragment, expression);
	}

	PyAtomList::PyAtomList(const AtomContainer& fragment)
	{
		set(fragment, "");
	}

	PyAtomList::PyAtomList(const Composite& composite)
	{
		clear();
		if (RTTI::isKindOf<AtomContainer>(composite))
		{
			set(*(AtomContainer*)&composite);
		}
		else if (RTTI::isKindOf<Atom>(composite))
		{
			push_back((Atom*)&composite);
		}
		else if (RTTI::isKindOf<Bond>(composite))
		{
			Bond* bond = (Bond*)&composite;
			push_back((Atom*)bond->getFirstAtom());
			push_back((Atom*)bond->getSecondAtom());
		}
	}


	void PyAtomList::set(const AtomContainer& fragment, const String& expression)
	{
		// clear the old contents of the list
		clear();

		// iterate over all atoms
		AtomConstIterator it = fragment.beginAtom();

		if (expression == "")
		{
			for (; +it; ++it)
			{
				// store the atom pointer in the list
				push_back(const_cast<Atom*>(&*it));
			}
		}
		else 
		{
			Expression match(expression);
			for (; +it; ++it)
			{
				if (match(*it))
				{
					// store the atom pointer in the list
					push_back(const_cast<Atom*>(&*it));
				}
			}
		}
	}

}
