#ifndef BALL_FORMAT_READFILE_H
#define BALL_FORMAT_READFILE_H

#include <string>
#include <vector>
#include <fstream>

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{
	using namespace std;

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
					starting at position pos in the string
			*/
			char* getToken_(Position& pos, char delimiter) const;

			/** function to get a token starting at position pos in the string
			*/
			char* getToken_(Position& pos) const;

			/// function to get the first token from a string
			char* getToken_() const;

			/// copy a substring to a new string
			string copyString_(Position start, Position end) const;

			/// function to test if a string starts like an other string
			bool startsWith_(const char* text) const;

			/// function to test if a string starts like an other string
			bool search_(const char* text);

			/// like search above, but stops search when coming to a line staring with stop
			bool search_(const char* text, const char* stop);

			/// return the position of line_ in data or -1 if it does not exist in data
			int switch_(const vector<string>& data) const;

			/// tests a condition, if false prints an errormsg and terminates the programm
			void test_(bool condition, char* msg) const;

			/// reads a line
			void readLine_();

			/// skip a given number of lines
			void skipLines_(int number = 0);

			/// return true if line_ has text
			bool has_(const char* text) const;

			//_@}

			/*_	@name	Help-Attributes for File Access
			*/
			//_@{

			/// filestream to read
			ifstream in;

			/// buffer for the line in use
			char line_[200];

			//_@}
	};

} // namespace BALL

#endif // BALL_FORMAT_READFILE_H
