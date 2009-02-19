// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDBRecords.C,v 1.1 2005/02/08 17:32:37 oliver Exp $
//

#include <BALL/FORMAT/PDBRecords.h>

namespace BALL 
{

	PDBRecords::PDBRecords() 
		:	records_(500)
	{
	}

	PDBRecords::PDBRecords(const PDBRecords& pdbr) 
		:	records_(pdbr.records_)
	{
	}

	PDBRecords::~PDBRecords() 
	{
		records_.clear();
	}

	PDBRecords& PDBRecords::operator = (const PDBRecords& rhs)
	{
		if (this != &rhs)
		{
			records_ = rhs.records_;
		}
		return *this;
	}

} // namespace BALL
