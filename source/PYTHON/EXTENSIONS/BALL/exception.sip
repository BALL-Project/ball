%UnitCode
	#include <sstream>
	#include <BALL/COMMON/exception.h>

	using std::stringstream;
	using namespace BALL::Exception;
%End

%Exception GeneralException(SIP_Exception) /PyName=GeneralException/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_GeneralException, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception IndexUnderflow(GeneralException) /PyName=IndexUnderflow/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_IndexUnderflow, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception SizeUnderflow(GeneralException) /PyName=SizeUnderflow/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_SizeUnderflow, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception IndexOverflow(GeneralException) /PyName=IndexOverflow/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_IndexOverflow, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception InvalidRange(GeneralException) /PyName=InvalidRange/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_InvalidRange, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception InvalidFormat(GeneralException) /PyName=InvalidFormat/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_InvalidFormat, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception IllegalSelfOperation(GeneralException) /PyName=IllegalSelfOperation/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_IllegalSelfOperation, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception NullPointer(GeneralException) /PyName=NullPointer/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_NullPointer, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception InvalidIterator(GeneralException) /PyName=InvalidIterator/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_InvalidOperator, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception IncompatibleIterators(GeneralException) /PyName=IncompatibleIterators/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_IncompatibleIterators, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception NotImplemented(GeneralException) /PyName=NotImplemented/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_NotImplemented, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception InvalidOption(GeneralException) /PyName=InvalidOption/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_InvalidOption, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception TooManyErrors(GeneralException) /PyName=TooManyErrors/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_TooManyErrors, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception OutOfMemory(GeneralException) /PyName=OutOfMemory/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_OutOfMemory, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception OutOfGrid(GeneralException) /PyName=OutOfGrid/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_OutOfGrid, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};
  
%Exception BufferOverflow(GeneralException) /PyName=BufferOverflow/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_BufferOverflow, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception DivisionByZero(GeneralException) /PyName=DivisionByZero/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_DivisionByZero, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception FileNotFound(GeneralException) /PyName=FileNotFound/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_FileNotFound, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception ParseError(GeneralException) /PyName=ParseError/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_ParseError, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception TooManyBonds(GeneralException) /PyName=TooManyBonds/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_TooManyBonds, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception File::CannotWrite(GeneralException) /PyName=CannotWrite/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_File_CannotWrite, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception RMSDMinimizer::IncompatibleCoordinateSets(GeneralException) /PyName=RMSDMinimizer_IncompatibleCoordinateSets/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_RMSDMinimizer_IncompatibleCoordinateSets, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception RMSDMinimizer::TooFewCoordinates(GeneralException) /PyName=RMSDMinimizer_TooFewCoordinates/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_RMSDMinimizer_TooFewCoordinates, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

%Exception StringHashMapString::IllegalKey(GeneralException) /PyName=StringHashMapString_IllegalKey/
{
%RaiseCode
	stringstream detail;
	detail << sipExceptionRef.getName() << " in line "
	       << sipExceptionRef.getLine() << " of "
	       << sipExceptionRef.getFile() << ": "
	       << sipExceptionRef.getMessage();

	SIP_BLOCK_THREADS
	PyErr_SetString(sipException_StringHashMapString_IllegalKey, detail.str().c_str());
	SIP_UNBLOCK_THREADS
%End
};

/// creates a crash under windows:
class GlobalExceptionHandler
{
	public:
	GlobalExceptionHandler();
	static void setName(const String&);
	static void setMessage(const String&);
	static void setLine(int);
	static void setFile(const String&);
	static void set(const String&, int, const String&, const String&);
};

GlobalExceptionHandler globalHandler;
