#ifndef BALL_FORMAT_READFILE_H
#define BALL_FORMAT_READFILE_H

#ifndef BALL_DATATYPE_String_H
# include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <vector>
#include <fstream>

namespace BALL 
{

	/**
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
			LineBasedFileError(const char* file, int line, const LineBasedFile* rf = 0, const string& message = "");
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
			throw(FileNotFound);

		/** Copy constructor
		*/
		LineBasedFile(const LineBasedFile& rf);

		//@}

		/**	@name Assignment
		*/
		//@{
			
		/// Assignment operator
		const LineBasedFile& operator = (const LineBasedFile& file)
			const throw();
			
		/**	Clear method.
		*/
		void clear() 
			throw();
		//@}

		/**	@name Accessors
		*/
		//@{
		/// Get the last line number in the file.
		Position getLineNumber() const;

		/// Return the current line
		const String& getLine() const;

		//@}


		/** Assignment operator
		*/
		LineBasedFile& operator = (const LineBasedFile& rf);

		/*_	@name	Help-Methods for File Acces
		*/
		//_@{

		/** Function to get a token surrounded by delimiter
				starting at position pos in the String
		*/
		String getField(Position& pos, char delimiter) const;

		/** Function to get a token starting at position pos in the String
		*/
		String getToken_(Position& pos) const;

		/// Function to get the first token from a String
		String getToken_() const;

		/// Copy a subString to a new String
		String copyString_(Position start, Position end = 0) const;

		/// Function to test if a String starts like an other String
		bool startsWith_(const String& text) const;

		/// Function to test if a String starts like an other String
		bool search_(const String& text);

		/// Like search above, but stops search when coming to a line staring with stop
		bool search_(const String& text, const String& stop);

		/// Return the position of line_ in data or -1 if it does not exist in data
		int switch_(const std::vector<String>& data) const;

		/// Tests a condition, if false prints an errormsg and terminates the programm
		void test_(const char* file, int line, bool condition, const String& msg) const;

		/// Reads a line and counts the line number
		void readLine();

		/// skip a given number of lines
		void skipLines(Size number = 1);

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

#endif // BALL_FORMAT_READFILE_H
