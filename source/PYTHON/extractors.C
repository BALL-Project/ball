// $Id: extractors.C,v 1.1 2000/06/27 07:27:01 oliver Exp $

#include <BALL/PYTHON/pyAtomList.h>
#include <BALL/PYTHON/pyPDBAtomList.h>
#include <BALL/KERNEL/baseFragment.h>

namespace BALL
{
	PyAtomList* atoms(const BaseFragment& fragment, bool selected_only)
	{
		return new PyAtomList(fragment, selected_only);
	}

	PyAtomList* PDBAtoms(const BaseFragment& fragment, bool selected_only)
	{
		return new PyPDBAtomList(fragment, selected_only);
	}
}
