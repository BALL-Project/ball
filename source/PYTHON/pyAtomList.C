// $Id: pyAtomList.C,v 1.1 2000/06/27 07:27:02 oliver Exp $

#include <BALL/PYTHON/pyAtomList.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/baseFragment.h>

namespace BALL
{

	PyAtomList::PyAtomList()
		: List<Atom*>()
	{
	}

	PyAtomList::PyAtomList(const PyAtomList& new_list)
		: List<Atom*>(new_list)
  {
	}

	PyAtomList::~PyAtomList()
	{
	}

	PyAtomList::PyAtomList(const BaseFragment& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	void PyAtomList::set(const BaseFragment& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all atoms
		AtomConstIterator it = fragment.beginAtom();

		// we use two separate loops to speed up
		// the processing for the case of selected_only == false
		if (selected_only)
		{
			for (; +it; ++it)
			{
				if (it->isSelected())
				{
					// store the atom pointer in the list
					push_back(const_cast<Atom*>(&*it));
				}
			}
		}
		else 
		{
			for (; +it; ++it)
			{
				// store the atom pointer in the list
				push_back(const_cast<Atom*>(&*it));
			}
		}
	}
}
