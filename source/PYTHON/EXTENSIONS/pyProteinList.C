// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyProteinList.C,v 1.1.2.1 2003/01/07 13:21:48 anker Exp $

#include <BALL/PYTHON/pyProteinList.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/atomContainer.h>

namespace BALL
{

	PyProteinList::PyProteinList()
		: List<Protein*>()
	{
	}

	PyProteinList::PyProteinList(const PyProteinList& new_list)
		: List<Protein*>(new_list)
  {
	}

	PyProteinList::~PyProteinList()
		throw()
	{
	}

	PyProteinList::PyProteinList(const AtomContainer& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	void PyProteinList::set(const AtomContainer& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all proteins
		AtomContainerConstIterator it = fragment.beginAtomContainer();

    for (; +it; ++it)
    {
      const Protein* protein = dynamic_cast<const Protein*>(&*it);
      if ((protein != 0) && (it->isSelected() || !selected_only))
      {
        // store the protein pointer in the list
        push_back(const_cast<Protein*>(protein));
			}
		}
	}
}
