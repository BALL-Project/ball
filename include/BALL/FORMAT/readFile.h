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

	class ReadFile
	{
		public:

			/** Exception thrown if a file could not be processed right.
			*/
			class ReadFileError
				: public Exception::GeneralException
			{
				public:
				ReadFileError(const char* file, int line, const ReadFile& rf, const String& message = "");
			};

			/// Get the last line number in the file.
			Position getLineNumber() const;

			/// Get the contens in the current line in the open file.
			const String& getLineContens() const;

		protected:

			/*_	@name	Constructors
			*/
			//_@{

			/** Standard constructor
			*/	
			ReadFile() {};

			/** Detailled constuctor.
					Opens the given file.
			*/
			ReadFile(const String& file_name);

			/** Copy constructor
			*/
			ReadFile(const ReadFile& rf);

			//_@}


			/** Assignment operator
			*/
			ReadFile& operator = (const ReadFile& rf);

			/*_	@name	Help-Methods for File Acces
			*/
			//_@{

			/** Function to get a token surrounded by delimiter
					starting at position pos in the String
			*/
			String getToken_(Position& pos, char delimiter) const;

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
			void readLine_();

			/// skip a given number of lines
			void skipLines_(Size number = 0);

			/// Return true if line_ has text
			bool has_(const String& text) const;
			
			/** Spool back to the start of the file.
					This is done by closing and reopening the file.
			*/
			void rewind_();

			//_@}

			/*_	@name	Help-Attributes for File Access
			*/
			//_@{

			/// filestream to read
			std::ifstream in;

			/// buffer for the line in use
			String line_;

			/// line number in the file
			Position line_number_;

			/// filename
			String file_name_;

			//_@}

	};

} // namespace BALL

#endif // BALL_FORMAT_READFILE_H
