// $Id: exception.h,v 1.4 1999/09/01 12:11:37 oliver Exp $
   
#ifndef BALL_COMMON_EXCEPTION_H
#define BALL_COMMON_EXCEPTION_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#ifndef BALL_COMMON_GLOBAL_H
#	include <BALL/COMMON/global.h>
#endif

#include <string>

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
				To support this feature, each {\bf throw} directive should lokk as follows:
				{\tt {\bf throw Exception::GeneralException}(\_\_FILE\_\_, \_\_LINE\_\_);}\\
				{\tt \_\_FILE\_\_} and {\tt \_\_LINE\_\_} are built-in preprocessor macros that hold the
				desired information.\\
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
			InvalidFormat(const char* file, int line);
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

		/**	File not found.
				A given file could not be found.
		*/
		class FileNotFound 
			: public GeneralException
		{
			public:
			FileNotFound(const char* file, int line, const string& filename);

			protected:
			string filename_;
		};
			

		/**
		*/
		class GlobalExceptionHandler
		{
			public:
			/**	@name	Constructors
			*/
			//@{

			/**
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

			static void terminate();

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
