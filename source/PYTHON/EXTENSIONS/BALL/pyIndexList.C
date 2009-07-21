// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyIndexList.C,v 1.2 2005-12-23 17:02:56 amoll Exp $
//

#include <BALL/PYTHON/pyIndexList.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/expression.h>
#include <BALL/KERNEL/atomContainer.h>

namespace BALL
{

	PyIndexList::PyIndexList()
		: list<Index>()
	{
	}

	PyIndexList::PyIndexList(const PyIndexList& new_list)
		: list<Index>(new_list)
  {
	}

	PyIndexList::~PyIndexList()
		throw()
	{
	}

 	PyIndexList::PyIndexList(const std::list<Index>& idx_list)
	{
		operator = (idx_list);
	}

	PyIndexList::PyIndexList(const std::list<Position>& pos_list)
	{
		operator = (pos_list);
	}

 	PyIndexList::PyIndexList(const std::vector<Index>& idx_vector)
	{
		operator = (idx_vector);
	}

	PyIndexList::PyIndexList(const std::vector<Position>& pos_vector)
	{
		operator = (pos_vector);
	}

	PyIndexList& PyIndexList::operator = (const std::list<Index>& idx_list)
	{
		// clear the old contents
		clear();
		
		std::copy(idx_list.begin(), idx_list.end(), std::back_inserter<PyIndexList>(*this));		

		return *this;
	}

	PyIndexList& PyIndexList::operator = (const std::list<Position>& pos_list)
	{
		// clear the old contents
		clear();
		
		std::copy(pos_list.begin(), pos_list.end(), std::back_inserter<PyIndexList>(*this));		

		return *this;
	}

	PyIndexList& PyIndexList::operator = (const std::vector<Index>& idx_vector)
	{
		// clear the old contents
		clear();
		
		std::copy(idx_vector.begin(), idx_vector.end(), std::back_inserter<PyIndexList>(*this));		

		return *this;
	}

	PyIndexList& PyIndexList::operator = (const std::vector<Position>& pos_vector)
	{
		// clear the old contents
		clear();
		
		std::copy(pos_vector.begin(), pos_vector.end(), std::back_inserter<PyIndexList>(*this));		

		return *this;
	}

}
