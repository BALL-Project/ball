// $Id: exception.C,v 1.1 1999/08/26 08:02:33 oliver Exp $

#include <BALL/COMMON/exception.h>
#include <BALL/COMMON/logStream.h>

#include <iostream.h>
#include <typeinfo>
#include <exception>
#include <stdio.h>


#define DEF_EXCEPTION(a,b) \
	a##::##a##(const char* file, int line)\
		: GeneralException(file, line, #a, b)\
	{\
	}\
\
	

namespace BALL 
{

	namespace Exception 
	{

			GeneralException::GeneralException() 
				:	file_("?"),
					line_(-1),
					name_("GeneralException"),
					message_("unspecified error")
			{
				globalHandler.set(file_, line_, name_, message_);
			}

			GeneralException::GeneralException(const char* file, int line, const string& name, const string& message) 
				:	file_(file),
					line_(line),
					name_(name),
					message_(message)
			{
				globalHandler.set(file_, line_, name_, message_);
			}

			GeneralException::GeneralException(const GeneralException& exception)
				:	file_(exception.file_),
					line_(exception.line_),
					name_(exception.name_),
					message_(exception.message_)
			{
			}

			GeneralException::~GeneralException()
			{
			}
		

			string GeneralException::getName() const
			{
				return name_;
			}

			string GeneralException::getMessage() const
			{
				return message_;
			}

			string GeneralException::getFile() const
			{
				return file_;
			}
			
			int GeneralException::getLine() const
			{
				return line_;
			}

			IndexUnderflow::IndexUnderflow(const char* file, int line, int index, int size)
				: GeneralException(file, line, "IndexUnderflow", "an index was too small"),
					size_(size),
					index_(index)
			{
			}

			IndexOverflow::IndexOverflow(const char* file, int line, int index, int size)
				:	GeneralException(file, line, "IndexOverflow", "an index was too large"),
					size_(size),
					index_(index)
			{
			}

			SizeUnderflow::SizeUnderflow(const char* file, int line, int size)
				:	GeneralException(file, line, "SizeUnderflow", ""),
					size_(size)
			{
				message_ = "the given size was too small: ";
				char buf[20];
				sprintf(buf, "%d", size);
				message_ += buf;
				globalHandler.setMessage(message_);
			}

			FileNotFound::FileNotFound(const char* file, int line, const string& filename)
				:	GeneralException(file, line, "FileNotFound", ""),
					filename_(filename)
			{
				message_ = "the file " + filename + " could not be found";
				globalHandler.setMessage(message_);
			}

			DEF_EXCEPTION(DivisionByZero, "a division by zero was requested")

			DEF_EXCEPTION(InvalidRange, "the range of the operation was invalid")

			DEF_EXCEPTION(InvalidFormat, "a conversion from a string to a numeric value failed")

			DEF_EXCEPTION(IllegalSelfOperation, "this operation may not be performed with the same object as argument")

			DEF_EXCEPTION(NullPointer, "a null pointer was specified")

			DEF_EXCEPTION(InvalidIterator, "the iterator is invalid - probably it is not bound to a container")

			DEF_EXCEPTION(IncompatibleIterators, "the iterator could not be assigned because it is bound to a different container")

			DEF_EXCEPTION(NotImplemented, "this method has not been implemented yet. Feel free to complain about it!")

			DEF_EXCEPTION(BufferOverflow, "the maximum buffersize has been reached")

		
			GlobalExceptionHandler::GlobalExceptionHandler()
			{
				std::set_terminate(terminate);
				std::set_unexpected(terminate);
			}
				
			void GlobalExceptionHandler::terminate()
			{
				// add cerr to the log stream
				// and write all available information on
				// the exception to the log stream (potentially with an assigned file!)
				// and cerr

				Log.insert(cerr);
				Log.error() << endl;
				Log.error() << "---------------------------------------------------" << endl;
				Log.error() << "FATAL: terminate called!" << endl;
				Log.error() << "---------------------------------------------------" << endl;
				Log.error() << "last entry in the exception handler: " << endl;
				Log.error() << "exception of type " << name_ << " occured in line " 
					<< line_ << " of " << file_ << endl;
				Log.error() << "error message: " << message_ << endl;
				Log.error() << "---------------------------------------------------" << endl;
				exit(1);
			}
				
			void GlobalExceptionHandler::set
				(const string& file, int line,
				 const string& name, const string& message)
			{
				name_ = name;
				line_ = line;
				message_ = message;
				file_ = file;
			}
			
			void GlobalExceptionHandler::setName(const string& name)
			{
				name_ = name;
			}
			
			void GlobalExceptionHandler::setMessage(const string& message)
			{
				message_ = message;
			}
			
			void GlobalExceptionHandler::setFile(const string& file)
			{
				file_ = file;
			}
			
			void GlobalExceptionHandler::setLine(int line) 
			{
				line_ = line;
			}

			string	GlobalExceptionHandler::name_			= "unknown exception";
			int			GlobalExceptionHandler::line_			= -1;
			string	GlobalExceptionHandler::message_	= " - ";
			string	GlobalExceptionHandler::file_			= "unknown";



			// create a global instance of the exception handler
			GlobalExceptionHandler globalHandler;

	} // namespace Exception

} // namespace BALL
