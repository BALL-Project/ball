// $Id: pyPDBAtomList.C,v 1.2 2000/07/03 11:11:12 oliver Exp $

#include <BALL/PYTHON/pyPDBAtomList.h>
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/baseFragment.h>

namespace BALL
{

	PyPDBAtomList::PyPDBAtomList()
		: List<PDBAtom*>()
	{
	}

	PyPDBAtomList::PyPDBAtomList(const PyPDBAtomList& new_list)
		: List<PDBAtom*>(new_list)
  {
	}

	PyPDBAtomList::~PyPDBAtomList()
	{
	}

	PyPDBAtomList::PyPDBAtomList(const BaseFragment& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	void PyPDBAtomList::set(const BaseFragment& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all PDBAtoms
		AtomConstIterator it = fragment.beginAtom();

    for (; +it; ++it)
    {
      const PDBAtom* pdb_atom = dynamic_cast<const PDBAtom*>(&*it);
      if ((pdb_atom != 0) && (it->isSelected() || !selected_only))
      {
        // store the pdb atom pointer in the list
        push_back(const_cast<PDBAtom*>(pdb_atom));
			}
		}
	}
}
