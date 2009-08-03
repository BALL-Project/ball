// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyDatasetVector.C,v 1.1.4.1 2007-03-25 21:37:21 oliver Exp $
//

#include <BALL/PYTHON/pyDatasetVector.h>

namespace BALL
{

	PyDatasetVector::PyDatasetVector()
		: vector<Dataset*>()
	{
	}

	PyDatasetVector::PyDatasetVector(const PyDatasetVector& dvector)
		: vector<Dataset*>(dvector)
  {
	}

	PyDatasetVector::PyDatasetVector(const vector<Dataset*>& dvector)
		: vector<Dataset*>(dvector)
  {
	}

	PyDatasetVector& PyDatasetVector::operator = (const vector<Dataset*>& dvector)	
		throw()
	{
		clear();

		vector<Dataset*>::const_iterator it(dvector.begin());
		for (; it != dvector.end(); ++it)
		{
			push_back(const_cast<Dataset*>(*it));
		}

		return *this;
	}

	PyDatasetVector::~PyDatasetVector()
		throw()
	{
	}

}
