// $Id: lineBasedFile.C,v 1.6 2000/10/15 22:36:10 amoll Exp $

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

	LineBasedFile::LineBasedFile()
		throw()
		:	File(),
			line_number_(0)
	{
	}

	LineBasedFile::LineBasedFile(const LineBasedFile& f)
	 throw()
		: File(f),
			line_number_(0)
	{
		if (f.line_number_ == 1)
		{
			readLine();
		}

		if (f.line_number_ > 1)
		{
			skipLines(f.line_number_ - 1);
		}
	}

	LineBasedFile::LineBasedFile(const String& filename, File::OpenMode open_mode)
		throw(Exception::FileNotFound)
		: File(filename, open_mode),
			line_number_(0)
	{
		if (!isAccessible())
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename);
		}
	}

	const LineBasedFile& LineBasedFile::operator = (const LineBasedFile& f)
		throw()
	{
		*this = LineBasedFile(f);
		return *this;
	}

	bool LineBasedFile::search(const String& text, bool return_to_point)
		throw(LineBasedFileError)
	{
		if (!isOpen() || getOpenMode() != IN)
		{
			throw LineBasedFileError(__FILE__, __LINE__, this, 
							"File " + name_ +" is not opend for read access or at all.");
		}

		Position start_point = line_number_;

		while (readLine())
		{
			if (startsWith(text))
			{
				return true;
			}
		}
		
		if (return_to_point)
		{
			goToLine(start_point);
		}

		return false;
	}

	bool LineBasedFile::search(const String& text, const String& stop, bool return_to_point)
		throw(LineBasedFileError)
	{
		if (!isOpen() || getOpenMode() != IN)
		{
			throw LineBasedFileError(__FILE__, __LINE__, this, 
							"File " + name_ +" is not opend for read access or at all.");
		}

		Position start_point = line_number_;

		while (readLine())
		{
			if (has(stop))
			{
				if (return_to_point)
				{
					goToLine(start_point);
				}

				return false;
			}

			if (startsWith(text))
			{
				return true;
			}
		}

		if (return_to_point)
		{
			goToLine(start_point);
		}

		return false;
	}

	bool LineBasedFile::readLine()
		throw(LineBasedFileError)
	{
		if (!isOpen() || getOpenMode() != IN)
		{
			throw LineBasedFileError(__FILE__, __LINE__, this, 
							"File " + name_ +" is not opend for read access or at all.");
		}

		if (eof())
		{
			return false;
		}

		char* c = new char[200];
		getline(c, 200);
		line_ = c;
		++line_number_;
		
		return true;
	}

	bool LineBasedFile::skipLines(Size number)
		throw(LineBasedFileError)
	{
		for (Position i = 0; i < number +1; i++)
		{
			if (!readLine())
			{
				return false;
			}
		}

		return true;
	}

	void LineBasedFile::rewind()
		 throw(LineBasedFileError)
	{
		if (!isOpen())
		{
			throw LineBasedFileError(__FILE__, __LINE__, this, 
							"File " + name_ +" is not opend.");
		}
		File::reopen();
		line_number_ = 0;
		line_ = "";
	}

	bool LineBasedFile::goToLine(Position line_number)
		 throw(LineBasedFileError)
	{
		if (!isOpen())
		{
			throw LineBasedFileError(__FILE__, __LINE__, this, 
							"File " + name_ +" is not opend.");
		}

		if (line_number == line_number_)
		{
			return true;
		}

		if (line_number < line_number_)
		{
			rewind();
			if (line_number == 0)
			{
				return true;
			}

			return skipLines(line_number - 1);
		}

		return skipLines(line_number - line_number_ - 1);
	}


	void LineBasedFile::clear()
		throw()
	{
		line_ = "";
		line_number_ = 0;
		close();
		name_ = "";
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

	void LineBasedFile::test(const char* file, int line, bool condition, const String& msg)
		const throw(LineBasedFileError)
	{
		if (!condition)
		{
			throw LineBasedFileError(file, line, this, msg);
		}
	}

	String LineBasedFile::getField(Position pos, const String& quotes, const String& delimiters)
		const	throw(Exception::IndexUnderflow)
	{
		if (quotes == "")
		{
			return line_.getField(pos, delimiters.c_str());
		}

		return line_.getFieldQuoted(pos, delimiters.c_str(), quotes.c_str());
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

	bool LineBasedFile::has(const String& text) 
		const throw()
	{
		return line_.hasSubstring(text);
	}

} //namespace
