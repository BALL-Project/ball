// $Id: exception.h,v 1.28 2001/07/29 17:24:00 oliver Exp $
   
#ifndef BALL_COMMON_EXCEPTION_H
#define BALL_COMMON_EXCEPTION_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#ifndef BALL_COMMON_GLOBAL_H
#	include <BALL/COMMON/global.h>
#endif

#include <new>
#include <string>

namespace BALL 
{

	class String;

	namespace Exception 
	{


		/**	@name	Exception Handling
		*/
		//@{

		
		/**	General exception class.
				This class is intended as a base class for all other exceptions.
				Each exception class should define a constructor taking a string
				and an int as parameters. These two values are interpreted as the
				current filename and line number and is usually printed in case of
				an uncaught exception.  To support this feature, each {\bf throw}
				directive should look as follows: \\
				{\tt {\bf throw Exception::GeneralException}(\_\_FILE\_\_, \_\_LINE\_\_);}\\
				{\tt \_\_FILE\_\_} and {\tt \_\_LINE\_\_} are built-in preprocessor
				macros that hold the desired information.
				\\
				BALL provides its own \Ref{terminate} handler. This handler
				extracts as much information as possible from the exception, prints
				it to {\tt cerr} and \Ref{Log}, and finally calls exits the program
				cleanly (with exit code 1).  This can be rather inconvenient for
				debugging, since you are told where the exception was thrown, but
				in general you do not know anything about the context.  Therefore
				{\tt terminate} can also create a core dump. Using a debugger (e.g.
				dbx or gdb) you can then create a stack traceback.  To create a
				core dump, you should set the environment variable {\tt
				BALL_DUMP_CORE} to any (non empty) value.
				\\
				{\bf Definition:}\URL{BALL/COMMON/exception.h}
				\\
		*/
		
		class GeneralException 
			:	std::exception
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			/// Default constructor
			GeneralException()
				throw();
			
			/// Constructor
			GeneralException(const char* file, int line)
				throw();

			/// Constructor
			GeneralException
				(const char* file, int line,
				 const String& name , const String& message)
				throw();

			/// Copy constructor
			GeneralException(const GeneralException& exception)
				throw();

			/// Destructor
			virtual ~GeneralException() throw();
			//@}

			/**	@name	Accessors
			*/
			//@{
	
			///	Returns the name of the exception 
			const char* getName() const
				throw();

			///	Returns the error message of the exception
			const char* getMessage() const
				throw();

			/// Returns the line number where it occured
			int getLine() const
				throw();
	
			/// Returns the file where it occured
			const char* getFile() const
				throw();
			//@}

			protected:
			const char*	file_;
			int					line_;

			std::string name_;
			std::string message_;
		};		

		/**	Index underflow.
				Throw this exception to indicate an index that was smaller than
				allowed.  The constructor has two additional arguments, the values
				of which should be set to the index that caused the failure and the
				smallest allowed value to simplify debugging.
				@param	index the value of the index causing the problem
				@param	size	smallest value allowed for index
		*/
		class IndexUnderflow 
			: public GeneralException
		{
			public:

			IndexUnderflow(const char* file, int line, Index index = 0, Size size = 0)
				throw();


			protected:

			Size size_;
			Index index_;
		};

		/**	Size underflow.
				Throw this exception to indicate a size was smaller than allowed.
				The constructor has an additional argument: the value of of the
				requested size.  This exception is thrown, if buffer sizes are
				insufficient.
				@param	size the size causing the problem
		*/
		class SizeUnderflow 
			: public GeneralException
		{
			public:

			SizeUnderflow(const char* file, int line, Size size = 0)
				throw();

			protected:
			Size size_;
		};

		/**	Index overflow.
				Throw this exception to indicate an index that was larger than
				allowed.  The constructor has two additional arguments, the values
				of which should be set to the index that caused the failure and the
				largest allowed value to simplify debugging.
				@param	index the value of the index causing the problem
				@param	size	largest value allowed for index
		*/
		class IndexOverflow 
			: public GeneralException
		{
			public:
			IndexOverflow(const char* file, int line, Index index = 0, Size size = 0)
				throw();

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
			InvalidRange(const char* file, int line)
				throw();
		};


		/**	Out of range.
				Use this exception to indicate that a given value is out of a
				defined range, i. e. not within the domain of a function.
		*/
		class OutOfRange 
			: public GeneralException
		{
			public:
			OutOfRange(const char* file, int line)
				throw();
		};

		/**	Invalid format.
				This exception indicates a conversion problem when converting from
				one type to another. It is thrown, if a conversion from ascii to
				numeric formats or vice versa failed.
		*/
		class InvalidFormat 
			: public GeneralException
		{
			public:
			InvalidFormat(const char* file, int line, const String& s)
				throw();
			
			~InvalidFormat()
				throw();

			protected:

			std::string format_;
		};

		/**	Illegal self operation.	
				Throw this excpetion to indicate an invalid operation on the object
				itself. In general these operations are self assignments or related
				methods.
		*/
		class IllegalSelfOperation
			: public GeneralException
		{
			public:
			IllegalSelfOperation(const char* file, int line)
				throw();
		};

		/**	Null pointer argument is invalid.
				Use this exception to indicate a failure due to an argument not
				containing a pointer to a valid object, but a null pointer.
		*/
		class NullPointer 
			: public GeneralException
		{
			public:
			NullPointer(const char* file, int line)
				throw();
		};

		/**	Invalid iterator.
				The iterator on which an operation should be performed was invalid.
		*/
		class InvalidIterator
			: public GeneralException
		{
			public:
			InvalidIterator(const char* file, int line)
				throw();
		};

		/**	Incompatible iterator.
				The iterators could not be assigned because they are bound to
				different containers.
		*/
		class IncompatibleIterators
			: public GeneralException
		{
			public:
			IncompatibleIterators(const char* file, int line)
				throw();
		};

		/**	Not implemented exception. 
				This exception should be thrown to indicate not yet inplemented
				methods.  If you take the time to use the detailed constructor
				instead of the default constructor, identification of the concerned
				source will get {\em much} easier!
		*/
		class NotImplemented
			: public GeneralException
		{
			public:
			NotImplemented(const char* file, int line)
				throw();
		};

		/** Illegal tree operation.
				This exception is thrown to indicate that an illegal tree operation
				i.e. node->setLeftChild(node) was requested.
		*/
		class IllegalTreeOperation
			: public GeneralException
		{
			public:
			IllegalTreeOperation(const char* file, int line)
				throw();
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
			OutOfMemory(const char* file, int line, Size size = 0)
				throw();
			
			virtual ~OutOfMemory() 
				throw();

			protected:
			Size size_;
		};

		/**	Buffer overflow exception.	
		*/
		class BufferOverflow 
			: public GeneralException
		{
			public:
			BufferOverflow(const char* file, int line)
				throw();
		};

		/**	Division by zero error.
		*/
		class DivisionByZero 
			: public GeneralException
		{
			public:
			DivisionByZero(const char* file, int line)
				throw();
		};

		/**	Out of grid error.
		*/
		class OutOfGrid 
			: public GeneralException
		{
			public:
			OutOfGrid(const char* file, int line)
				throw();
		};

		/**	File not found.
				A given file could not be found.
		*/
		class FileNotFound 
			: public GeneralException
		{
			public:
			FileNotFound(const char* file, int line, const String& filename)
				throw();

			~FileNotFound()
				throw();
			String getFilename() const
				throw();

			protected:
			std::string filename_;
		};

		/**	File not found.
				A given position in three dimensional is invalid.
		*/
		class IllegalPosition 
			: public GeneralException
		{
			public:
			IllegalPosition(const char* file, int line, float x, float y, float z)
				throw();
		};

		/**	Parse Error.
				A given expression could not be parsed.
		*/
		class ParseError
			: public GeneralException
		{
			public:
			ParseError(const char* file, int line, const String& expression,
					const String& message)
				throw();
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
			GlobalExceptionHandler()
				throw();
			//@}
			
			/**	@name	Accessors
			*/
			//@{
				
			/**
			*/
			static void setName(const String& name)
				throw();
				
			/**
			*/
			static void setMessage(const String& message)
				throw();

			/**
			*/
			static void setLine(int line)
				throw();

			/**
			*/
			static void setFile(const String& file)
				throw();

			/**
			*/
			static void set
				(const String& file, int line, 
				 const String& name, const String& message)
				throw();
			//@}	
			
			protected:

			/// The BALL replacement for terminate
			static void terminate()
				throw();

			/// The BALL new handler
			static void newHandler()
				throw();

			static std::string file_;
			static int				 line_;
			static std::string name_;
			static std::string message_;
		};

		/**	Global static instance of GlobalExceptionHandler
		*/
		extern GlobalExceptionHandler globalHandler;

		//@}
	}
		/**	Output operator for exceptions.
				All BALL exceptions can be printed to an arbitrary output stream.
				Information written contains the exception class, the error message,
        and the location (file, line number). The following code block
        can thus be used to catch any BALL exceptions and convert them to
        human readable information:
				\begin{verbatim}
				try
				{
					.... // some code which potentially throws an exception
				}
				catch (Exception::GeneralException e)
				{
					Log.error() << "caught exception: " << e << std::endl;
				}
				\end{verbatim}
		*/
		std::ostream& operator << (std::ostream& os, const Exception::GeneralException& e);
	
} // namespace BALL

#endif // BALL_COMMON_EXCEPTION_H
