#include <iostream> 
#include <fstream>
#include <string>
#include <vector>
#include<BALL/FORMAT/readFile.h>

using namespace std;

namespace BALL
{

	char* ReadFile::getToken_(Position& pos, char delimiter) const
	{
		int len = 0;
		char* p = new char[200];

		if(pos >= strlen(line_)) 
		{
			Log.error() << "in getToken_ : pos too large" << endl;
			throw ReadFileError();
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
				Log.error() << "open token" << endl;
				throw ReadFileError();
			}

			p[len] = line_[pos];
			++len;
			++pos;
		}

		++pos;

		if(len == 0) 
		{
			Log.error() << "token could not be read" << endl;
			throw ReadFileError();
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
			Log.error() << "in getToken_ : pos too large" << endl;
			throw ReadFileError();
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
			Log.error() << "token could not be read" << endl;
			throw ReadFileError();
		}

		p[len] = '\0';
		return p;
	}

	char* ReadFile::getToken_() const
	{
		if (strlen(line_) > 199)
		{
			Log.error() << "line too long" << endl;
			throw ReadFileError();
		}
		int len = 0;
		int pos = 0;

		char* p = new char[200];

		if(p == 0) 
		{
			Log.error() << "memory could not be assigned" << endl;
			throw ReadFileError();
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
			Log.error() << "token could not be read" << endl;
			throw ReadFileError();
		}

		p[len] = '\0';
		return p;
	}

	string ReadFile::copyString_(Position start, Position end) const
	{
		if (end >= strlen(line_))
		{
			Log.error() << "error in copyString_";
			throw ReadFileError();
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
			Log.error() << msg;
			throw ReadFileError();
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


} //namespace