// $Id: pyNucleicAcidList.C,v 1.1 2000/07/18 21:18:48 oliver Exp $

#include <BALL/PYTHON/pyNucleicAcidList.h>
#include <BALL/KERNEL/nucleicAcid.h>
#include <BALL/KERNEL/baseFragment.h>

namespace BALL
{

	PyNucleicAcidList::PyNucleicAcidList()
		: List<NucleicAcid*>()
	{
	}

	PyNucleicAcidList::PyNucleicAcidList(const PyNucleicAcidList& new_list)
		: List<NucleicAcid*>(new_list)
  {
	}

	PyNucleicAcidList::~PyNucleicAcidList()
	{
	}

	PyNucleicAcidList::PyNucleicAcidList(const BaseFragment& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	void PyNucleicAcidList::set(const BaseFragment& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all nucleic acids
		BaseFragmentConstIterator it = fragment.beginBaseFragment();

    for (; +it; ++it)
    {
      const NucleicAcid* nucleic_acid = dynamic_cast<const NucleicAcid*>(&*it);
      if ((nucleic_acid != 0) && (it->isSelected() || !selected_only))
      {
        // store the nucleic_acid pointer in the list
        push_back(const_cast<NucleicAcid*>(nucleic_acid));
			}
		}
	}
}
