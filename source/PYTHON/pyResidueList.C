// $Id: pyResidueList.C,v 1.1 2000/06/27 13:10:29 oliver Exp $

#include <BALL/PYTHON/pyResidueList.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/baseFragment.h>

namespace BALL
{

	PyResidueList::PyResidueList()
		: List<Residue*>()
	{
	}

	PyResidueList::PyResidueList(const PyResidueList& new_list)
		: List<Residue*>(new_list)
  {
	}

	PyResidueList::~PyResidueList()
	{
	}

	PyResidueList::PyResidueList(const BaseFragment& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	void PyResidueList::set(const BaseFragment& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all residues
		BaseFragmentConstIterator it = fragment.beginBaseFragment();

    for (; +it; ++it)
    {
      Residue* residue = const_cast<Residue*>(&dynamic_cast<const Residue&>(*it));
      if ((residue != 0) && (it->isSelected() || !selected_only))
      {
        // store the residue pointer in the list
        push_back(residue);
			}
		}
	}
}
