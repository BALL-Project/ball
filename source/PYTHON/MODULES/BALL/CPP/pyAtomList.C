// $Id: pyAtomList.C,v 1.1 2000/07/18 21:18:37 oliver Exp $

#include <BALL/PYTHON/pyAtomList.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/expression.h>
#include <BALL/KERNEL/baseFragment.h>

namespace BALL
{

	PyAtomList::PyAtomList()
		: List<Atom*>()
	{
	}

	PyAtomList::PyAtomList(const PyAtomList& new_list)
		: List<Atom*>(new_list)
  {
	}

	PyAtomList::~PyAtomList()
	{
	}

	PyAtomList::PyAtomList(const BaseFragment& fragment, const String& expression)
	{
		set(fragment, expression);
	}

	PyAtomList::PyAtomList(const BaseFragment& fragment)
	{
		set(fragment, "");
	}

	void PyAtomList::set(const BaseFragment& fragment, const String& expression)
	{
		// clear the old contents of the list
		clear();

		// iterate over all atoms
		AtomConstIterator it = fragment.beginAtom();

		if (expression == "")
		{
			for (; +it; ++it)
			{
				// store the atom pointer in the list
				push_back(const_cast<Atom*>(&*it));
			}
		}
		else 
		{
			Expression match(expression);
			for (; +it; ++it)
			{
				if (match(*it))
				{
					// store the atom pointer in the list
					push_back(const_cast<Atom*>(&*it));
				}
			}
		}
	}

}
