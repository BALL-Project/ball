// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyRepresentationList.C,v 1.2 2003/11/15 12:53:58 oliver Exp $

#include <BALL/PYTHON/pyRepresentationList.h>
#include <BALL/VIEW/KERNEL/representation.h>

namespace BALL
{

	PyRepresentationList::PyRepresentationList()
		: List<VIEW::Representation*>()
	{
	}

	PyRepresentationList::PyRepresentationList(const PyRepresentationList& py_repr_list)
		: List<VIEW::Representation*>(py_repr_list)
  {
	}

	PyRepresentationList::PyRepresentationList(const List<VIEW::Representation*>& repr_list)
		: List<VIEW::Representation*>(repr_list)
  {
	}

	PyRepresentationList::~PyRepresentationList()
		throw()
	{
	}
}
