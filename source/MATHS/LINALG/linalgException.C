#include <BALL/MATHS/LINALG/linalgException.h>
#include <BALL/DATATYPE/string.h>

using namespace BALL::Exception;


MatrixHasWrongDimension::MatrixHasWrongDimension() throw()
{
	name_= "MatrixHasWrongDimension";
	BALL::Exception::globalHandler.setName(String(name_));
}

MatrixHasWrongDimension::MatrixHasWrongDimension(const char* file, unsigned long line) throw()
	: GeneralException(file,line)
{
	name_= "MatrixHasWrongDimension";
	BALL::Exception::globalHandler.setName(String(name_));
}

MatrixHasWrongDimension::~MatrixHasWrongDimension() throw()
{
}

VectorHasWrongDimension::VectorHasWrongDimension() throw()
{
	name_= "VectorHasWrongDimension";
	BALL::Exception::globalHandler.setName(String(name_));
}

VectorHasWrongDimension::VectorHasWrongDimension(const char* file, unsigned long line) throw()
	: GeneralException(file,line)
{
	name_= "VectorHasWrongDimension";
	BALL::Exception::globalHandler.setName(String(name_));
}

VectorHasWrongDimension::~VectorHasWrongDimension() throw()
{
}

MatrixNotRegular::MatrixNotRegular() throw()
{
	name_= "MatrixNotRegular";
	BALL::Exception::globalHandler.setName(String(name_));
}

MatrixNotRegular::~MatrixNotRegular() throw()
{
}

MatrixIsSingular::MatrixIsSingular(const char* file, unsigned long line) 
	throw() : GeneralException(file,line)
{
	name_= "MatrixIsSingular";
	BALL::Exception::globalHandler.setName(String(name_));
}

MatrixIsSingular::~MatrixIsSingular() throw()
{
}

MatrixNotQuadratic::MatrixNotQuadratic() throw()
{
	name_= "MatrixNotQuadratic";
	BALL::Exception::globalHandler.setName(String(name_));
}

MatrixNotQuadratic::~MatrixNotQuadratic() throw()
{
}

SingularIterator::SingularIterator() throw()
{
	name_= "SingularIterator";
	BALL::Exception::globalHandler.setName(String(name_));
}

SingularIterator::~SingularIterator() throw()
{
}

Uninitialized::Uninitialized() throw()
{
	name_= "Uninitialized";
	BALL::Exception::globalHandler.setName(String(name_));
}

Uninitialized::~Uninitialized() throw()
{
}
