// $Id: pyAtomContainerList.C,v 1.2 2001/07/25 11:34:51 oliver Exp $

#include <BALL/PYTHON/pyAtomContainerList.h>
#include <BALL/KERNEL/atomContainer.h>

namespace BALL
{

	PyAtomContainerList::PyAtomContainerList()
		: List<AtomContainer*>()
	{
	}

	PyAtomContainerList::PyAtomContainerList(const PyAtomContainerList& new_list)
		: List<AtomContainer*>(new_list)
  {
	}

	PyAtomContainerList::~PyAtomContainerList()
		throw()
	{
	}

	PyAtomContainerList::PyAtomContainerList(const AtomContainer& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	void PyAtomContainerList::set(const AtomContainer& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all base fragments
		AtomContainerConstIterator it = fragment.beginAtomContainer();
		for (; +it; ++it)
		{
			if (!selected_only || it->isSelected())
			{
				// store the base fragment pointer in the list
				push_back(const_cast<AtomContainer*>(&*it));
			}
		}
	}
}
