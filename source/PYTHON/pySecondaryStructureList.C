// $Id: pySecondaryStructureList.C,v 1.1 2000/06/27 13:10:30 oliver Exp $

#include <BALL/PYTHON/pySecondaryStructureList.h>
#include <BALL/KERNEL/secondaryStructure.h>
#include <BALL/KERNEL/baseFragment.h>

namespace BALL
{

	PySecondaryStructureList::PySecondaryStructureList()
		: List<SecondaryStructure*>()
	{
	}

	PySecondaryStructureList::PySecondaryStructureList(const PySecondaryStructureList& new_list)
		: List<SecondaryStructure*>(new_list)
  {
	}

	PySecondaryStructureList::~PySecondaryStructureList()
	{
	}

	PySecondaryStructureList::PySecondaryStructureList(const BaseFragment& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	void PySecondaryStructureList::set(const BaseFragment& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all secondary structures
		BaseFragmentConstIterator it = fragment.beginBaseFragment();

    for (; +it; ++it)
    {
      SecondaryStructure* sec_struct = const_cast<SecondaryStructure*>(&dynamic_cast<const SecondaryStructure&>(*it));
      if ((sec_struct != 0) && (it->isSelected() || !selected_only))
      {
        // store the sec_struct pointer in the list
        push_back(sec_struct);
			}
		}
	}
}
