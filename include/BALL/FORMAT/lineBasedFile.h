#ifndef BALL_FORMAT_LINEBASEDFILE_H
#define BALL_FORMAT_LINEBASEDFILE_H

#ifndef BALL_DATATYPE_String_H
# include <BALL/DATATYPE/string.h>
#endif

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

	/** This class is intended for use with Line Based File Formats.
	*/
	class LineBasedFile
		:	public File
	{
		public:

		//BALL_CREATE(LineBasedFile)

		/**	@name Exceptions
		*/
		//@{
		/** Exception thrown if a file could not be processed right.
		*/
		class LineBasedFileError
			: public Exception::GeneralException
		{
			public:
			LineBasedFileError(const char* file, int line, const LineBasedFile* f = 0, const string& message = "");
		};
		//@}

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
		*/
		LineBasedFile(const LineBasedFile& f)
			throw();

		//@}

		/**	@name Assignment
		*/
		//@{
			
		/// Assignment operator
		const LineBasedFile& operator = (const LineBasedFile& file)
			throw();
			
		/**	Clear method.
		*/
		void clear() 
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

		//@}

		/*_	@name	Help-Methods for File Acces
		*/
		//_@{

		/** Function to get a token surrounded by delimiter
		*/
		String getField(Position pos = 0, const String& quotes = "",
										const String& delimiters = String::CHARACTER_CLASS__WHITESPACE)
			const	throw(Exception::IndexUnderflow, Exception::NullPointer);

		/// Copy a subString to a new String
		String copyString(Position start, Position end = 0) 
			const throw(Exception::IndexUnderflow, Exception::NullPointer);

		/// Function to test if a String starts like an other String
		bool startsWith(const String& text) 
			const throw();

		/// Return true if line_ has text
		bool has(const String& text) 
			const throw();

		/// Function to test if a String starts like an other String
		bool search(const String& text)
			throw();

		/// Like search above, but stops search when coming to a line staring with stop
		bool search(const String& text, const String& stop)
			throw();

		/// Return the position of line_ in data or -1 if it does not exist in data
		Index switchString(const std::vector<String>& data) 
			const throw();

		/// Tests a condition, if false prints an errormsg and terminates the programm
		void test(const char* file, int line, bool condition, const String& msg) 
			const throw(LineBasedFileError);

		/// Reads a line and counts the line number
		void readLine()
			throw();

		/// Skip a given number of lines
		void skipLines(Size number = 1)
			throw(Exception::IndexUnderflow);

		/// Rewind file to start
		void rewind()
			throw();

		//_@}

		/*	@name	Protected Attributes
		*/
		//_@{

		/// buffer for the line in use
		String line_;

		/// line number in the file
		Position line_number_;
		//_@}
	};

} // namespace BALL

#endif // BALL_FORMAT_LINEBASEDFILE_H
