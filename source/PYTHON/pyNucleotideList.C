// $Id: pyNucleotideList.C,v 1.1 2000/06/27 13:10:26 oliver Exp $

#include <BALL/PYTHON/pyNucleotideList.h>
#include <BALL/KERNEL/nucleotide.h>
#include <BALL/KERNEL/baseFragment.h>

namespace BALL
{

	PyNucleotideList::PyNucleotideList()
		: List<Nucleotide*>()
	{
	}

	PyNucleotideList::PyNucleotideList(const PyNucleotideList& new_list)
		: List<Nucleotide*>(new_list)
  {
	}

	PyNucleotideList::~PyNucleotideList()
	{
	}

	PyNucleotideList::PyNucleotideList(const BaseFragment& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	void PyNucleotideList::set(const BaseFragment& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all nucleotides
		BaseFragmentConstIterator it = fragment.beginBaseFragment();

    for (; +it; ++it)
    {
      Nucleotide* nucleotide = const_cast<Nucleotide*>(&dynamic_cast<const Nucleotide&>(*it));
      if ((nucleotide != 0) && (it->isSelected() || !selected_only))
      {
        // store the nucleotide pointer in the list
        push_back(nucleotide);
			}
		}
	}
}
