//#include <stdio.h>
#include <iostream> 
#include <fstream>
#include <string>
#include <vector>

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
			class ReadFile_ProcessError{};

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

	char* ReadFile::getToken_(Position& pos, char delimiter) const
	{
		int len = 0;
		char* p = new char[200];

		if(pos >= strlen(line_)) 
		{
			cerr << "in getToken_ : pos too large" << endl;
			throw ReadFile_ProcessError();
		}

		while (line_[pos] && line_[pos] != delimiter)
		{
			++pos;
		}

		++pos;

		while (line_[pos] != delimiter)
		{
			if (!line_[pos])
			{
				cerr << "open token" << endl;
				throw ReadFile_ProcessError();
			}

			p[len] = line_[pos];
			++len;
			++pos;
		}

		++pos;

		if(len == 0) 
		{
			cerr << "token could not be read" << endl;
			throw ReadFile_ProcessError();
		}

		p[len] = '\0';
		return p;
	}

	char* ReadFile::getToken_(Position& pos) const
	{
		int len = 0;
		char* p = new char[200];

		if(pos >= strlen(line_)) 
		{
			cerr << "in getToken_ : pos too large" << endl;
			throw ReadFile_ProcessError();
		}

		while (line_[pos] && isspace(line_[pos]))
		{
			++pos;
		}

		while (line_[pos] && !isspace(line_[pos]))
		{
			p[len] = line_[pos];
			++len;
			++pos;
		}
		
		if(len == 0) 
		{
			cerr << "token could not be read" << endl;
			throw ReadFile_ProcessError();
		}

		p[len] = '\0';
		return p;
	}

	char* ReadFile::getToken_() const
	{
		if (strlen(line_) > 199)
		{
			cerr << "line too long" << endl;
			throw ReadFile_ProcessError();
		}
		int len = 0;
		int pos = 0;

		char* p = new char[200];

		if(p == 0) 
		{
			cerr << "memory could not be assigned" << endl;
			throw ReadFile_ProcessError();
		}

		while (line_[pos] && isspace(line_[pos]))
		{
			++pos;
		}

		while (line_[pos] && !isspace(line_[pos]))
		{
			p[len] = line_[pos];
			++len;
			++pos;
		}
		
		if(len == 0) 
		{
			cerr << "token could not be read" << endl;
			throw ReadFile_ProcessError();
		}

		p[len] = '\0';
		return p;
	}

	string ReadFile::copyString_(Position start, Position end) const
	{
		if (end >= strlen(line_))
		{
			cerr << "error in copyString_";
			throw ReadFile_ProcessError();
		}
		string dest;
		for (; start <= end ; ++start)
		{
			dest += line_[start];
		}
		return dest;
	}

	int ReadFile::switch_(const vector<string>& data) const
	{
		Position i = 0;
		while (i < data.size())
		{
			if (strcmp(line_, data[i].c_str()) == 0)
			{
				return i;
			}
			++i;
		}
		return (-1);
	}

	bool ReadFile::startsWith_(const char* text) const
	{
		return (memcmp(line_, text, strlen(text)) == 0);
	}

	bool ReadFile::search_(const char* text)
	{
		while (!in.eof())
		{
			readLine_();

			if (startsWith_(text))
			{
				return true;
			}
		}
		return false;
	}

	bool ReadFile::search_(const char* text, const char* stop)
	{
		while (!in.eof())
		{
			readLine_();
			if (has_(stop))
			{
				return false;
			}

			if (startsWith_(text))
			{
				return true;
			}
		}
		return false;
	}

	void ReadFile::test_(bool condition, char* msg) const
	{
		if (condition == false)
		{
			cerr << msg;
			throw ReadFile_ProcessError();
		}
	}

	void ReadFile::readLine_()
	{
		in.getline(line_,200);
	}

	void ReadFile::skipLines_(int number)
	{
		for (int i = 0;  i <= number ; i++)
		{
			in.getline(line_,200);
		}
	}

	bool ReadFile::has_(const char* text) const
	{
		Position pos_line = 0;
		Position pos_line2;
		Position pos_text;
		if(strlen(line_) == 0 || strlen(text) == 0 ||
			 strlen(line_) < strlen(text) ) 
		{
			return false;
		}

		while (line_[pos_line])
		{
			pos_text = 0;
			pos_line2 = pos_line;
			while (text[pos_text])
			{
				if (line_[pos_line2] != text[pos_text])
				{
					break;
				}
				++pos_text;
				++pos_line2;
			}

			if (!text[pos_text])
			{
				return true;
			}

			++pos_line;
		}

		return false;
	}

} // namespace BALL