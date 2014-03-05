// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

%Exception GeneralException(SIP_Exception) /PyName=GeneralException, Default/
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_GeneralException, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
};

%Exception IndexUnderflow(GeneralException) /PyName=IndexUnderflow/
{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_IndexUnderflow, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
};

%Exception SizeUnderflow(GeneralException) /PyName=SizeUnderflow/
{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_SizeUnderflow, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
};

%Exception IndexOverflow(GeneralException) /PyName=IndexOverflow/
{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_IndexOverflow, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
};

%Exception InvalidRange(GeneralException) /PyName=InvalidRange/
{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_InvalidRange, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
};

%Exception InvalidFormat(GeneralException) /PyName=InvalidFormat/
{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_InvalidFormat, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
};

%Exception IllegalSelfOperation(GeneralException) /PyName=IllegalSelfOperation/
{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_IllegalSelfOperation, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
};

%Exception NullPointer(GeneralException) /PyName=NullPointer/
{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_NullPointer, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
};

%Exception InvalidIterator(GeneralException) /PyName=InvalidIterator/
{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_InvalidIterator, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
};

%Exception IncompatibleIterators(GeneralException) /PyName=IncompatibleIterators/
{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_IncompatibleIterators, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
};

%Exception NotImplemented(GeneralException) /PyName=NotImplemented/
{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_NotImplemented, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
};

%Exception InvalidOption(GeneralException) /PyName=InvalidOption/
{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_InvalidOption, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
};

%Exception TooManyErrors(GeneralException) /PyName=TooManyErrors/
{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_TooManyErrors, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
};

%Exception OutOfMemory(GeneralException) /PyName=OutOfMemory/
{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_OutOfMemory, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
};

%Exception OutOfGrid(GeneralException) /PyName=OutOfGrid/
{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_OutOfGrid, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
};
  
%Exception BufferOverflow(GeneralException) /PyName=BufferOverflow/
{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_BufferOverflow, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
};

%Exception DivisionByZero(GeneralException) /PyName=DivisionByZero/
{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_DivisionByZero, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
};

%Exception FileNotFound(GeneralException) /PyName=FileNotFound/
{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_FileNotFound, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
};

%Exception ParseError(GeneralException) /PyName=ParseError/
{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_ParseError, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
};

%Exception TooManyBonds(GeneralException) /PyName=TooManyBonds/
{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_TooManyBonds, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
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

