// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: exception.sip,v 1.4 2003/10/29 22:08:39 oliver Exp $
//

class GeneralException 
{
%TypeHeaderCode
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
};  


class IndexUnderflow 
 	: GeneralException
{
%TypeHeaderCode
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  IndexUnderflow(const char* /file/, int /line/, Index /index/ = 0, Size /size/ = 0);
};
class SizeUnderflow 
  : GeneralException
{
%TypeHeaderCode
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  SizeUnderflow(const char* /file/, int /line/, Size /size/ = 0);
};
class IndexOverflow 
  : GeneralException
{
%TypeHeaderCode
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	IndexOverflow(const char* /file/, int /line/, Index /index/ = 0, Size /size/ = 0);
};

class InvalidRange 
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	InvalidRange(const char* /file/, int /line/);
};

class InvalidFormat 
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	InvalidFormat(const char* /file/, int /line/, const String& /format/);
};

class IllegalSelfOperation
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	IllegalSelfOperation(const char* /file/, int /line/);
};

class NullPointer 
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	NullPointer(const char* /file/, int /line/);
};

class InvalidIterator
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	InvalidIterator(const char* /file/, int /line/);
};

class IncompatibleIterators
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	IncompatibleIterators(const char* /file/, int /line/);
};

class NotImplemented
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	NotImplemented(const char* /file/, int /line/);
};

class OutOfMemory
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	OutOfMemory(const char* /file/, int /line/, Size /size/ = 0);
	~OutOfMemory() throw();
};

class OutOfGrid
  : GeneralException
{
%TypeHeaderCode
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	OutOfGrid(const char* /file/, int /line/)
    throw();
};

  
class BufferOverflow 
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	BufferOverflow(const char* /file/, int /line/);
};

class DivisionByZero 
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	DivisionByZero(const char* /file/, int /line/);
};

class FileNotFound 
 : GeneralException
{
%TypeHeaderCode
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  FileNotFound(const char* /file/, int /line/, const String& /filename/);
  String getFilename() const;
};

class ParseError
	: GeneralException
{
%TypeHeaderCode
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	ParseError(const char* /file/, int /line/, const String& /expression/,
 	  				 const String& /message/)
							throw();
};



class GlobalExceptionHandler
{
%TypeHeaderCode
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
