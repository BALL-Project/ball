// $Id: pyPDBAtomList.C,v 1.2 2000/08/30 19:58:39 oliver Exp $

#include <BALL/PYTHON/pyPDBAtomList.h>
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/expression.h>
#include <BALL/KERNEL/atomContainer.h>

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

	PyPDBAtomList::PyPDBAtomList(const AtomContainer& fragment, const String& expression)
	{
		set(fragment, expression);
	}

	PyPDBAtomList::PyPDBAtomList(const AtomContainer& fragment)
	{
		set(fragment, "");
	}

	void PyPDBAtomList::set(const AtomContainer& fragment, const String& expression)
	{
		// clear the old contents of the list
		clear();

		if (expression == "")
		{
			// iterate over all PDBAtoms
			AtomConstIterator it = fragment.beginAtom();

			for (; +it; ++it)
			{
				const PDBAtom* pdb_atom = dynamic_cast<const PDBAtom*>(&*it);
				if (pdb_atom != 0)
				{
					// store the pdb atom pointer in the list
					push_back(const_cast<PDBAtom*>(pdb_atom));
				}
			}
		}
		else
		{
			Expression match(expression);

			// iterate over all PDBAtoms
			AtomConstIterator it = fragment.beginAtom();

			for (; +it; ++it)
			{
				const PDBAtom* pdb_atom = dynamic_cast<const PDBAtom*>(&*it);
				if ((pdb_atom != 0) && match(*pdb_atom))
				{
					// store the pdb atom pointer in the list
					push_back(const_cast<PDBAtom*>(pdb_atom));
				}
			}
		}
	}
}
