// $Id: pyNucleicAcidList.C,v 1.1 2001/09/03 16:53:13 oliver Exp $

#include <BALL/PYTHON/pyNucleicAcidList.h>
#include <BALL/KERNEL/nucleicAcid.h>
#include <BALL/KERNEL/atomContainer.h>

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
		throw()
	{
	}

	PyNucleicAcidList::PyNucleicAcidList(const AtomContainer& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	void PyNucleicAcidList::set(const AtomContainer& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all nucleic acids
		AtomContainerConstIterator it = fragment.beginAtomContainer();

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
