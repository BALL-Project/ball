// $Id: pyProteinList.C,v 1.2 2000/07/03 11:11:12 oliver Exp $

#include <BALL/PYTHON/pyProteinList.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/baseFragment.h>

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
	{
	}

	PyProteinList::PyProteinList(const BaseFragment& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	void PyProteinList::set(const BaseFragment& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all proteins
		BaseFragmentConstIterator it = fragment.beginBaseFragment();

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
