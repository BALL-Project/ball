// $Id: lineBasedFile.h,v 1.18 2001/12/20 02:35:32 oliver Exp $

#ifndef BALL_FORMAT_LINEBASEDFILE_H
#define BALL_FORMAT_LINEBASEDFILE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
# include <BALL/SYSTEM/file.h>
#endif

#include <vector>
#include <fstream>

namespace BALL 
{

	/** A class for the convenient parsing of line-based file formats.
			{\bf Definition:} \URL{BALL/FORMAT/lineBasedFile.h} \\
	*/
	class LineBasedFile
		:	public File
	{
		public:

		BALL_CREATE(LineBasedFile)

		/**	@name Constructors and Destructors
		*/
		//@{

		/// Default constructor
		LineBasedFile()
			throw();
			
		/** Detailed constuctor.
				Open the given file.
		*/
		LineBasedFile(const String& filename, File::OpenMode open_mode = File::IN)
			throw(Exception::FileNotFound);

		/** Copy constructor
				The file is opened and the same position in it is seeked.
		*/
		LineBasedFile(const LineBasedFile& f)
			throw(Exception::FileNotFound);


		/**	Clear method.
		*/
		void clear() 
			throw();

		//@}


		/**	@name Equality operators
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const LineBasedFile& f)  throw();

		/** Inequality operator
		*/
		bool operator != (const LineBasedFile& f)  throw();

		//@}
		/**	@name Assignment
		*/
		//@{
			
		/** Assignment operator.
				The file is opened and the same position in it is seeked.
		*/
		const LineBasedFile& operator = (const LineBasedFile& file)
			throw();
		//@}

		/**	@name Accessors
		*/
		//@{

		/// Get the last line number in the file.
		Position getLineNumber() 
			const	throw();

		/// Return the current line
		const String& getLine() 
			const throw();

		/// Return the current line
		String& getLine() 
			throw();
		//@}


		/**	@name	Help-Methods for File Acces
		*/
		//@{

		/** Reads a line and counts the line number.
				@return true if a line could be read, false if End Of File.
		*/
		bool readLine()
			throw(Exception::ParseError);

		/** Skip a given number of lines.
				@return false, if EOF occurs.
		*/
		bool skipLines(Size number = 1)
			throw(Exception::ParseError);

		/** Search for a line starting with a given string.
				Search starts at the current line and ends at the end of the file 
				(no wrap around).
				@param return_to_start if set to {\bf true}, the current line is reset to its value prior to the invocation
				@return {\bf true} if line could be found
		*/
		bool search(const String& text, bool return_to_start = false)
			throw(Exception::ParseError);

		/* Search for a line starting with a given string, abort at a stop tag.
		*/
		bool search(const String& text, const String& stop, 
								bool return_to_start = false)
			throw(Exception::ParseError);

		/** Go to a given line.
				@return false if EOF occurs
		*/
		bool gotoLine(Position line_number)
			throw(Exception::ParseError);

		/** Rewind file to start
		*/
		void rewind()
			throw(Exception::ParseError);

		/** Test for a condition.
				Throw an exception if a given condition is not met.
				\begin{verbatim}
					abort(__FILE__, __LINE__, shift_reference->elements.size() > 0,
								"no data for shift references found");
				\end{verbatim}
				@param file should be used for __FILE__
				@param line should be used for __LINE__
				@param condition to be tested
				@param msg this string is used as message in the exception
				@exception ParseError if {\tt condition} is not fulfilled
		*/
		void test(const char* file, int line, bool condition, const String& msg) 
			const throw(Exception::ParseError);

		/** Function to get a field surrounded by delimiter
		*/
		String getField(Index pos = 0, const String& quotes = "",
										const String& delimiters = String::CHARACTER_CLASS__WHITESPACE)
			const	throw(Exception::IndexUnderflow);

		/// Test if the current line starts with text
		bool startsWith(const String& text) 
			const throw();

		/// Return true if the current line contains text
		bool has(const String& text) 
			const throw();

		/** Switch method of the current line.
				Return the position of the current line in data or -1 if it does not exist.
		*/
		Index switchString(const std::vector<String>& data) 
			const throw();

		/**	Parse column based formats.
				Copy the subsection of the current line defined by {\tt index} and {\tt length} into a buffer
				try to parse it using {\tt sscanf}. The result is stored in {\tt arg} (use with caution: no type checking!).
		*/
		bool parseColumnFormat(const char* format, Position index, Size length, void* arg);

		//@}

		/*	@name	Protected Attributes
		*/
		//_@{

		/// buffer for the line in use
		String line_;

		/// line number in the file
		Position line_number_;
		//_@}
	};


# ifndef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/FORMAT/lineBasedFile.iC>
# endif

} // namespace BALL

#endif // BALL_FORMAT_LINEBASEDFILE_H
