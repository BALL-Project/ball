// $Id: pyResidueList.C,v 1.2 2000/08/30 19:58:39 oliver Exp $

#include <BALL/PYTHON/pyResidueList.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/atomContainer.h>

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

	PyResidueList::PyResidueList(const AtomContainer& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	void PyResidueList::set(const AtomContainer& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all residues
		AtomContainerConstIterator it = fragment.beginAtomContainer();

    for (; +it; ++it)
    {
			const Residue* residue = dynamic_cast<const Residue*>(&*it);
      if ((residue != 0) && (it->isSelected() || !selected_only))
      {
        // store the residue pointer in the list
        push_back(const_cast<Residue*>(residue));
			}
		}
	}
}
