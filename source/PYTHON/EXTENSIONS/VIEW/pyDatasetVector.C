#include <BALL/PYTHON/EXTENSIONS/pyDatasetVector.h>

namespace BALL
{

	PyDatasetVector::PyDatasetVector()
		: vector<VIEW::Dataset*>()
	{
	}

	PyDatasetVector::PyDatasetVector(const PyDatasetVector& dvector)
		: vector<VIEW::Dataset*>(dvector)
  {
	}

	PyDatasetVector::PyDatasetVector(const std::vector<VIEW::Dataset*>& dvector)
		: vector<VIEW::Dataset*>(dvector)
	{
	}

	PyDatasetVector& PyDatasetVector::operator = (const std::vector<VIEW::Dataset*>& dvector)
		throw()
	{
		clear();

		std::vector<VIEW::Dataset*>::const_iterator it(dvector.begin());
		for (; it != dvector.end(); ++it)
		{
			push_back(const_cast<VIEW::Dataset*>(*it));
		}

		return *this;
	}

	PyDatasetVector::~PyDatasetVector()
		throw()
	{
	}

}
