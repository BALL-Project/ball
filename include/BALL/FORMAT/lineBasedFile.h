// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_LINEBASEDFILE_H
#define BALL_FORMAT_LINEBASEDFILE_H

#ifndef BALL_SYSTEM_FILE_H
# include <BALL/SYSTEM/file.h>
#endif

#include <vector>

namespace BALL 
{
	/** A class for the convenient parsing of line-based file formats.
			
    	\ingroup  General
	*/
	class BALL_EXPORT LineBasedFile
		:	public File
	{
		public:

		/**	@name Constructors and Destructors
		*/
		//@{

		/// Default constructor
		LineBasedFile();
			
		/** Detailed constuctor.
		 *	Open the given file.
		 *	@param trim_whitespaces - sets wheter leading and trailing whitespaces 
		 *				 shall be removed while reading the file
		 *  @throw Exception::FileNotFound if the file could not be opened
		 */
		LineBasedFile(const String& filename, File::OpenMode open_mode = std::ios::in, bool trim_whitespaces = false);

		/**	Clear method.
		*/
		void clear();

		//@}
		/**	@name Equality operators
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const LineBasedFile& f);

		/** Inequality operator
		*/
		bool operator != (const LineBasedFile& f);
		//@}

		/**	@name Assignment
		*/
		//@{
			
		/** Assignment operator.
		 *	The file is opened and the same position in it is seeked.
		 *  @throw Exception::FileNotFound if the file could not be opened
		 */
		const LineBasedFile& operator = (const LineBasedFile& file);

		//@}
		/**	@name Accessors
		*/
		//@{

		/// Get the last line number in the file.
		Position getLineNumber() const;

		/// Return the current line
		const String& getLine() const;

		/// Return the current line
		String& getLine();

		//@}
		/**	@name	Help-Methods for File Acces
		*/
		//@{

		/** Reads a line and counts the line number.
		 *	@return true if a line could be read, false if End Of File.
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		bool readLine();

		/** Skip a given number of lines.
		 *	@return false, if EOF occurs.
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		bool skipLines(Size number = 1);

		/** Search for a line starting with a given string.
		 *	Search starts at the current line and ends at the end of the file (no wrap around).
		 *	@param return_to_start if set to <b>true</b>, the current line is reset to its value prior to the invocation
		 *	@return true if line could be found
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		bool search(const String& text, bool return_to_start = false);

		/* Search for a line starting with a given string, abort at a stop tag.
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		bool search(const String& text, const String& stop, bool return_to_start = false);

		/** Go to a given line.
		 *	@return false if EOF occurs
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		bool gotoLine(Position line_number);

		/** Rewind file to start
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		void rewind();

		/** Test for a condition.
		 *	Throw an exception if a given condition is not met.
		 *	\begin{verbatim}
		 *		abort(__FILE__, __LINE__, shift_reference->elements.size() > 0,
		 *					"no data for shift references found");
		 *	\end{verbatim}
		 *	@param file should be used for __FILE__
		 *	@param line should be used for __LINE__
		 *	@param condition to be tested
		 *	@param msg this string is used as message in the exception
		 *	@throw Exception::ParseError if <tt>condition</tt> is not fulfilled
		 */
		void test(const char* file, int line, bool condition, const String& msg) const;

		/** Function to get a field surrounded by delimiter
		 *  @see String::getField
		 *  @see String::getFieldQuoted
		 */
		String getField(Index pos = 0, const String& quotes = "",
										const String& delimiters = String::CHARACTER_CLASS__WHITESPACE) const;

		/// Test if the current line starts with text
		bool startsWith(const String& text) const;

		/// Return true if the current line contains text
		bool has(const String& text) const;

		/** Switch method of the current line.
				Return the position of the current line in data or -1 if it does not exist.
		*/
		Index switchString(const std::vector<String>& data) const;

		/**	Parse column based formats.
				Copy the subsection of the current line defined by <tt>index</tt> and <tt>length</tt> into a buffer
				try to parse it using <tt>sscanf</tt>. The result is stored in <tt>arg</tt> (use with caution: no type checking!).
		*/
		bool parseColumnFormat(const char* format, Position index, Size length, void* arg);

		/// Set wheter leading and trailing whitespaces in lines shall be removed
		void enableTrimWhitespaces(bool state);
		
		///
		bool trimWhiteSpacesEnabled() const;

		protected:
		//@}
		/*	@name	Protected Attributes
		*/
		//_@{
		/// buffer for the line in use
		String line_;

		/// line number in the file
		Position line_number_;

		bool trim_whitespaces_;

		/// buffer for column parsing
		vector<char> col_buffer_;

		//_@}
	};


# ifndef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/FORMAT/lineBasedFile.iC>
# endif
} // namespace BALL

#endif // BALL_FORMAT_LINEBASEDFILE_H
