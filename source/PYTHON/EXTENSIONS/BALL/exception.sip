// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class GeneralException 
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	
	public:
	GeneralException();
	GeneralException
    (const char*, int);
	
	GeneralException
    (const char*, int,	
     const String&,
     const String&);

	GeneralException(const GeneralException&);
	~GeneralException();
	const char* getName() const;
	const char* getMessage() const;
	int getLine() const;
	const char* getFile() const;

 	SIP_PYOBJECT __repr__();
%MethodCode
  if (sipCpp == 0)
  {
    sipRes = PyString_FromString("");
  }
  else
  {
    sipRes = PyString_FromString(String(String(sipCpp->getName())
					+ " in line " + String(sipCpp->getLine()) + " of "
          + String(sipCpp->getFile()) + ": " + String(sipCpp->getMessage())).c_str());
  }
%End

};  


class IndexUnderflow 
 	: GeneralException
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  IndexUnderflow(const char*, int, Index index = 0, Size size = 0);
 	SIP_PYOBJECT __repr__();
%MethodCode
  if (sipCpp == 0)
  {
    sipRes = PyString_FromString("");
  }
  else
  {
    sipRes = PyString_FromString(String(String(sipCpp->getName())
					+ " in line " + String(sipCpp->getLine()) + " of "
          + String(sipCpp->getFile()) + ": " + String(sipCpp->getMessage())).c_str());
  }
%End
};

class SizeUnderflow 
  : GeneralException
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  SizeUnderflow(const char*, int, Size size = 0);
 	SIP_PYOBJECT __repr__();
%MethodCode
  if (sipCpp == 0)
  {
    sipRes = PyString_FromString("");
  }
  else
  {
    sipRes = PyString_FromString(String(String(sipCpp->getName())
					+ " in line " + String(sipCpp->getLine()) + " of "
          + String(sipCpp->getFile()) + ": " + String(sipCpp->getMessage())).c_str());
  }
%End

};


class IndexOverflow 
  : GeneralException
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	IndexOverflow(const char*, int, Index index = 0, Size size = 0);
 	SIP_PYOBJECT __repr__();
%MethodCode
  if (sipCpp == 0)
  {
    sipRes = PyString_FromString("");
  }
  else
  {
    sipRes = PyString_FromString(String(String(sipCpp->getName())
					+ " in line " + String(sipCpp->getLine()) + " of "
          + String(sipCpp->getFile()) + ": " + String(sipCpp->getMessage())).c_str());
  }
%End
};

class InvalidRange 
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	InvalidRange(const char*, int, float);
 	SIP_PYOBJECT __repr__();
%MethodCode
  if (sipCpp == 0)
  {
    sipRes = PyString_FromString("");
  }
  else
  {
    sipRes = PyString_FromString(String(String(sipCpp->getName())
					+ " in line " + String(sipCpp->getLine()) + " of "
          + String(sipCpp->getFile()) + ": " + String(sipCpp->getMessage())).c_str());
  }
%End
};

class InvalidFormat 
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	InvalidFormat(const char*, int, const String&);
 	SIP_PYOBJECT __repr__();
%MethodCode
  if (sipCpp == 0)
  {
    sipRes = PyString_FromString("");
  }
  else
  {
    sipRes = PyString_FromString(String(String(sipCpp->getName())
					+ " in line " + String(sipCpp->getLine()) + " of "
          + String(sipCpp->getFile()) + ": " + String(sipCpp->getMessage())).c_str());
  }
%End
};

class IllegalSelfOperation
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	IllegalSelfOperation(const char*, int);
 	SIP_PYOBJECT __repr__();
%MethodCode
  if (sipCpp == 0)
  {
    sipRes = PyString_FromString("");
  }
  else
  {
    sipRes = PyString_FromString(String(String(sipCpp->getName())
					+ " in line " + String(sipCpp->getLine()) + " of "
          + String(sipCpp->getFile()) + ": " + String(sipCpp->getMessage())).c_str());
  }
%End
};

class NullPointer 
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	NullPointer(const char*, int);
 	SIP_PYOBJECT __repr__();
%MethodCode
  if (sipCpp == 0)
  {
    sipRes = PyString_FromString("");
  }
  else
  {
    sipRes = PyString_FromString(String(String(sipCpp->getName())
					+ " in line " + String(sipCpp->getLine()) + " of "
          + String(sipCpp->getFile()) + ": " + String(sipCpp->getMessage())).c_str());
  }
%End
};

class InvalidIterator
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	InvalidIterator(const char*, int);
 	SIP_PYOBJECT __repr__();
%MethodCode
  if (sipCpp == 0)
  {
    sipRes = PyString_FromString("");
  }
  else
  {
    sipRes = PyString_FromString(String(String(sipCpp->getName())
					+ " in line " + String(sipCpp->getLine()) + " of "
          + String(sipCpp->getFile()) + ": " + String(sipCpp->getMessage())).c_str());
  }
%End
};

class IncompatibleIterators
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	IncompatibleIterators(const char*, int);
 	SIP_PYOBJECT __repr__();
%MethodCode
  if (sipCpp == 0)
  {
    sipRes = PyString_FromString("");
  }
  else
  {
    sipRes = PyString_FromString(String(String(sipCpp->getName())
					+ " in line " + String(sipCpp->getLine()) + " of "
          + String(sipCpp->getFile()) + ": " + String(sipCpp->getMessage())).c_str());
  }
%End
};

class NotImplemented
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	NotImplemented(const char*, int);
 	SIP_PYOBJECT __repr__();
%MethodCode
  if (sipCpp == 0)
  {
    sipRes = PyString_FromString("");
  }
  else
  {
    sipRes = PyString_FromString(String(String(sipCpp->getName())
					+ " in line " + String(sipCpp->getLine()) + " of "
          + String(sipCpp->getFile()) + ": " + String(sipCpp->getMessage())).c_str());
  }
%End
};

class InvalidOption
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
		InvalidOption(const char* file, int line, String option);

		SIP_PYOBJECT __repr__();
%MethodCode
	if(sipCpp == 0)
	{
		sipRes = PyString_FromString("");
	}
	else
	{
		sipRes = PyString_FromString(String(String(sipCpp->getName())
		         + " in line " + String(sipCpp->getLine()) + " of "
		         + String(sipCpp->getFile()) + ": " + String(sipCpp->getMessage())).c_str());
	}
%End
};

class TooManyErrors
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	TooManyErrors(const char*, int);
 	SIP_PYOBJECT __repr__();
%MethodCode
  if (sipCpp == 0)
  {
    sipRes = PyString_FromString("");
  }
  else
  {
    sipRes = PyString_FromString(String(String(sipCpp->getName())
					+ " in line " + String(sipCpp->getLine()) + " of "
          + String(sipCpp->getFile()) + ": " + String(sipCpp->getMessage())).c_str());
  }
%End
};


class OutOfMemory
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	OutOfMemory(const char*, int, Size size = 0);
	~OutOfMemory() throw();
 	SIP_PYOBJECT __repr__();
%MethodCode
  if (sipCpp == 0)
  {
    sipRes = PyString_FromString("");
  }
  else
  {
    sipRes = PyString_FromString(String(String(sipCpp->getName())
					+ " in line " + String(sipCpp->getLine()) + " of "
          + String(sipCpp->getFile()) + ": " + String(sipCpp->getMessage())).c_str());
  }
%End
};

class OutOfGrid
  : GeneralException
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	OutOfGrid(const char*, int)
    throw();
 	SIP_PYOBJECT __repr__();
%MethodCode
  if (sipCpp == 0)
  {
    sipRes = PyString_FromString("");
  }
  else
  {
    sipRes = PyString_FromString(String(String(sipCpp->getName())
					+ " in line " + String(sipCpp->getLine()) + " of "
          + String(sipCpp->getFile()) + ": " + String(sipCpp->getMessage())).c_str());
  }
%End
};

  
class BufferOverflow 
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	BufferOverflow(const char*, int);
 	SIP_PYOBJECT __repr__();
%MethodCode
  if (sipCpp == 0)
  {
    sipRes = PyString_FromString("");
  }
  else
  {
    sipRes = PyString_FromString(String(String(sipCpp->getName())
					+ " in line " + String(sipCpp->getLine()) + " of "
          + String(sipCpp->getFile()) + ": " + String(sipCpp->getMessage())).c_str());
  }
%End
};

class DivisionByZero 
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	DivisionByZero(const char*, int);
 	SIP_PYOBJECT __repr__();
%MethodCode
  if (sipCpp == 0)
  {
    sipRes = PyString_FromString("");
  }
  else
  {
    sipRes = PyString_FromString(String(String(sipCpp->getName())
					+ " in line " + String(sipCpp->getLine()) + " of "
          + String(sipCpp->getFile()) + ": " + String(sipCpp->getMessage())).c_str());
  }
%End
};

class FileNotFound 
 : GeneralException
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  FileNotFound(const char*, int, const String&);
  String getFilename() const;
 	SIP_PYOBJECT __repr__();
%MethodCode
  if (sipCpp == 0)
  {
    sipRes = PyString_FromString("");
  }
  else
  {
    sipRes = PyString_FromString(String(String(sipCpp->getName())
					+ " in line " + String(sipCpp->getLine()) + " of "
          + String(sipCpp->getFile()) + ": " + String(sipCpp->getMessage())).c_str());
  }
%End
};

class ParseError
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	ParseError(const char*, int, const String&,
 	  				 const String&)
							throw();
 	SIP_PYOBJECT __repr__();
%MethodCode
  if (sipCpp == 0)
  {
    sipRes = PyString_FromString("");
  }
  else
  {
    sipRes = PyString_FromString(String(String(sipCpp->getName())
					+ " in line " + String(sipCpp->getLine()) + " of "
          + String(sipCpp->getFile()) + ": " + String(sipCpp->getMessage())).c_str());
  }
%End
};

class TooManyBonds
 : GeneralException
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
  #include <BALL/COMMON/exception.h>
  using namespace BALL;
  using namespace BALL::Exception;
%End
  public:
  TooManyBonds(const char*, int, const String) throw();
};


/// creates a crash under windows:
class GlobalExceptionHandler
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	GlobalExceptionHandler();
	static void setName(const String&);
	static void setMessage(const String&);
	static void setLine(int);
	static void setFile(const String&);
	static void set
		(const String&, int, 
		 const String&, const String&);
};	

GlobalExceptionHandler globalHandler;

