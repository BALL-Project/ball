// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyNucleotideList.C,v 1.1.2.1 2003/01/07 13:21:47 anker Exp $

#include <BALL/PYTHON/pyNucleotideList.h>
#include <BALL/KERNEL/nucleotide.h>
#include <BALL/KERNEL/atomContainer.h>

namespace BALL
{

	PyNucleotideList::PyNucleotideList()
		: List<Nucleotide*>()
	{
	}

	PyNucleotideList::PyNucleotideList(const PyNucleotideList& new_list)
		: List<Nucleotide*>(new_list)
  {
	}

	PyNucleotideList::~PyNucleotideList()
		throw()
	{
	}

	PyNucleotideList::PyNucleotideList(const AtomContainer& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	void PyNucleotideList::set(const AtomContainer& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all nucleotides
		AtomContainerConstIterator it = fragment.beginAtomContainer();

    for (; +it; ++it)
    {
      const Nucleotide* nucleotide = dynamic_cast<const Nucleotide*>(&*it);
      if ((nucleotide != 0) && (it->isSelected() || !selected_only))
      {
        // store the nucleotide pointer in the list
        push_back(const_cast<Nucleotide*>(nucleotide));
			}
		}
	}
}
