#include <BALL/PYTHON/EXTENSIONS/pyColorRGBAVector.h>

namespace BALL
{

	PyColorRGBAVector::PyColorRGBAVector()
		: vector<VIEW::ColorRGBA>()
	{
	}

	PyColorRGBAVector::PyColorRGBAVector(const PyColorRGBAVector& v)
		: vector<VIEW::ColorRGBA>(v)
  {
	}

	PyColorRGBAVector::PyColorRGBAVector(const vector<VIEW::ColorRGBA>& v)
		: vector<VIEW::ColorRGBA>(v)
  {
	}

	PyColorRGBAVector::~PyColorRGBAVector()
		throw()
	{
	}

}
