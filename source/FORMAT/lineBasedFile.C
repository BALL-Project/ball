// $Id: lineBasedFile.C,v 1.1 2000/10/10 08:51:56 amoll Exp $

#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/COMMON/exception.h>
#include <stdio.h>

using namespace std;

namespace BALL
{

	LineBasedFile::LineBasedFileError::LineBasedFileError
		(const char* file, int line, const LineBasedFile* f, const string& message)
		: Exception::GeneralException(file, line, "LineBasedFileError", "")
	{
		message_ = message;
		if (f != 0)
		{
			message_ += "\n last read line number = ";
			char buf[40];
			sprintf(buf, "%i", f->getLineNumber());
			message_ += buf;
			message_ += "\n contents of line: \n";
			message_ += f->getLine();
		}
		Exception::globalHandler.setMessage(message_);
	}


	Position LineBasedFile::getLineNumber() 
		const throw()
	{
		return line_number_;
	}

	const String& LineBasedFile::getLine() 
		const throw()
	{
		return line_;
	}

	LineBasedFile::LineBasedFile(const String& filename, File::OpenMode open_mode = File::IN)
		throw(Exception::FileNotFound)
		: File(filename, open_mode),
			line_number_(0)
	{
	}

	String LineBasedFile::getField(Position pos, const String& quotes, const String& delimiters)
		const	throw(Exception::IndexUnderflow, Exception::NullPointer)
	{
		if (quotes == "")
		{
			return line_.getField(pos, delimiters.c_str());
		}

		return line_.getFieldQuoted(pos, delimiters.c_str(), quotes.c_str());
	}

	String LineBasedFile::copyString(Position start, Position end)
		const throw(Exception::IndexUnderflow, Exception::NullPointer)
	{
		if (end == 0)
		{
			end = line_.size() - 1;
		}

		return String(start, end - start);
	}

	int LineBasedFile::switchString(const vector<String>& data)
		const throw()
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

	bool LineBasedFile::startsWith(const String& text)
		const throw()
	{
		return line_.hasPrefix(text);
	}

	bool LineBasedFile::search(const String& text)
		throw()
	{
		while (!eof())
		{
			readLine();

			if (startsWith(text))
			{
				return true;
			}
		}
		return false;
	}

	bool LineBasedFile::search(const String& text, const String& stop)
		throw()
	{
		while (!eof())
		{
			readLine();
			if (has(stop))
			{
				return false;
			}

			if (startsWith(text))
			{
				return true;
			}
		}
		return false;
	}

	void LineBasedFile::test(const char* file, int line, bool condition, const String& msg)
		const throw(LineBasedFileError)
	{
		if (!condition)
		{
			throw LineBasedFileError(file, line, this, msg);
		}
	}

	void LineBasedFile::readLine()
		throw()
	{
		char* c = new char[200];
		getline(c, 200);
		line_ = c;
		++line_number_;
	}

	void LineBasedFile::skipLines(Size number)
		throw(Exception::IndexUnderflow)
	{
		if (number < 1)
		{
			throw Exception::IndexUnderflow(__FILE__, __LINE__);
		}
		for (Position i = 0; i < number +1; i++)
		{
			readLine();
		}
	}

	bool LineBasedFile::has(const String& text) 
		const throw()
	{
		return line_.hasSubstring(text);
	}

	void LineBasedFile::rewind()
		 throw()
	{
		File::reopen();
		line_number_ = 0;
		line_ = "";
	}

	const LineBasedFile& LineBasedFile::operator = (const LineBasedFile& f)
		throw()
	{
		name_ = f.name_;
		rewind();
		return *this;
	}

	LineBasedFile::LineBasedFile(const LineBasedFile& f)
	 throw()
		: File(f)
	{
		name_ = f.name_;
		rewind();
	}

} //namespace
