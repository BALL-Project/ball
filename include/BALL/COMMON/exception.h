// $Id: exception.h,v 1.16 2000/10/03 02:01:13 amoll Exp $
   
#ifndef BALL_COMMON_EXCEPTION_H
#define BALL_COMMON_EXCEPTION_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#ifndef BALL_COMMON_GLOBAL_H
#	include <BALL/COMMON/global.h>
#endif

#include <string>
#include <new>

namespace BALL 
{

	namespace Exception 
	{

		using std::string;

		/**	@name	Exception Handling
		*/
		//@{

		
		/**	General exception class.
				This class is intended as a base class for all other exceptions.
				Each exception class should define a constructor taking a string
				and an int as parameters. These two values are interpreted as the current
				filename and line number and is usually printed in case of an uncaught exception.
				To support this feature, each {\bf throw} directive should look as follows:
				{\tt {\bf throw Exception::GeneralException}(\_\_FILE\_\_, \_\_LINE\_\_);}\\
				{\tt \_\_FILE\_\_} and {\tt \_\_LINE\_\_} are built-in preprocessor macros that hold the
				desired information.\\

				BALL provides its own \Ref{terminate} handler. This handler extracts as much information
				as possible from the exception, prints it to {\tt cerr} and \Ref{Log}, and finally calls
				exits the program cleanly (with exit code 1).
				This can be rather inconvenient for debugging, since you are told where the exception was 
				thrown, but in general you do not know anything about the context.
				Therefore {\tt terminate} can also create a core dump. Using a debugger (e.g. dbx or gdb)
				you can then create a stack traceback.
				To create a core dump, you should set the environment variable {\tt BALL_DUMP_CORE}
				to any (non empty) value.\\
				{\bf Definition:}\URL{BALL/COMMON/exception.h}
				\\
		*/
		
		class GeneralException 
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			/// Default constructor
			GeneralException();
			
			/// Constructor
			GeneralException
				(const char* file, int line, 
				 const string& name = "GeneralException",
				 const string& message = "unspecified error");

			/// Copy constructor
			GeneralException(const GeneralException& exception);

			/// Destructor
			virtual ~GeneralException();
			//@}

			/**	@name	Accessors
			*/
			//@{
	
			///	Returns the name of the exception 
			string getName() const;

			///	Returns the error message of the exception
			string getMessage() const;

			/// Returns the line number where it occured
			int getLine() const;
	
			/// Returns the file where it occured
			string getFile() const;
			//@}

			protected:
			string	file_;
			int			line_;

			string 	name_;
			string 	message_;
		};		
	
	
		/**	Index underflow.
				Throw this exception to indicate an index that was smaller than allowed.
				The constructor has two additional arguments, the values of which should be 
				set to the index that caused the failure and the smallest allowed value to 
				simplify debugging.
				@param	index the value of the index causing the problem
				@param	size	smallest value allowed for index
		*/
		class IndexUnderflow 
			: public GeneralException
		{
			public:

			IndexUnderflow(const char* file, int line, Index index = 0, Size size = 0);


			protected:

			Size size_;
			Index index_;
		};

		/**	Size underflow.
				Throw this exception to indicate a size was smaller than allowed.
				The constructor has an additional argument: the value of of the requested size.
				This exception is thrown, if buffer sizes are insufficient.
				@param	size the size causing the problem
		*/
		class SizeUnderflow 
			: public GeneralException
		{
			public:

			SizeUnderflow(const char* file, int line, Size size = 0);

			protected:
			Size size_;
		};

		/**	Index overflow.
				Throw this exception to indicate an index that was larger than allowed.
				The constructor has two additional arguments, the values of which should be 
				set to the index that caused the failure and the largest allowed value to 
				simplify debugging.
				@param	index the value of the index causing the problem
				@param	size	largest value allowed for index
		*/
		class IndexOverflow 
			: public GeneralException
		{
			public:
			IndexOverflow(const char* file, int line, Index index = 0, Size size = 0);

			protected:
			Size size_;
			Index index_;
		};

		/**	Invalid range.
				Use this exception to indicate a general range problems.
		*/
		class InvalidRange 
			: public GeneralException
		{
			public:
			InvalidRange(const char* file, int line);
		};

		/**	Invalid format.
				This exception indicates a conversion problem when converting from
				one type to another. It is thrown, if a conversion from ascii to numeric 
				formats or vice versa failed.
		*/
		class InvalidFormat 
			: public GeneralException
		{
			public:
			InvalidFormat(const char* file, int line, const string& s = "");
			
			protected:
			string format_;
		};

		/**	Illegal self operation.	
				Throw this excpetion to indicate an invalid operation on the object itself. In general
				these operations are self assignments or related methods.
		*/
		class IllegalSelfOperation
			: public GeneralException
		{
			public:
			IllegalSelfOperation(const char* file, int line);
		};

		/**	Null pointer argument is invalid.
				Use this exception to indicate a failure due to an argument not containing
				a pointer to a valid object, but a null pointer.
		*/
		class NullPointer 
			: public GeneralException
		{
			public:
			NullPointer(const char* file, int line);
		};

		/**	Invalid iterator.
				The iterator on which an operation should be performed was invalid.
		*/
		class InvalidIterator
			: public GeneralException
		{
			public:
			InvalidIterator(const char* file, int line);
		};

		/**	Incompatible iterator.
				The iterators could not be assigned because they are bound to different containers.
		*/
		class IncompatibleIterators
			: public GeneralException
		{
			public:
			IncompatibleIterators(const char* file, int line);
		};

		/**	Not implemented exception. 
				This exception should be thrown to indicate not yet inplemented methods.
				If you take the time to use the detailed constructor instead of the default constructor,
				identification of the concerned source will get {\em much} easier!
		*/
		class NotImplemented
			: public GeneralException
		{
			public:
			NotImplemented(const char* file, int line);
		};

		/** Illegal tree operation.
				This exception is thrown to indicate that an illegal tree operation
				i.e. node->setLeftChild(node) was requested.
		*/
		class IllegalTreeOperation
			: public GeneralException
		{
			public:
			IllegalTreeOperation(const char* file, int line);
		};

		/**	Out of memory.
				Throw this exception to indicate that an allocation failed.
				This exception is thrown in the BALL new handler.
				@param	size	the number of bytes that should have been allocated
				@see GlobalException::newHandler
		*/
		class OutOfMemory
			: public GeneralException, public std::bad_alloc
		{
			public:
			OutOfMemory(const char* file, int line, Size size = 0);
			
			virtual ~OutOfMemory() throw();

			protected:
			Size size_;
		};

		/**	Buffer overflow exception.	
		*/
		class BufferOverflow 
			: public GeneralException
		{
			public:
			BufferOverflow(const char* file, int line);
		};

		/**	Division by zero error.
		*/
		class DivisionByZero 
			: public GeneralException
		{
			public:
			DivisionByZero(const char* file, int line);
		};

		/**	Out of grid error.
		*/
		class OutOfGrid 
			: public GeneralException
		{
			public:
			OutOfGrid(const char* file, int line);
		};

		/**	File not found.
				A given file could not be found.
		*/
		class FileNotFound 
			: public GeneralException
		{
			public:
			FileNotFound(const char* file, int line, const string& filename);

			string getFilename() const;

			protected:
			string filename_;
		};

		/** Exception thrown if a file could not be processed right.
		*/
		class ReadFileError
			: public GeneralException
		{
			friend class ReadFile;

			public:
			ReadFileError(const string& message = "", const string& line = "", int line_nr = -1);
		};

		/**
		*/
		class GlobalExceptionHandler
		{
			public:
			/**	@name	Constructors
			*/
			//@{

			/**	Default constructor.
					This constructor installs the BALL specific handlers for
					{\tt terminate}, {\tt unexpected}, and {\tt new_handler}.
					{\tt terminate} or {\tt unexpected} are called to abort 
					a program if an exception was not caught or a function 
					exits via an exception that is not allowed by its exception
					specification. Both functions are replaced by a function of 
					GlobalExceptionHandler that tries to determine the 
					last exception thrown. This mechanism only works, if all 
					exceptions are defrived from \Ref{GeneralException}.\\

					The default {\tt new_handler} is replaced by \Ref{newHandler}
					and throws an exception of type \Ref{OutOfMemory} instead of 
					{\tt bad_alloc} (the default behaviour defined in the ANSI C++ 
					standard).
			*/
			GlobalExceptionHandler();
			//@}
			
			/**	@name	Accessors
			*/
			//@{
				
			/**
			*/
			static void setName(const string& name);
				
			/**
			*/
			static void setMessage(const string& message);

			/**
			*/
			static void setLine(int line);

			/**
			*/
			static void setFile(const string& file);

			/**
			*/
			static void set
				(const string& file, int line, 
				 const string& name, const string& message);
			//@}	
			
			protected:

			/// The BALL replacement for terminate
			static void terminate();

			/// The BALL new handler
			static void newHandler();

			static string file_;
			static int		line_;
			static string name_;
			static string message_;
		};

		/**	Global static instance of GlobalExceptionHandler
		*/
		extern GlobalExceptionHandler globalHandler;

		//@}
	}
}

#endif // BALL_COMMON_EXCEPTION_H
