#include<BALL/FORMAT/readFile.h>

namespace BALL
{

	String ReadFile::getToken_(Position& pos, char delimiter) const
	{
		if (pos >= line_.size())
		{
			Log.error() << "in getToken_ : pos too large" << endl;
			throw ReadFileError();
		}

		while (pos < line_.size() && line_[pos] != delimiter)
		{
			++pos;
		}
		++pos;

		if (pos >= line_.size())
		{
			Log.error() << "open token" << endl;
			throw ReadFileError();
		}

		String token;

		while (line_[pos] != delimiter)
		{
			if (pos >= line_.size())
			{
				Log.error() << "open token" << endl;
				throw ReadFileError();
			}

			token += line_[pos];
			++pos;
		}

		++pos;

		if(token.size() == 0) 
		{
			Log.error() << "token could not be read" << endl;
			throw ReadFileError();
		}

		return token;
	}

	String ReadFile::getToken_(Position& pos) const
	{
		if (pos >= line_.size())
		{
			Log.error() << "in getToken_ : pos too large" << endl;
			throw ReadFileError();
		}

		while (pos < line_.size() && isspace(line_[pos]))
		{
			++pos;
		}

		String token;

		while (pos < line_.size() && !isspace(line_[pos]))
		{
			token += line_[pos];
			++pos;
		}
		++pos;
	
		if(token.size() == 0) 
		{
			Log.error() << "token could not be read" << endl;
			throw ReadFileError();
		}

		return token;
	}

	String ReadFile::getToken_() const
	{
		Position pos = 0;

		while (pos < line_.size() && isspace(line_[pos]))
		{
			++pos;
		}

		String token;

		while (pos < line_.size() && !isspace(line_[pos]))
		{
			token += line_[pos];
			++pos;
		}
		
		if(token.size() == 0) 
		{
			Log.error() << "token could not be read" << endl;
			throw ReadFileError();
		}

		return token;
	}

	String ReadFile::copyString_(Position start, Position end) const
	{
		if (end == 0)
		{
			end = line_.size() - 1;
		}

		if (end >= line_.size())
		{
			Log.error() << "error in copyString_";
			throw ReadFileError();
		}
		String dest;
		for (; start <= end ; ++start)
		{
			dest += line_[start];
		}
		return dest;
	}

	int ReadFile::switch_(const vector<String>& data) const
	{
		for (Position i = 0; i < data.size(); i++)
		{
			if (line_ == data[i])
			{
				return i;
			}
		}
		return (-1);
	}

	bool ReadFile::startsWith_(const String& text) const
	{
		return line_.hasPrefix(text);
	}

	bool ReadFile::search_(const String& text)
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

	bool ReadFile::search_(const String& text, const String& stop)
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

	void ReadFile::test_(bool condition, const String& msg) const
	{
		if (condition == false)
		{
			Log.warn() << msg << endl;
			throw ReadFileError();
		}
	}

	void ReadFile::readLine_()
	{
		char* c = new char[200];
		in.getline(c, 200);
		line_ = c;
		//in.getline(const_cast<char*>(line_.c_str()), 200);
	}

	void ReadFile::skipLines_(Size number)
	{
		for (Position i = 0; i <= number; i++)
		{
			readLine_();
		}
	}

	bool ReadFile::has_(const String& text) const
	{
		return line_.hasSubstring(text);
	}

	void ReadFile::rewind_()
	{
		in.seekg(0);
	}

} //namespace
