// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/COMMON/exception.h>
#include <cstdio>

using namespace std;

namespace BALL
{

	LineBasedFile::LineBasedFile()
		:	File(),
			line_number_(0),
			trim_whitespaces_(false)
	{
	}

	LineBasedFile::LineBasedFile(const String& filename, File::OpenMode open_mode, bool trim_whitespaces)
		: File(),
			line_number_(0),
			trim_whitespaces_(trim_whitespaces)
	{
		File::open(filename, open_mode);
		if (!isAccessible())
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename);
		}
	}

	const LineBasedFile& LineBasedFile::operator = (const LineBasedFile& f)
	{
		open(f.getName(), f.getOpenMode());
		line_number_ = 0;
		trim_whitespaces_ = f.trim_whitespaces_;
		skipLines(f.line_number_ - 1);
		return *this;
	}

	bool LineBasedFile::operator == (const LineBasedFile& f)  
	{
		return File::operator == (f);
	}

	bool LineBasedFile::operator != (const LineBasedFile& f)  
	{
		return !(File::operator == (f));
	}


	bool LineBasedFile::search(const String& text, bool return_to_start)
	{
		if (!isOpen() || getOpenMode() != MODE_IN)
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
	{
		if (!isOpen() || getOpenMode() != MODE_IN)
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
	{
		if (!isOpen() || getOpenMode() != MODE_IN)
		{
			throw Exception::ParseError(__FILE__, __LINE__, String("File '") + getName() + "' not open for reading" , 
																	"LineBasedFile::readLine");
		}
		line_.getline(getFileStream());
		if (trim_whitespaces_) line_.trim();
		++line_number_;
		return !eof();
	}

	bool LineBasedFile::skipLines(Size number)
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
	{
		line_ = "";
		line_number_ = 0;
		File::clear();
		trim_whitespaces_ = false;
	}

	void LineBasedFile::test(const char* file, int line, bool condition, const String& msg) const
	{
		if (!condition)
		{
			throw Exception::ParseError(file, line, String("File '") + getName() + "' while parsing line " + String(getLineNumber()), msg);
		}
	}

	String LineBasedFile::getField(Index pos, const String& quotes, const String& delimiters) const
	{
		if (quotes == "")
		{
			return line_.getField(pos, delimiters.c_str());
		}

		return line_.getFieldQuoted(pos, delimiters.c_str(), quotes.c_str());
	}

	Index LineBasedFile::switchString(const vector<String>& data) const 
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

	bool LineBasedFile::startsWith(const String& text) const 
	{
		return line_.hasPrefix(text);
	}

	bool LineBasedFile::has(const String& text) const 
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

			if (col_buffer_.empty())
				col_buffer_.resize(max_len+1);

			length = std::min(length, max_len);

			// copy the specified string section into the buffer...
			strncpy(&(col_buffer_[0]), line_.c_str() + start, length);
			col_buffer_[length] = '\0';

			// ...and try to parse it.
			read = sscanf(&(col_buffer_[0]), format, ptr);
		}
		else
		{
			Log.warn() << "LineBasedFile::parseColumnFormat: undefined position while parsing line (" 
							   << start << "-" << start + length << " in line of length " << getLine().size() << ")" << std::endl;
		}

		// return true if exactly one entry was read
		return (read == 1);
	}

	void LineBasedFile::enableTrimWhitespaces(bool state)
	{
		trim_whitespaces_ = state;
	}

	bool LineBasedFile::trimWhiteSpacesEnabled() const
	{
		return trim_whitespaces_;
	}

# ifdef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/FORMAT/lineBasedFile.iC>
# endif

} // namespace BALL
