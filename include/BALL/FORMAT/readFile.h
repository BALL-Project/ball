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

			/** Exception thrown if file could not be processed right.
			*/
			class ReadFileError{};

		protected:
			/*_	@name	Help-Methods for File Acces
			*/
			//_@{

			/** function to get a token surrounded by delimiter
					starting at position pos in the String
			*/
			String getToken_(Position& pos, char delimiter) const;

			/** function to get a token starting at position pos in the String
			*/
			String getToken_(Position& pos) const;

			/// function to get the first token from a String
			String getToken_() const;

			/// copy a subString to a new String
			String copyString_(Position start, Position end = 0) const;

			/// function to test if a String starts like an other String
			bool startsWith_(const String& text) const;

			/// function to test if a String starts like an other String
			bool search_(const String& text);

			/// like search above, but stops search when coming to a line staring with stop
			bool search_(const String& text, const String& stop);

			/// return the position of line_ in data or -1 if it does not exist in data
			int switch_(const std::vector<String>& data) const;

			/// tests a condition, if false prints an errormsg and terminates the programm
			void test_(bool condition, const String& msg) const;

			/// reads a line
			void readLine_();

			/// skip a given number of lines
			void skipLines_(Size number = 0);

			/// return true if line_ has text
			bool has_(const String& text) const;
			
			/// spool back to the start of the file
			void rewind_();

			//_@}

			/*_	@name	Help-Attributes for File Access
			*/
			//_@{

			/// filestream to read
			ifstream in;

			/// buffer for the line in use
			String line_;

			//_@}
	};

} // namespace BALL

#endif // BALL_FORMAT_READFILE_H
