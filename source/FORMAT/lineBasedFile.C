// $Id: lineBasedFile.C,v 1.20 2001/12/20 02:35:35 oliver Exp $

#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/COMMON/exception.h>
#include <stdio.h>

using namespace std;

namespace BALL
{

	LineBasedFile::LineBasedFile()
		throw()
		:	File(),
			line_number_(0)
	{
	}

	LineBasedFile::LineBasedFile(const LineBasedFile& f)
	 throw(Exception::FileNotFound)
		: File(),
			line_number_(0)
	{
			if (f.getName() != "")
			{
				open(f.getName());
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
		open(f.getName(), f.getOpenMode());
		line_number_ = 0;
		skipLines(f.line_number_ - 1);
		return *this;
	}

	bool LineBasedFile::operator == (const LineBasedFile& f)  throw()
	{
		return File::operator == (f);
	}

	bool LineBasedFile::operator != (const LineBasedFile& f)  throw()
	{
		return !(File::operator == (f));
	}


	bool LineBasedFile::search(const String& text, bool return_to_start)
		throw(Exception::ParseError)
	{
		if (!isOpen() || getOpenMode() != IN)
		{
			throw Exception::ParseError(__FILE__, __LINE__, String("File '") + getName() + "' not open for reading" , 
																	"LineBasedFile::search");
		}

		Position start_point = line_number_;
		while (readLine())
		{
			if (startsWith(text))
			{ 
				return true;
			}
		}
		
		if (return_to_start)
		{
			gotoLine(start_point);
		}
		return false;
	}

	bool LineBasedFile::search(const String& text, const String& stop, bool return_to_start)
		throw(Exception::ParseError)
	{
		if (!isOpen() || getOpenMode() != IN)
		{
			throw Exception::ParseError(__FILE__, __LINE__, String("File '") + getName() + "' not open for reading" , 
																	"LineBasedFile::search");
		}

		Position start_point = line_number_;

		while (readLine())
		{
			if (startsWith(stop))
			{
				if (return_to_start)
				{
					gotoLine(start_point);
				}

				return false;
			}

			if (startsWith(text))
			{
				return true;
			}
		}

		if (return_to_start)
		{
			gotoLine(start_point);
		}

		return false;
	}

	bool LineBasedFile::readLine()
		throw(Exception::ParseError)
	{
		if (!isOpen() || getOpenMode() != IN)
		{
			throw Exception::ParseError(__FILE__, __LINE__, String("File '") + getName() + "' not open for reading" , 
																	"LineBasedFile::readLine");
		}

		line_.getline(getFileStream());
		++line_number_;
		return eof();
	}

	bool LineBasedFile::skipLines(Size number)
		throw(Exception::ParseError)
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
		 throw(Exception::ParseError)
	{
		if (!isOpen())
		{
			throw Exception::ParseError(__FILE__, __LINE__, String("File '") + getName() + "' not open" , 
																	"LineBasedFile::rewind");
		}
		File::reopen();
		line_number_ = 0;
		line_ = "";
	}

	bool LineBasedFile::gotoLine(Position line_number)
		 throw(Exception::ParseError)
	{
		if (!isOpen())
		{
			throw Exception::ParseError(__FILE__, __LINE__, String("File '") + getName() + "' not open for reading" , 
																	"LineBasedFile::gotoLine");
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
		File::clear();
	}

	void LineBasedFile::test(const char* file, int line, bool condition, const String& msg)
		const throw(Exception::ParseError)
	{
		if (!condition)
		{
			throw Exception::ParseError(__FILE__, __LINE__, String("File '") + getName() + "' while parsing line " + String(getLineNumber()), msg);
		}
	}

	String LineBasedFile::getField(Index pos, const String& quotes, const String& delimiters)
		const	throw(Exception::IndexUnderflow)
	{
		if (quotes == "")
		{
			return line_.getField(pos, delimiters.c_str());
		}

		return line_.getFieldQuoted(pos, delimiters.c_str(), quotes.c_str());
	}

	Index LineBasedFile::switchString(const vector<String>& data)
		const throw()
	{
		for (Index i = 0; i < (Index) data.size(); i++)
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

	bool LineBasedFile::parseColumnFormat(const char* format, Position start, Size length, void* ptr)
	{
		// the number of entries parsed
		int read = 0;

		// make sure the specified section of the string exists
		if (getLine().size() >= (start + length))
		{
			const Size max_len = 16384;
			static char buf[max_len + 1];
			length = std::min(length, max_len);

			// copy the specified string section into the buffer...
			strncpy(buf, line_.c_str() + start, length);
			buf[length] = '\0';

			// ...and try to parse it.
			read = sscanf(buf, format, ptr);
		}
		else
		{
			Log.warn() << "LineBasedFile::parseColumnFormat: undefined position while parsing line (" 
							   << start << "-" << start + length << " in line of length " << getLine().size() << ")" << std::endl;
		}

		// return true if exactly one entry was read
		return (read == 1);
	}


# ifdef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/FORMAT/lineBasedFile.iC>
# endif



} // namespace BALL
