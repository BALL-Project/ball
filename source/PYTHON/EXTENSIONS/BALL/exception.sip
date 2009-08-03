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
    (const char* /file/, int /line/);
	
	GeneralException
    (const char* /file/, int /line/,	
     const String& /name/,
     const String& /message/);

	GeneralException(const GeneralException& /exception/);
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
  IndexUnderflow(const char* /file/, int /line/, Index /index/ = 0, Size /size/ = 0);
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
  SizeUnderflow(const char* /file/, int /line/, Size /size/ = 0);
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
	IndexOverflow(const char* /file/, int /line/, Index /index/ = 0, Size /size/ = 0);
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
	InvalidRange(const char* /file/, int /line/, float);
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
	InvalidFormat(const char* /file/, int /line/, const String& /format/);
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
	IllegalSelfOperation(const char* /file/, int /line/);
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
	NullPointer(const char* /file/, int /line/);
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
	InvalidIterator(const char* /file/, int /line/);
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
	IncompatibleIterators(const char* /file/, int /line/);
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
	NotImplemented(const char* /file/, int /line/);
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
	TooManyErrors(const char* /file/, int /line/);
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
	OutOfMemory(const char* /file/, int /line/, Size /size/ = 0);
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
	OutOfGrid(const char* /file/, int /line/)
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
	BufferOverflow(const char* /file/, int /line/);
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
	DivisionByZero(const char* /file/, int /line/);
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
  FileNotFound(const char* /file/, int /line/, const String& /filename/);
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
	ParseError(const char* /file/, int /line/, const String& /expression/,
 	  				 const String& /message/)
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
  TooManyBonds(const char* /file/, int /line/, const String /error/) throw();
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
	static void setName(const String& /name/);
	static void setMessage(const String& /message/);
	static void setLine(int /line/);
	static void setFile(const String& /file/);
	static void set
		(const String& /file/, int /line/, 
		 const String& /name/, const String& /message/);
};	

GlobalExceptionHandler globalHandler;

