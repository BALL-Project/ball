// $Id: string.C,v 1.38 2001/08/18 14:27:00 oliver Exp $

#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/limits.h>

#include <stdio.h>
#include <stdarg.h>
#include <algorithm>

using std::ostream;
using std::istream;
using std::strstream;
using std::endl;
using std::ends;
using std::vector;

namespace BALL 
{

#ifndef BALL_HAVE_VSNPRINTF
	int BALLString_vsnprintf(char* s, size_t n, const char* format,  va_list ap)
	{
		// this is an ugly hack - this is safe only up to
		// the static buffer size - no time to implement
		// something more sophisticated... OK
		char* tmp = new char[65536];
		vsprintf(tmp, format, ap);
		if (n > 65535)
		{	
			n = 65535;
		}
		strncpy(s, tmp, n - 1);
		s[n - 1] = (char)0;
		delete [] tmp;
		return (int)strlen(s);
	}
#	define vsnprintf BALLString_vsnprintf
#endif

	const char* String::CHARACTER_CLASS__ASCII_ALPHA = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const char* String::CHARACTER_CLASS__ASCII_ALPHANUMERIC = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	const char* String::CHARACTER_CLASS__ASCII_LOWER = "abcdefghijklmnopqrstuvwxyz";
	const char* String::CHARACTER_CLASS__ASCII_NUMERIC = "0123456789";
	const char* String::CHARACTER_CLASS__ASCII_UPPER = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const char* String::CHARACTER_CLASS__WHITESPACE = " \n\t\r\f\v";
	const char* String::CHARACTER_CLASS__QUOTES = "\"";

	const Size String::EndPos = Limits<Size>::max();

	String::CompareMode String::compare_mode_ = String::CASE_SENSITIVE;

	Substring::UnboundSubstring::UnboundSubstring(const char* file, int line)
		:	Exception::GeneralException(file, line, "UnboundSubstring", "trying to use a substring that was not bound to a string.")
	{
	}

	Substring::InvalidSubstring::InvalidSubstring(const char* file, int line)
		:	Exception::GeneralException(file, line, "InvalidSubstring", "the substring is not valid")
	{
	}

	Substring::Substring()
		throw()
		:	bound_(0),
			from_((Index)String::EndPos),
			to_((Index)String::EndPos)
	{
	}

	Substring::Substring(const Substring& substring, bool /* deep */)
		throw()
		:	bound_(substring.bound_),
			from_(substring.from_),
			to_(substring.to_)
	{
	}

	Substring::Substring(const String& s, Index from, Size len)
		throw(Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		s.validateRange_(from, len);

		bound_	= (String *)& s;
		from_		= from;
		to_			= from + (Index)len - 1;
	}

	Substring::~Substring()
		throw()
	{
	}

	void Substring::dump(ostream& s, Size depth) const
		throw(Substring::UnboundSubstring)
	{
		if (bound_ == 0)
		{
			throw UnboundSubstring(__FILE__, __LINE__);
		}

		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_DEPTH(s, depth);
		s << "  bound String: " << (void *)bound_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  from index: " << from_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  to index: " << to_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  string: ";
		
		const char *end_of_string = bound_->c_str() + to_;

		for (const char* ptr = bound_->c_str() + from_; ptr <= end_of_string; ptr++)
			s << *ptr;

		s << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	ostream& operator << (ostream &s, const Substring& substring)
		throw()
	{
		if (substring.isBound() == false)
		{
			return s;
		}

		const char* char_ptr = substring.bound_->c_str() + substring.from_;
		const char* end_of_string = substring.bound_->c_str() + substring.to_;

		while (char_ptr <= end_of_string)
		{
			s.put(*char_ptr++);
		}

		return s;
	}

	String::String(const char* char_ptr, Index from, Size len)
 	 throw(Exception::NullPointer, Exception::IndexUnderflow,
	Exception::IndexOverflow)
 	 : string()
	{
 	 validateCharPtrRange_(from, len, char_ptr);
 	 if (len > 0)
 	 {
 	   assign(char_ptr + from, len);
 	 }
	}

	// hand-coded create method
	void* String::create(bool /* deep */, bool empty) const
		throw()
	{
		void* ptr;
		if (empty == true)
		{
			ptr = (void*)new String;
		} 
		else 
		{
			ptr = (void*)new String(*this);
		}

		return ptr;
	}
 
	String::String(Size buffer_size, const char* format, ... )
		throw(Exception::IndexUnderflow, Exception::NullPointer)
		: string() 
	{
		if (buffer_size <= 0)
		{
			throw Exception::IndexUnderflow(__FILE__, __LINE__);
		}

		if (format == 0)
		{
       throw Exception::NullPointer(__FILE__, __LINE__);
		}
 
		char* buffer = new char[buffer_size];

		va_list var_arg_list;
		va_start(var_arg_list, format);
		vsnprintf(buffer, (Size)buffer_size, format, var_arg_list);
		va_end(var_arg_list);

		assign(buffer);
		
		delete [] buffer;
	}

	String::String(strstream& s)
		throw()
		: string("")
	{
		s << ends;
		char* str = s.str();
		assign(str);
	}

#	define BALL_STRING_DEFINE_CONSTRUCTOR_METHOD(type, format_string) \
	String::String(type t) 	throw()\
	{ \
		char buffer[128]; \
	\
		sprintf(buffer, format_string, t); \
		assign(buffer);\
	}\

	BALL_STRING_DEFINE_CONSTRUCTOR_METHOD(short, "%hd")
	BALL_STRING_DEFINE_CONSTRUCTOR_METHOD(unsigned short, "%hu")
	BALL_STRING_DEFINE_CONSTRUCTOR_METHOD(int, "%d")
	BALL_STRING_DEFINE_CONSTRUCTOR_METHOD(unsigned int, "%u")
	BALL_STRING_DEFINE_CONSTRUCTOR_METHOD(long, "%ld")
	BALL_STRING_DEFINE_CONSTRUCTOR_METHOD(unsigned long, "%lu")
	BALL_STRING_DEFINE_CONSTRUCTOR_METHOD(float, "%f")
	BALL_STRING_DEFINE_CONSTRUCTOR_METHOD(double, "%f")

	#undef BALL_STRING_DEFINE_CONSTRUCTOR_METHOD

	String::~String()
		throw()
	{
		erase();
	}

	void String::set(const String& s, Index from, Size len)
		throw(Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		s.validateRange_(from, len);

		if (len == 0)
		{
			erase();
		} 
		else 
		{
			assign(s.c_str() + from, len);
		}
	}

	void String::set(const char* s, Index from, Size len)
		throw(Exception::NullPointer, Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		validateCharPtrRange_(from, len, s);

		if (len == 0)
		{
			erase();
		} 
		else 
		{
			assign(s + from, len);
		}
	}

	void String::set(Size buffer_size, const char *format, ... )
		throw(Exception::IndexUnderflow, Exception::NullPointer)
	{
		if (buffer_size <= 0)
		{
		  throw Exception::IndexUnderflow(__FILE__, __LINE__);
		}

		if (format == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}
 
		char* buffer = new char[buffer_size];

		va_list var_arg_list;
		va_start(var_arg_list, format);
		vsnprintf(buffer, (Size)buffer_size, format, var_arg_list);
		va_end(var_arg_list);

		assign(buffer);
		
		delete [] buffer;
	}

#	define BALL_STRING_DEFINE_SET_METHOD(type, format_string) \
	void String::set(type t) 	throw()\
	{ \
		char buffer[128]; \
	\
		sprintf(buffer, format_string, t); \
	\
		assign(buffer);\
	}

	BALL_STRING_DEFINE_SET_METHOD(short, "%hd")
	BALL_STRING_DEFINE_SET_METHOD(unsigned short, "%hu")
	BALL_STRING_DEFINE_SET_METHOD(int, "%d")
	BALL_STRING_DEFINE_SET_METHOD(unsigned int, "%u")
	BALL_STRING_DEFINE_SET_METHOD(long, "%ld")
	BALL_STRING_DEFINE_SET_METHOD(unsigned long, "%lu")
	BALL_STRING_DEFINE_SET_METHOD(float, "%f")
	BALL_STRING_DEFINE_SET_METHOD(double, "%f")

	#undef BALL_STRING_DEFINE_SET_METHOD

  void String::get(char* char_ptr, Index from, Size max_len) const
		throw(Exception::NullPointer, Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		validateIndex_(from);

		if (max_len == 0)
		{
			return;
		}

		Size len = std::min(max_len, (Size)(size() - from));

		const char*	string_ptr = &(c_str()[from]);

		Size i = 0;
		while (i < len) 
		{
			*char_ptr = *string_ptr;

			char_ptr++;
			string_ptr++;
			i++;
		}
		*char_ptr = '\0';
	}
 
	bool String::toBool() const
		throw()
	{
		string::size_type str_index = find_first_not_of(CHARACTER_CLASS__WHITESPACE);
		
		if (size() == 0)
		{
			return true;
		}

		if (str_index != string::npos)
		{
			Size index = (Index)str_index;
			if (!(c_str()[index] == '0' && (isWhitespace(c_str()[index + 1]) == true || c_str()[index + 1] == '\0'))
					&& !(c_str()[index++] == 'f'
					&& c_str()[index++] == 'a'
					&& c_str()[index++] == 'l'
					&& c_str()[index++] == 's'
					&& c_str()[index++] == 'e'
					&& (isWhitespace(c_str()[index]) == true || c_str()[index] == '\0')))
			{
				return true;
			}
		}

		return false;
	}
	 
	short String::toShort() const
		throw(Exception::InvalidFormat)
	{
		if (!isFloat())
		{
			throw Exception::InvalidFormat(__FILE__, __LINE__, string(c_str()));
		}

		errno = 0;
		int i = atoi(c_str());

		if ((errno == ERANGE) || (i < (int)Limits<short>::min()) || (i > (int)Limits<short>::max()))
		{
			errno = 0;
			throw Exception::InvalidFormat(__FILE__, __LINE__, string("out of range: ") + c_str());
		}
		errno = 0;

		return (short)i;
	}
	 
	unsigned short String::toUnsignedShort() const
		throw(Exception::InvalidFormat)
	{
		if (!isFloat())
		{
			throw Exception::InvalidFormat(__FILE__, __LINE__, string(c_str()));
		}

		errno = 0;
		int i = atoi(c_str());

		if ((errno == ERANGE) || (i < (int)0) || (i > (int)Limits<unsigned short>::max()))
		{
			errno = 0;
			throw Exception::InvalidFormat(__FILE__, __LINE__, string("out of range: ") + c_str());
		}
		errno = 0;

		return (unsigned short)i;
	}

	
	int String::toInt() const
		throw(Exception::InvalidFormat)
	{
		if (!isFloat())
		{
			throw Exception::InvalidFormat(__FILE__, __LINE__, string(c_str()));
		}

		errno = 0;
		int i = atoi(c_str());

		if (errno == ERANGE)
		{
			throw Exception::InvalidFormat(__FILE__, __LINE__, string("out of range: ") + string(c_str()));
		}
		
		return i;
	}

	 
	unsigned int String::toUnsignedInt() const
		throw(Exception::InvalidFormat)
	{
		if (!isFloat())
		{
			throw Exception::InvalidFormat(__FILE__, __LINE__, string(c_str()));
		}

		errno = 0;
		unsigned int ui = (unsigned int)strtoul(c_str(), (char **)0, 10);

		if (errno == ERANGE)
		{
			throw Exception::InvalidFormat(__FILE__, __LINE__, string(c_str()));
		}
		
		return ui;
	}

	 
	long String::toLong() const
		throw(Exception::InvalidFormat)	
	{
		if (!isFloat())
		{
			throw Exception::InvalidFormat(__FILE__, __LINE__, string(c_str()));
		}

		errno = 0;
		long l = atol(c_str());

		if (errno == ERANGE)
		{
			throw Exception::InvalidFormat(__FILE__, __LINE__, string(c_str()));
		}
		
		return l;
	}

	 
	unsigned long String::toUnsignedLong() const
		throw(Exception::InvalidFormat)
	{
		if (!isFloat())
		{
			throw Exception::InvalidFormat(__FILE__, __LINE__, string(c_str()));
		}

		errno = 0;
		unsigned long ul = strtoul(c_str(), (char **)0, 10);

		if (errno == ERANGE)
		{
			throw Exception::InvalidFormat(__FILE__, __LINE__, string(c_str()));
		}
		
		return ul;
	}

	 
	float String::toFloat() const
		throw(Exception::InvalidFormat)
	{
		if (!isFloat())
		{
			throw Exception::InvalidFormat(__FILE__, __LINE__, string(c_str()));
		}

		errno = 0;
		float f = (float)atof(c_str());

		if (errno == ERANGE)
		{
			throw Exception::InvalidFormat(__FILE__, __LINE__, string(c_str()));
		}
		
		return f;
	}

	
	double String::toDouble() const
		throw(Exception::InvalidFormat)
	{
		if (!isFloat())
		{
			throw Exception::InvalidFormat(__FILE__, __LINE__, string(c_str()));
		}
		errno = 0;
		double d = atof(c_str());

		if (errno == ERANGE)
		{
			throw Exception::InvalidFormat(__FILE__, __LINE__, string(c_str()));
		}
		
		return d;
	}


	void String::toLower(Index from, Size len)
		throw(Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		validateRange_(from, len);

		Index index = from;
		char* char_ptr = const_cast<char*>(c_str()) + from;

		for (; index < (Index)(from + len); index++, char_ptr++)
		{
			*char_ptr = tolower(*char_ptr);
		}
	}

	void String::toUpper(Index from, Size len)
		throw(Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		validateRange_(from, len);

		Index index = from;
		char *char_ptr = const_cast<char*>(c_str()) + from;

		for(; index < (Index)(from + len); index++, char_ptr++)
		{
			*char_ptr = toupper(*char_ptr);
		}
	}

	Size String::countFields(const char* delimiters) const
		throw(Exception::NullPointer)
	{
		if (delimiters == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}

		Size number_of_fields = 0;

		const char* end_char = &c_str()[size()];
		const char* current_delimiter = 0;
		const char* current_char = c_str();

		while (current_char < end_char)
		{
			for (; current_char < end_char; ++current_char)
			{
				current_delimiter = (char*)strchr(delimiters, *current_char);
				
				if (current_delimiter == 0)
				{
					break;
				}
			}

			if (current_char < end_char)
			{
				++number_of_fields;
				++current_char;
			}

			for (; current_char < end_char; ++current_char)
			{
				current_delimiter = (char*)strchr(delimiters, *current_char);
				
				if (current_delimiter != 0)
				{
					break;
				}
			}
		}

		return number_of_fields;
	}

	Size String::countFieldsQuoted(const char* delimiters, const char* quotes) const
		throw(Exception::NullPointer)
	{
		if ((delimiters == 0) || (quotes == 0))
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}

		Size number_of_fields = 0;

		const char* end_char = &c_str()[size()];
		const char* current_delimiter = 0;
		const char* current_char = c_str();
		const char* current_quote = 0;
		const char* last_quote = 0;

		while (current_char < end_char)
		{
			for (; current_char < end_char; ++current_char)
			{	
				current_quote = (char*)strchr(quotes, *current_char);
				
				if (current_quote != 0)
				{
					if (last_quote == 0)
					{
						last_quote = current_char;
					}
					else
					{
						// if the same quote character is used again,
						// it is a closing quote
						if (*last_quote == *current_quote)
						{
							last_quote = 0;
						}
					}
				}
				
				if ((last_quote == 0) && (current_quote != current_char))
				{
					current_delimiter = (char*)strchr(delimiters, *current_char);
				
					if (current_delimiter == 0)
					{
						break;
					}
				}
			}

			if (current_char < end_char)
			{
				++number_of_fields;
				++current_char;
			}

			for (; current_char < end_char; ++current_char)
			{
				current_quote = (char*)strchr(quotes, *current_char);
				
				if (current_quote != 0)
				{
					if (last_quote == 0)
					{
						last_quote = current_char;
					}
					else
					{
						// if the same quote character is used again,
						// it is a closing quote
						if (*last_quote == *current_quote)
						{
							last_quote = 0;
						}
					}
				}
				
				if (last_quote == 0)
				{
					current_delimiter = (char*)strchr(delimiters, *current_char);
					
					if (current_delimiter != 0)
					{
						break;
					}
				}
			}
		}

		return number_of_fields;
	}

	String String::getField(Index index, const char* delimiters, Index* from_and_next_field) const
		throw(Exception::IndexUnderflow, Exception::NullPointer)
	{
		if ((from_and_next_field != 0) && (*from_and_next_field < 0))
		{
			throw Exception::IndexUnderflow(__FILE__, __LINE__, *from_and_next_field, 0);
		}
		
		if (delimiters == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}

		// allow also negative indices (last field == -1)
		if (index < 0)
		{
			index = (Index)countFields(delimiters) + index;
			if (index < 0)
			{
				throw Exception::IndexUnderflow(__FILE__, __LINE__, index);
			}
		}

		const char *end = &c_str()[size()];
		Index current_index = 0;
		const char *current_delimiter = 0;
		const char *current_char = &c_str()[from_and_next_field == 0 ? 0 : *from_and_next_field];
		const char *field_begin = 0;

		while (current_char < end)
		{
			for (; current_char < end; ++current_char)
			{
				current_delimiter = (char*)strchr(delimiters, *current_char);
				
				if (current_delimiter == 0)
				{
					break;
				}
			}

			if (current_index == index)
			{
				field_begin = current_char;

				for (++current_char; current_char < end; ++current_char)
				{
					current_delimiter = (char*)strchr(delimiters, *current_char);

						if (current_delimiter != 0)
						{
							break;
						}
				}

				if (from_and_next_field != 0)
				{
					if (current_char >= end)
					{
						*from_and_next_field = (Index)EndPos;
					}
					else 
					{
						*from_and_next_field = (Index)(current_char - c_str());
					}
				}

				if (field_begin < end)
				{
					return String(field_begin, 0, (Size)(current_char - field_begin));
				}
			}

			++current_index;

			for (; current_char < end; ++current_char)
			{
				current_delimiter = (char*)strchr(delimiters, *current_char);
				
				if (current_delimiter != 0)
				{
					break;
				}
			}
		}

		if (from_and_next_field != 0)
		{
			*from_and_next_field = (Index)EndPos;
		}

		return String();
	}

	const char* eatDelimiters_(const char* start, const char* end, const char* delimiters)
		throw()
	{
		const char* current_delimiter = (char*)strchr(delimiters, *start);
		while ((current_delimiter != 0) && (start < end))
		{
			start++;
			current_delimiter = (char*)strchr(delimiters, *start);
		}
		
		return start;
	}

	String String::getFieldQuoted(Index index, const char* delimiters, 
																const char* quotes, Index* from_and_next_field) const
		throw(Exception::IndexUnderflow, Exception::NullPointer)
	{
		if ((from_and_next_field != 0) && (*from_and_next_field < 0))
		{
			throw Exception::IndexUnderflow(__FILE__, __LINE__, *from_and_next_field, 0);
		}
		
		if ((delimiters == 0) || (quotes == 0))
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}

		// allow also negative indices (last field == -1)
		if (index < 0)
		{
			index = (Index)countFieldsQuoted(delimiters) + index;
			if (index < 0)
			{
				throw Exception::IndexUnderflow(__FILE__, __LINE__, index);
			}
		}

		String field;
		const char* end = &c_str()[size()];
		Index current_index = -1;
		const char* current_delimiter = 0;
		const char* current_quote = 0;
		const char* last_quote = 0;
		const char* current_char = &c_str()[from_and_next_field == 0 ? 0 : *from_and_next_field];

		do
		{
			current_char = eatDelimiters_(current_char, end, delimiters);
			current_index++;
			
			// we are at the start of a new field
			while (current_char < end)
			{
				current_quote = (char*)strchr(quotes, *current_char);
				if (current_quote != 0)
				{
					if (last_quote != 0)
					{
						// reached the terminating quote
						if (*last_quote == *current_quote)
						{
							last_quote = 0;
						}
						else
						{
							// just another quote character which doesn't matter
							if (index == current_index)
							{
								field += *current_char;
							}
						}
					}
					else
					{
						// this is a new quote
						last_quote = current_quote;
					}
				} 
				else
				{
					if (last_quote == 0)
					{
						current_delimiter = (char*)strchr(delimiters, *current_char);
						if (current_delimiter != 0)
						{
							// we found a delimiter
							// continue with the outer loop -> eat these delimiters!
							break;
						}	
					}
					if (current_index == index)
					{
						field += *current_char;
					}
				}

				current_char++;
			}

			if (current_index == index)
			{
				break;
			}
		}
		while ((current_char < end) && (current_index < index));

		if (from_and_next_field != 0)
		{
			*from_and_next_field = (Index)(current_char - &(c_str()[0]));
			if (current_char >= end)
			{
				*from_and_next_field = (Index)EndPos;
			}
		}

		return field;
	}

	Size String::split(String string_array[], Size array_size, const char* delimiters, Index from) const
		throw(Exception::IndexUnderflow, Exception::NullPointer)
	{
		Size	array_index = 0;

		if (array_size < 1)
		{
			return 0;
		}

		while(from != (Index)EndPos)
		{
			string_array[array_index] = getField(0, delimiters, &from);
			
			if (string_array[array_index] != "")
			{
				array_index++;
			}

			if (array_index == array_size)
			{
				return array_index;
			}
		}

		return array_index; 
	}

	Size String::split(vector<String>& strings, const char* delimiters, Index from) const
		throw(Exception::IndexUnderflow, Exception::NullPointer)
	{
		// clear the vector anyway
		strings.clear();

		while(from != (Index)EndPos)
		{
			String field = getField(0, delimiters, &from);
			
			if (field != "")
			{
				strings.push_back(field);
			}
		}

		return (Size)strings.size(); 
	}

	Size String::splitQuoted(vector<String>& strings, const char* delimiters, const char* quotes, Index from) const
		throw(Exception::IndexUnderflow, Exception::NullPointer)
	{
		// clear the vector anyway
		strings.clear();

		while (from != (Index)EndPos)
		{
			String field = getFieldQuoted(0, delimiters, quotes, &from);
			
			if (field != "")
			{
				strings.push_back(field);
			}
		}

		return (Size)strings.size(); 
	}

	String& String::trimLeft(const char* trimmed_chars)
		throw()
	{
		if ((trimmed_chars == 0) || (size() == 0))
		{
			return *this;
		}

		string::size_type index = find_first_not_of(trimmed_chars);
		
		if (index != string::npos)
		{
			// erase the whitespace characters on the left
			erase(0, index);
		} 
		else 
		{
			// if nothing was found, the string might contain only whitespaces!
			String trimmed(trimmed_chars);
			if (trimmed.has((*this)[0]))
			{
				assign("");
			}
		}

		return *this;
	}

	String& String::trimRight(const char* trimmed_chars)
		throw()
	{
		if (trimmed_chars == 0)
		{
			return *this;
		}

		string::size_type index = find_last_not_of(trimmed_chars);
		
		if (index != string::npos)
		{
			// delete the whitespace characters on the right hand side
			erase(index + 1);
		}
		else 
		{
			// if nothing was found, the string might contain only whitespaces!
			String trimmed(trimmed_chars);
			if (trimmed.has((*this)[size() - 1]))
			{
				assign("");
			}
		}
		
		return *this;
	}

	String operator + (const char* char_ptr, const String& s)
		throw()
	{
		String result(char_ptr);
		result.append(s);
		return result;
	}

	String operator + (char c, const String& s)
		throw()
	{
		String result(c);
		result.append(s);
		return result;
	}

	bool String::hasPrefix(const String& s) const
		throw()
	{
		if (s.size() > size())
		{
			return false;
		}
		if (s.size() == 0)
		{
			return true;
		}

		return (memcmp(c_str(), s.c_str(), s.size()) == 0);
	}

	bool String::hasSuffix(const String& s) const
		throw()
	{
		if (s.size() > size())
		{
			return false;
		}
		if (s.size() == 0)
		{
			return true;
		}

		int result = memcmp(c_str() + size() - s.size(), s.c_str(), s.size());

		return (result == 0);
	}


	#define BALL_STRING_DEFINE_IS_CLASS_METHOD(func, isclass) \
	bool func() const throw()\
	{ \
		const char* end = &c_str()[size()]; \
	 \
		for (const char* ptr = c_str(); ptr < end; ptr++) \
			if (!isclass(*ptr)) \
				return false; \
	 \
		return true; \
	}

	BALL_STRING_DEFINE_IS_CLASS_METHOD(String::isAlpha,  isAlpha)
	BALL_STRING_DEFINE_IS_CLASS_METHOD(String::isAlnum,  isAlnum)
	BALL_STRING_DEFINE_IS_CLASS_METHOD(String::isDigit,  isDigit)
	BALL_STRING_DEFINE_IS_CLASS_METHOD(String::isWhitespace,  isWhitespace)
	BALL_STRING_DEFINE_IS_CLASS_METHOD(String::isSpace,  isSpace)

	#undef BALL_STRING_DEFINE_IS_CLASS_METHOD

	String& String::reverse(Index from, Size len)
		throw(Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		validateRange_(from, len);

		if (len > 1)
		{
			char* forward_ptr = const_cast<char*>(c_str()) + from;
			char* backward_ptr = const_cast<char*>(c_str()) + from + len - 1;
			char temp = 0;

			for (; forward_ptr < backward_ptr; ++forward_ptr, --backward_ptr)
			{
				temp = *forward_ptr;
				*forward_ptr = *backward_ptr;
				*backward_ptr = temp;
			}
		}

		return *this;
	}

	int String::compare(const String& s, Index from, Size len) const
		throw(Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		validateRange_(from, len);

		if ((this == &s) && (from == 0))
		{
			return 0;
		}

		Size newlen = std::min((Size)s.size(), len);

		int result = 0;
		if (compare_mode_ == CASE_INSENSITIVE)
		{
			const char* s1 = c_str() + from;
			const char* s2 = s.c_str();

			for (; newlen > 0; s1++, s2++, newlen--)
			{
				result = tolower(*s1) - tolower(*s2);

				if (result != 0)
				{
					break;
				}
			}
		}
		else 
		{
			result = strncmp(c_str() + from, s.c_str(), newlen);
		}

		if ((result == 0) && (len != newlen))
		{
			result = (int)len - (int)s.size();
		}
			
		return result;
	}

	int String::compare(const String& s, Index from) const
		throw(Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		validateIndex_(from);

		if ((this == &s) && (from == 0))
		{
			return 0;
		}

		Size len = (Size)(size() - from);
		Size newlen = std::min((Size)s.size(), len);

		int result = 0;
		if (compare_mode_ == CASE_INSENSITIVE)
		{
			const char* s1 = c_str() + from;
			const char* s2 = s.c_str();

			for (; newlen > 0; s1++, s2++, newlen--)
			{
				result = tolower(*s1) - tolower(*s2);

				if (result != 0)
				{
					break;
				}
			}
			
		}
		else 
		{
			result = strncmp(c_str() + from, s.c_str(), newlen);
		}

		if (result == 0)
		{
			result = (int)len - (int)s.size();
		}
		
		return result;
	}

	int String::compare(const char* char_ptr, Index from, Size len) const
		throw(Exception::NullPointer, Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		if (char_ptr == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}
		
		validateRange_(from, len);

		if ((c_str() + from) == char_ptr)
		{
			return 0;
		}
	
		Size newlen = (Size)strlen(char_ptr);
		
		newlen = std::min(len, newlen);

		int result = 0;
		if (compare_mode_ == CASE_INSENSITIVE)
		{
			const char* ptr1 = c_str() + from;
			const char* ptr2 = char_ptr;

			for (; newlen > 0; ptr1++, ptr2++)
			{
				result = tolower(*ptr1) - tolower(*ptr2);

				if (result != 0)
				{
					break;
				}
			}
			
		}
		else 
		{
			result = strncmp(c_str() + from, char_ptr, newlen);
		}

		if ((result == 0) && (len != newlen))
		{
			result = (int)size() - (int)from - (int)strlen(char_ptr);
		}

		return result;
	}

	int String::compare(const char* char_ptr, Index from) const
		throw(Exception::NullPointer, Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		if (char_ptr == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}

		// indices may be given as negative arguments: start from the end
		// -1 therefore means the last bit.
		Size string_size = (Size)size();
		if (from < 0)
		{
			from = (Index)string_size + from;

			// if the value is out of bounds - throw an exception
			// and leave it...
			if (from < 0)
			{
				throw Exception::IndexUnderflow(__FILE__, __LINE__, from, string_size);
			}
		}

		if ((Size)from > string_size)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, from, string_size);
		}
		
		Size len = string_size - from;

		if ((c_str() + from) == char_ptr)
		{
			return 0;
		}
	
		Size newlen = std::min((Size)strlen(char_ptr), len);

		newlen = len;
		
		int result = 0;

		if (compare_mode_ == CASE_INSENSITIVE)
		{
			const char* ptr1 = c_str() + from;
			const char* ptr2 = char_ptr;

			for (; newlen > 0; ptr1++, ptr2++)
			{
				result = tolower(*ptr1) - tolower(*ptr2);

				if (result != 0)
				{
					break;
				}
			}
			
		} 
		else 
		{
			result = strncmp(c_str() + from, char_ptr, newlen);
		}

		if ((result == 0) && (len == newlen))
		{
			return (int)string_size - (int)from - (int)strlen(char_ptr);
		}

		return result;
	}

	istream& getline(istream& s, String& str, char delimiter)
		throw()
	{
		char c;
		
		str.destroy();

		while (s.get(c)) 
		{
			if (c == delimiter) 
			{
				break;
			}
			str.append(1, c);
		}

		return s;
	}

	void String::dump(ostream &s, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_DEPTH(s, depth);
		s << "  capacity: " << capacity() << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  size: " << size() << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  string: ";
		
		const char* end_ptr = &c_str()[size()];

		for (const char *char_ptr = c_str(); char_ptr < end_ptr; char_ptr++)
			s << *char_ptr;

		s << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	Size String::substitute(const String& to_replace, const String& replacing)
		throw()
	{
		Size replaced_size = (Size)to_replace.size();

		string::size_type found = 0;
		if (to_replace != "")
		{
			found = find(to_replace);
		}

		if (found != string::npos)
		{
			replace(found, replaced_size, replacing);
		}

		return ((found == string::npos) ? EndPos : (Size)found);
	}
 

	void String::validateIndex_(Index& index) const
		throw (Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		// indices may be given as negative arguments: start from the end
		// -1 therefore means the last bit.
		Size string_size = (Size)size();
		if (index < 0)
		{
			index = (Index)string_size + index;

			// if the value is out of bounds - throw an exception
			// and leave it...
			if (index < 0)
			{
				throw Exception::IndexUnderflow(__FILE__, __LINE__, index, string_size);
			}
		}

		if ((Size)index > string_size)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, index, string_size);
		}
	}

	void String::validateRange_(Index& from, Size& len) const
		throw (Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		Size string_size = (Size)size();
		
    // indices may be given as negative arguments: start from the end
    // -1 therefore means the last character of the string.
    if (from < 0)
		{
      from = (Index)string_size + from;

			// if the values are out of bounds - throw an exception
			// and leave it...
			if (from < 0)
			{
				throw Exception::IndexUnderflow(__FILE__, __LINE__, from, string_size);
			}
		}

    if (((Size)from > string_size) || ((string_size > 0) && ((Size)from == string_size) && (len != 0)))
		{
      throw Exception::IndexOverflow(__FILE__, __LINE__, from, string_size);
		}

		if (len == EndPos)
		{
			len = string_size - from;
		}
		
		if (len > (string_size - from))
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, (Index)len, string_size);
		}
 	}

	void Substring::validateRange_(Index& from, Size& len) const
		throw(Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		Size size = to_ - from_ + 1;

    // indices may be given as negative arguments: start from the end
    // -1 therefore means the to bit.
    if (from < 0)
		{
      from = (Index)size + from;

			// if the values are out of bounds - throw an exception
			// and leave it...
			if (from < 0)
			{
				throw Exception::IndexUnderflow(__FILE__, __LINE__, from, size);
			}
		}

    if (((Size)from > size) || ((size > 0) && ((Size)from == size)))
		{
      throw Exception::IndexOverflow(__FILE__, __LINE__, from, size);
		}

		if (len == String::EndPos)
		{
			len = size - from;
		}
		
		if (len > (size - from))
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, (Index)len, size);
		}
 	}

	void String::validateCharPtrRange_(Index& from, Size& len, const char* char_ptr)
		throw(Exception::NullPointer, Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		if (char_ptr == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}

		Size total_len = (Size)strlen(char_ptr);

    // indices may be given as negative arguments: start from the end
    // -1 therefore means the to bit.
    if (from < 0)
		{
      from = (Index)total_len + from;

			// if the values are out of bounds - throw an exception
			// and leave it...
			if (from < 0)	
			{
				throw Exception::IndexUnderflow(__FILE__, __LINE__, from, len);
			}
		}

    if (((Size)from > total_len) || ((total_len > 0) && ((Size)from == total_len)))
		{
      throw Exception::IndexOverflow(__FILE__, __LINE__, from, len);
		}

		if (len == EndPos)
		{
			len = total_len - from;
		}
		
		if (len > (total_len - from))
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, (Index)len, total_len);
		}
 	}

	bool Substring::operator == (const char* char_ptr) const
		throw(Substring::UnboundSubstring, Exception::NullPointer)
	{
		if (bound_ == 0)
		{
			throw UnboundSubstring(__FILE__, __LINE__);
		}

		if (char_ptr == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}

		// to prevent compiler warning:
		if ((signed)strlen(char_ptr) != (to_ - from_ +1))
		{
			return false;
		}
		return (bound_->compare(char_ptr, from_, to_ - from_ + 1) == 0);
	}


	bool Substring::operator != (const char* char_ptr) const
		throw(Substring::UnboundSubstring, Exception::NullPointer)
	{
		if (bound_ == 0)
		{
			throw UnboundSubstring(__FILE__, __LINE__);
		}
		if (char_ptr == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}
		// to prevent compiler warning:
		if ((signed)strlen(char_ptr) != (to_ - from_ +1))
		{
			return true;
		}
		return (bound_->compare(char_ptr, from_, to_ - from_ + 1) != 0);
	}


	Substring& Substring::bind(const Substring& s, Index from, Size len)
		throw(Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		s.validateRange_(from, len);

		bound_ = s.bound_;
		from 	+= s.from_;
		to_ 	 = s.from_ + (Index)len - 1;
		from_  = from;

		return *this;
	}


	void Substring::set(const char* char_ptr, Size size)
		throw(Substring::UnboundSubstring, Exception::NullPointer, Exception::SizeUnderflow)
	{
		if (bound_ == 0)
		{
			throw UnboundSubstring(__FILE__, __LINE__);
		}
		if (char_ptr == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}
		if (size == 0)
		{
			throw Exception::SizeUnderflow(__FILE__, __LINE__);
		}

		if (size == String::EndPos)
		{
			bound_->replace(from_, to_ - from_ + 1, string(char_ptr));
		}
		else
		{
			bound_->replace(from_, to_ - from_ + 1, char_ptr, size);
		}
	}


	bool Substring::operator == (const Substring& s) const
		throw(Substring::UnboundSubstring)
	{
		if (bound_ == 0 || s.bound_ == 0)
		{
			throw UnboundSubstring(__FILE__, __LINE__);
		}
		if ((s.to_ - s.from_) != (to_ - from_))
		{
			return false;
		}
		return (bound_->compare(s.c_str() + from_, from_, to_ - from_ + 1) == 0);
	}


	bool Substring::operator != (const Substring& s) const
		throw(Substring::UnboundSubstring)
	{
		if (bound_ == 0 || s.bound_ == 0)
		{
			throw UnboundSubstring(__FILE__, __LINE__);
		} 
		if ((s.to_ - s.from_) != (to_ - from_))
		{
			return true;
		}
		return (bound_->compare(s.c_str() + from_, from_, to_ - from_ + 1) != 0);
	}


	Substring String::before(const String& s, Index from) const
		throw()
	{
		Position found = EndPos;
		if (s != "")
		{
			found = (Position)find(s, from);
		}

		if (found == 0 || found == EndPos) 
		{
			return Substring(*this, 0, 0);
		}

		return getSubstring(0, found);
	}


	Substring String::through (const String& s, Index from) const
		throw()
	{
		Position found = EndPos;
		if (s != "")
		{
			found = (Position)find(s, from); 
		}

		if (found == EndPos) 
		{
			return Substring(*this, 0, 0);
		}

		return getSubstring(0, found + (Size)s.size());
	}


	Substring String::from(const String& s, Index from) const
		throw()
	{
		if (s == "")
		{
			return getSubstring(0);
		}

		Size found = (Size)find(s, from); 

		if (found == EndPos)
		{
			return Substring(*this, -1, 0);
		}
		
		return getSubstring((Index)found);
	}


	Substring String::after(const String& s, Index from) const
		throw()
	{
		if (s == "")
		{
			return getSubstring(0);
		}

		Position found = (Position)find(s, from); 

		if ((found == EndPos) || (found + s.size() >= size()))
		{
			return Substring(*this, -1, 0);
		}

		return getSubstring((Index)found + (Index)s.size());
	}


	Substring String::right(Size len) const
		throw()
	{
		// to save calls to size()
		Size s = (Size)size();
		if (len > s) 
		{
			len = s;
		}

		Index from = (Index)s - (Index)len;

		if (len > 0) 
		{
			from = (Index)s - (Index)len;
		}
		else 
		{
			if (s > 0)
			{
				from = (Index)s - 1;
			} 
			else 
			{
				from = 0;
			}
		}
		
		return Substring(*this, from, len);
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/DATATYPE/string.iC>
#	endif

} // namespace BALL
