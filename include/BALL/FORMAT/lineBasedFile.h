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

		BALL_CREATE(LineBasedFile)

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
			
		/** Assignment operator.
				The file is opened and the same position in it is seeked.
		*/
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

		/**	@name	Help-Methods for File Acces
		*/
		//@{

		/** Reads a line and counts the line number.
				@return true if a line could be read, false if End Of File.
		*/
		bool readLine()
			throw(LineBasedFileError);

		/** Skip a given number of lines.
				@return false, if EOF occurs.
		*/
		bool skipLines(Size number = 1)
			throw(LineBasedFileError);

		/** Function to search for a line starting like a given String.
				The search is started at the actual line.
				@param return_to_point : true if line can not be found return to the starting position
				@return true if line could be found
		*/
		bool search(const String& text, bool return_to_point = false)
			throw(LineBasedFileError);

		/** Like search above, but stop search when coming to a line staring with stop
		*/
		bool search(const String& text, const String& stop, bool return_to_point = false)
			throw(LineBasedFileError);

		/** Go to a given line.
				@return false if EOF occurs
		*/
		bool goToLine(Position line_number)
			throw(LineBasedFileError);

		/// Rewind file to start
		void rewind()
			throw(LineBasedFileError);

		/// Tests a condition, if false prints an errormsg and terminates the programm
		void test(const char* file, int line, bool condition, const String& msg) 
			const throw(LineBasedFileError);

		/** Function to get a field surrounded by delimiter
		*/
		String getField(Index pos = 0, const String& quotes = "",
										const String& delimiters = String::CHARACTER_CLASS__WHITESPACE)
			const	throw(Exception::IndexUnderflow);

		/// Function to test if a String starts like an other String
		bool startsWith(const String& text) 
			const throw();

		/// Return true if line_ has text
		bool has(const String& text) 
			const throw();

		/// Return the position of line_ in data or -1 if it does not exist in data
		Index switchString(const std::vector<String>& data) 
			const throw();

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
