// $Id: pyMoleculeList.C,v 1.2 2000/08/30 19:58:38 oliver Exp $

#include <BALL/PYTHON/pyMoleculeList.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atomContainer.h>

namespace BALL
{

	PyMoleculeList::PyMoleculeList()
		: List<Molecule*>()
	{
	}

	PyMoleculeList::PyMoleculeList(const PyMoleculeList& new_list)
		: List<Molecule*>(new_list)
  {
	}

	PyMoleculeList::~PyMoleculeList()
	{
	}

	PyMoleculeList::PyMoleculeList(const AtomContainer& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	void PyMoleculeList::set(const AtomContainer& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all molecules
		AtomContainerConstIterator it = fragment.beginAtomContainer();

    for (; +it; ++it)
    {
      const Molecule* molecule = dynamic_cast<const Molecule*>(&*it);
      if ((molecule != 0) && (it->isSelected() || !selected_only))
      {
        // store the molecule pointer in the list
        push_back(const_cast<Molecule*>(molecule));
			}
		}
  }
}
