// $Id: exception.C,v 1.29 2001/08/01 01:22:02 oliver Exp $

#include <BALL/COMMON/exception.h>
#include <BALL/COMMON/logStream.h>
#include <BALL/DATATYPE/string.h>

#include <iostream>
#include <typeinfo>
#include <exception>
#include <stdio.h>
#include <stdlib.h>	// for getenv in terminate()
#include <sys/types.h>
#include <signal.h> // for SIGSEGV and kill
#include <unistd.h> // fot getpid

#define BALL_CORE_DUMP_ENVNAME "BALL_DUMP_CORE"

#define DEF_EXCEPTION(a,b) \
	a :: a (const char* file, int line) throw()\
		: GeneralException(file, line, #a, b)\
	{\
	}\
	

using std::string;
using std::endl;

namespace BALL 
{

	namespace Exception 
	{

			GeneralException::GeneralException() 
				throw()
				:	file_("?"),
					line_(-1),
					name_("GeneralException"),
					message_("unspecified error")
			{
				globalHandler.set(file_, line_, String(name_), String(message_));
			}

			GeneralException::GeneralException(const char* file, int line, const String& name, const String& message) 
				throw()
				:	file_(file),
					line_(line),
					name_(name),
					message_(message)
			{
				globalHandler.set(file_, line_, name_, message_);
			}

			GeneralException::GeneralException(const char* file, int line) 
				throw()
				:	file_(file),
					line_(line),
					name_("GeneralException"),
					message_("unknown error")
			{
				globalHandler.set(file_, line_, name_, message_);
			}

			GeneralException::GeneralException(const GeneralException& exception)
				throw()
				:	std::exception(exception),
					file_(exception.file_),
					line_(exception.line_),
					name_(exception.name_),
					message_(exception.message_)
			{
			}

			GeneralException::~GeneralException()
				throw()
			{
			}
		

			const char* GeneralException::getName() const
				throw()
			{
				return name_.c_str();
			}

			const char* GeneralException::getMessage() const
				throw()
			{
				return message_.c_str();
			}

			const char* GeneralException::getFile() const
				throw()
			{
				return file_;
			}
			
			int GeneralException::getLine() const
				throw()
			{
				return line_;
			}

			IndexUnderflow::IndexUnderflow(const char* file, int line, Index index, Size size)
				throw()
				: GeneralException(file, line, "IndexUnderflow", ""),
					size_(size),
					index_(index)
			{
				message_ = "the given index was too small: ";
				char buf[40];

				sprintf(buf, "%ld", (long)index);
				message_ += buf;
				message_ += " (size = ";

				sprintf(buf, "%ld", (long)size);
				message_ += buf;
				message_ += ")";

				globalHandler.setMessage(message_);
			}

			IndexOverflow::IndexOverflow(const char* file, int line, Index index, Size size)
				throw()
				:	GeneralException(file, line, "IndexOverflow", "an index was too large"),
					size_(size),
					index_(index)
			{
				message_ = "the given index was too large: ";
				char buf[40];

				sprintf(buf, "%ld", (long)index);
				message_ += buf;
				message_ += " (size = ";

				sprintf(buf, "%ld", (long)size);
				message_ += buf;
				message_ += ")";

				globalHandler.setMessage(message_);
			}

			OutOfMemory::OutOfMemory(const char* file, int line, Size size)
				throw()
				:	GeneralException(file, line, "OutOfMemory", "a memory allocation failed"),
					size_(size)
			{
				message_ = "unable to allocate enough memory (size = ";
				char buf[40];

				sprintf(buf, "%ld", (long)size_);
				message_ += buf;
				message_ += " bytes) ";

				globalHandler.setMessage(message_);
			}

			OutOfMemory::~OutOfMemory() 
				throw()
			{
			}

			SizeUnderflow::SizeUnderflow(const char* file, int line, Size size)
				throw()
				:	GeneralException(file, line, "SizeUnderflow", ""),
					size_(size)
			{
				message_ = "the given size was too small: ";
				char buf[40];
				sprintf(buf, "%ld", (long)size);
				
				message_ += buf;
				globalHandler.setMessage(message_);
			}

			IllegalPosition::IllegalPosition(const char* file, int line, float x, float y, float z)
				throw()
				:	GeneralException(file, line, "IllegalPosition:", "")
			{
				char buf1[40];
				sprintf(buf1, "%f", x);
				char buf2[40];
				sprintf(buf2, "%f", y);
				char buf3[40];
				sprintf(buf3, "%f", z);

				message_ += "(";
				message_ += buf1;
				message_ += ",";
				message_ += buf2;
				message_ += ",";
				message_ += buf3;
				message_ += ")";
				globalHandler.setMessage(message_);
			}

			ParseError::ParseError(const char* file, int line, 
					const String& expression, const String& message)
				throw()
				: GeneralException(file, line, "Parse Error", "")
			{

				message_ += message;
				message_ += " in ";
				message_ += expression;
				globalHandler.setMessage(message_);
			}

			FileNotFound::FileNotFound(const char* file, int line, const String& filename)
				throw()
				:	GeneralException(file, line, "FileNotFound", ""),
					filename_(filename)
			{
				message_ = "the file " + filename + " could not be found";
				globalHandler.setMessage(message_);
			}

			FileNotFound::~FileNotFound()
				throw()
			{
			}

			String FileNotFound::getFilename() const
				throw()
			{
				return filename_;
			}

			InvalidFormat::InvalidFormat(const char* file, int line, const String& s)
				throw()
				:	GeneralException(file, line, "InvalidFormat", ""),
					format_(s)
			{
				message_ = "problem converting '";
				message_.append(s + "' to a number.");

				globalHandler.setMessage(message_);
			}

			InvalidFormat::~InvalidFormat()
				throw()
			{
			}
		
			DEF_EXCEPTION(DivisionByZero, "a division by zero was requested")

			DEF_EXCEPTION(InvalidRange, "the range of the operation was invalid")

			DEF_EXCEPTION(OutOfRange, "the argument was not in range")

			DEF_EXCEPTION(NullPointer, "a null pointer was specified")

			DEF_EXCEPTION(InvalidIterator, "the iterator is invalid - probably it is not bound to a container")

			DEF_EXCEPTION(IncompatibleIterators, "the iterator could not be assigned because it is bound to a different container")

			DEF_EXCEPTION(NotImplemented, "this method has not been implemented yet. Feel free to complain about it!")

			DEF_EXCEPTION(IllegalSelfOperation, "cannot perform operation on the same object")

			DEF_EXCEPTION(IllegalTreeOperation, "an illegal tree operation was requested")

			DEF_EXCEPTION(BufferOverflow, "the maximum buffersize has been reached")

			DEF_EXCEPTION(OutOfGrid, "a point was outside a grid")

		
			GlobalExceptionHandler::GlobalExceptionHandler()
				throw()
			{
				std::set_terminate(terminate);
				std::set_unexpected(terminate);
				std::set_new_handler(newHandler);
			}

			void GlobalExceptionHandler::newHandler()
				throw()
			{
				throw Exception::OutOfMemory(__FILE__, __LINE__);
			}
				
			void GlobalExceptionHandler::terminate()
				throw()
			{
				// add cerr to the log stream
				// and write all available information on
				// the exception to the log stream (potentially with an assigned file!)
				// and cerr

				Log.insert(std::cerr);
				Log.error() << endl;
				Log.error() << "---------------------------------------------------" << endl;
				Log.error() << "FATAL: terminate called!" << endl;
				Log.error() << "---------------------------------------------------" << endl;
				if ((line_ != -1) && (name_ != "unknown"))
				{
					Log.error() << "last entry in the exception handler: " << endl;
					Log.error() << "exception of type " << name_.c_str() << " occured in line " 
											<< line_ << " of " << file_.c_str() << endl;
					Log.error() << "error message: " << message_.c_str() << endl;
				}
				Log.error() << "---------------------------------------------------" << endl;

				// if the environment variable declared in BALL_CORE_DUMP_ENVNAME
				// is set, provoke a core dump (this is helpful to get s stack traceback)
				if (getenv(BALL_CORE_DUMP_ENVNAME) != 0)
				{
					Log.error() << "dumping core file.... (to avoid this, unset " << BALL_CORE_DUMP_ENVNAME 
											<< " in your environment)" << endl;
					// provoke a core dump 
					kill(getpid(), SIGSEGV);
				}

				// otherwise exit cleanly
				exit(1);
			}
				
			void GlobalExceptionHandler::set
				(const String& file, int line,
				 const String& name, const String& message)
				throw()
			{
				name_ = name;
				line_ = line;
				message_ = message;
				file_ = file;
			}
			
			void GlobalExceptionHandler::setName(const String& name)
				throw()
			{
				name_ = name;
			}
			
			void GlobalExceptionHandler::setMessage(const String& message)
				throw()
			{
				message_ = message;
			}
			
			void GlobalExceptionHandler::setFile(const String& file)
				throw()
			{
				file_ = file;
			}
			
			void GlobalExceptionHandler::setLine(int line) 
				throw()
			{
				line_ = line;
			}

			std::string	GlobalExceptionHandler::name_			= "unknown exception";
			int					GlobalExceptionHandler::line_			= -1;
			std::string	GlobalExceptionHandler::message_	= " - ";
			std::string	GlobalExceptionHandler::file_			= "unknown";



			// create a global instance of the exception handler
			GlobalExceptionHandler globalHandler;

	} // namespace Exception

	std::ostream& operator << (std::ostream& os, const Exception::GeneralException& e)
	{
		os << e.getName() << " @ " << e.getFile() << ":" << e.getLine() << ": " << e.getMessage();

		return os;
	}


} // namespace BALL
