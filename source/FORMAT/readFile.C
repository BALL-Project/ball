// $Id: readFile.C,v 1.6 2000/10/03 02:04:06 amoll Exp $

#include<BALL/FORMAT/readFile.h>

using namespace std;

namespace BALL
{

	ReadFile::ReadFile(const String& file_name)
		: line_number_(0),
			file_name_(file_name)
	{
		in.open(file_name_.c_str()); 
		if (!in)
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, file_name);
		}
	}

	String ReadFile::getToken_(Position& pos, char delimiter) const
	{
		test_(pos < line_.size(), "in getToken_ : pos too large");

		while (pos < line_.size() && line_[pos] != delimiter)
		{
			++pos;
		}
		++pos;

		test_(pos < line_.size(), "open token");

		String token;

		while (line_[pos] != delimiter)
		{
			test_(pos < line_.size(), "open token");

			token += line_[pos];
			++pos;
		}

		++pos;
		test_(token.size() > 0, "token could not be read");
		return token;
	}

	String ReadFile::getToken_(Position& pos) const
	{
		test_(pos < line_.size(), "in getToken_ : pos too large");

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
		test_(token.size() > 0, "token could not be read");
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
		
		test_(token.size() > 0, "token could not be read");
		return token;
	}

	String ReadFile::copyString_(Position start, Position end) const
	{
		if (end == 0)
		{
			end = line_.size() - 1;
		}

		test_(end < line_.size(), "error in copyString_");
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
		if (!condition)
		{
			throw Exception::ReadFileError(msg, line_, line_number_);
		}
	}

	void ReadFile::readLine_()
	{
		char* c = new char[200];
		in.getline(c, 200);
		line_ = c;
		++line_number_;
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
		in.close();
		in.open(file_name_.c_str());
	}

} //namespace
