// $Id: string.C,v 1.17 2000/06/27 07:46:23 oliver Exp $

#include <BALL/DATATYPE/string.h>

#include <stdio.h>
#include <stdarg.h>

#include <algorithm>


using std::ostream;
using std::istream;
using std::strstream;
using std::endl;
using std::ends;

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

	String::CompareMode String::compare_mode_ = String::CASE_SENSITIVE;

	Substring::UnboundSubstring::UnboundSubstring(const char* file, int line)
		:	Exception::GeneralException(file, line)
	{
	}

	Substring::Substring()
		:	bound_(0),
			from_((Index)string::npos),
			to_((Index)string::npos)
	{
	}

	Substring::Substring(const Substring& substring, bool /* deep */)
		:	bound_(substring.bound_),
			from_(substring.from_),
			to_(substring.to_)
	{
	}

	Substring::Substring(const String& s, Index from, Size len)
	{
		s.validateRange_(from, len);

		bound_	= (String *)& s;
		from_		= from;
		to_			= from + (Index)len - 1;
	}

	Substring::~Substring()
	{
	}

	void Substring::dump(ostream& s, Size depth) const
	{
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

	bool operator != (const String& s, const Substring& substring)
	{
		return (substring != s);
	}

	bool operator == (const String& s, const Substring& substring)
	{
		return (substring == s);
	}

	// hand-coded create method
	void* String::create(bool /* deep */, bool empty) const
	{
		void* ptr;
		if (empty == true)
		{
			ptr = (void*)new String;
		} else {
			ptr = (void*)new String(*this);
		}

		return ptr;
	}
 
	String::String()
		: string()
	{
	}

	String::String(const String& s)
		: string(s.c_str())
	{
	}

	String::String(const char* char_ptr, Index from, Size len)
		: string()
	{
		validateCharPtrRange_(from, len, char_ptr);
		if (len > 0)
		{
			assign(char_ptr + from, len);
		}
	}
	 
	String::String(const unsigned char c)
		: string(1, (char)c)
	{
	}

	String::String(const char c, Size len)
		: string(len, c)
	{
	}

	String::String(const string& s)
		: string(s)
	{
	}

	String::String(const String& s, Index from, Size len)
		: string("")
	{
		s.validateRange_(from, len);
		if (len > 0)
		{
			assign(s.c_str() + from, len);
		}
	}

	String::String(Size buffer_size, const char* format, ... )
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
		: string("")
	{
		s << ends;
		char* str = s.str();
		assign(str);
	}

#	define BALL_STRING_DEFINE_CONSTRUCTOR_METHOD(type, format_string) \
	String::String(type t) \
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
	{
		erase();
	}

	void String::set(const String& s, Index from, Size len)
	{
		s.validateRange_(from, len);

		if (len == 0)
		{
			erase();
		} else {
			assign(s.c_str() + from, len);
		}
	}

	void String::set(const char* s, Index from, Size len)
	{
		validateCharPtrRange_(from, len, s);

		if (len == 0)
		{
			erase();
		} else {
			assign(s + from, len);
		}
	}

	void String::set(Size buffer_size, const char *format, ... )
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

	void String::set(strstream& s)
	{
		s << ends;

		char* char_ptr = s.str();
		assign(char_ptr);
	}

	void String::set(char c, Size len)
	{
		assign(len, c);
	}

	void String::set(unsigned char c)
	{
		assign(1, (char)c);
	}

#	define BALL_STRING_DEFINE_SET_METHOD(type, format_string) \
	void String::set(type t) \
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
	{
		validateIndex_(from);

		if (max_len == 0)
		{
			return;
		}

		Size len = std::min(max_len, size() - from);

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
	{
		Size index = find_first_not_of(CHARACTER_CLASS__WHITESPACE);
		
		if (size() == 0)
			return true;

		if (index != npos)
		{
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

	void String::toLower(Index from, Size len)
	{
		validateRange_(from, len);

		Index index = from;
		char* char_ptr = const_cast<char*>(c_str()) + from;

		for (; index < (Index)(from + len); index++, char_ptr++)
			*char_ptr = tolower(*char_ptr);
	}

	void String::toUpper(Index from, Size len)
	{
		validateRange_(from, len);

		Index index = from;
		char *char_ptr = const_cast<char*>(c_str()) + from;

		for(; index < (Index)(from + len); index++, char_ptr++)
			*char_ptr = toupper(*char_ptr);
	}

	Size String::countFields(const char* delimiters) const
	{
		if (delimiters == 0)
			throw Exception::NullPointer(__FILE__, __LINE__);

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
					break;
			}

			if (current_char < end_char)
				++number_of_fields;

			for (; current_char < end_char; ++current_char)
			{
				current_delimiter = (char*)strchr(delimiters, *current_char);
				
				if (current_delimiter != 0)
					break;
			}
		}

		return number_of_fields;
	}

	String String::getField(Index index, const char* delimiters, Index* from_and_next_field) const
	{
		BALL_PRECONDITION
			(from_and_next_field == 0
			 || *from_and_next_field >= 0, 
			 BALL_STRING_ERROR_HANDLER(ERROR__INDEX_UNDERFLOW));

		BALL_PRECONDITION
			(delimiters != 0, 
			 BALL_STRING_ERROR_HANDLER(ERROR__POINTER_IS_NULL));

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
						*from_and_next_field = (Index)npos;
					} else {
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
			*from_and_next_field = (Index)npos;
		}

		return String();
	}

	Size String::split(String string_array[], Size array_size, const char* delimiters, Index from) const
	{
		Size	array_index = 0;

		if (array_size < 1)
		{
			return 0;
		}

		while(from != (Index)npos)
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

	String& String::trimLeft(const char* trimmed_chars)
	{
		if (trimmed_chars == 0)
		{
			return *this;
		}

		Index index = (Index)find_first_not_of(trimmed_chars);
		
		if (index > 0)
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
	{
		if (trimmed_chars == 0)
		{
			return *this;
		}

		Size index = find_last_not_of(trimmed_chars);
		
		if (index < (size() - 1))
		{
			// delete the whitespace characters on the right hand side
			erase(index + 1);
		} else {
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
	{
		String result(char_ptr);

		result.append(s);

		return result;
	}

	String operator + (char c, const String& s)
	{
		String result(c);

		result.append(s);

		return result;
	}

	void String::swap(String& s)
	{
		string::swap(s);
	}

	bool String::hasPrefix(const String& s) const
	{
		if (s.size() > size())
			return false;

		if (s.size() == 0)
			return true;

		int result = 0;

		result = memcmp(c_str(), s.c_str(), s.size());

		return (bool)(result == 0);
	}

	bool String::hasSuffix(const String& s) const
	{
		if (s.size() > size())
			return false;

		if (s.size() == 0)
			return true;

		int result = memcmp(c_str() + size() - s.size(), s.c_str(), s.size());

		return (bool)(result == 0);
	}


	#define BALL_STRING_DEFINE_IS_CLASS_METHOD(func, isclass) \
	bool func() const \
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
	{
		validateRange_(from, len);

		if ((this == &s) && (from == 0))
		{
			return 0;
		}

		Size newlen = std::min(s.size(), len);

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
		} else {
			result = strncmp(c_str() + from, s.c_str(), newlen);
		}

		if ((result == 0) && (len != newlen))
		{
			result = (int)len - (int)s.size();
		}
			
		return result;
	}

	int String::compare(const String& s, Index from) const
	{
		validateIndex_(from);

		if ((this == &s) && (from == 0))
		{
			return 0;
		}

		Size len = size() - from;
		Size newlen = std::min(s.size(), len);

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
			
		} else {
			result = strncmp(c_str() + from, s.c_str(), newlen);
		}

		if (result == 0)
		{
			result = (int)len - (int)s.size();
		}
		
		return result;
	}

	int String::compare(const char* char_ptr, Index from, Size len) const
	{
		if (char_ptr == 0)
			throw Exception::NullPointer(__FILE__, __LINE__);
		
		validateRange_(from, len);

		if ((c_str() + from) == char_ptr)
			return 0;
	
		Size newlen = strlen(char_ptr);
		
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
			
		} else {

			result = strncmp(c_str() + from, char_ptr, newlen);
		}

		if ((result == 0) && (len != newlen))
		{
			result = (int)size() - (int)from - (int)strlen(char_ptr);
		}

		return result;
	}

	int String::compare(const char* char_ptr, Index from) const
	{
		if (char_ptr == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}

		// indices may be given as negative arguments: start from the end
		// -1 therefore means the last bit.
		Size string_size = size();
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
	
		Size newlen = std::min(strlen(char_ptr), len);

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
			
		} else {
			result = strncmp(c_str() + from, char_ptr, newlen);
		}

		if ((result == 0) && (len == newlen))
		{
			return (int)string_size - (int)from - (int)strlen(char_ptr);
		}

		return result;
	}

	bool operator == (const char *char_ptr, const String &s)
	{
		return (bool)(s.compare(char_ptr) == 0);
	}

	bool operator != (const char* char_ptr, const String &s)
	{
		return (bool)(s.compare(char_ptr) != 0);
	}

	bool operator < (const char* char_ptr, const String& s)
	{
		return (bool)(s.compare(char_ptr) > 0);
	}

	bool operator <= (const char* char_ptr, const String &s)
	{
		return (bool)(s.compare(char_ptr) >= 0);
	}

	bool operator >= (const char* char_ptr, const String& s)
	{
		return (bool)(s.compare(char_ptr) <= 0);
	}

	bool operator > (const char *char_ptr, const String& s)
	{
		return (bool)(s.compare(char_ptr) < 0);
	}

	bool operator == (char c, const String& s)
	{
		return (bool)(s.compare(c) == 0);
	}

	bool operator != (char c, const String &s)
	{
		return (bool)(s.compare(c) != 0);
	}

	bool operator < (char c, const String& s)
	{
		return (bool)(s.compare(c) > 0);
	}

	bool operator <= (char c, const String &s)
	{
		return (bool)(s.compare(c) >= 0);
	}

	bool operator >= (char c, const String &s)
	{
		return (bool)(s.compare(c) <= 0);
	}

	bool operator > (char c, const String &s)
	{
		return (bool)(s.compare(c) < 0);
	}

	istream& getline(istream& s, String& str, char delimiter)
	{
		char c;
		
		str.destroy();

		while (s.get(c)) 
		{
			if (c == delimiter) 
				break;

			str.append(1, c);
		}

		return s;
	}

	void String::dump(ostream &s, Size depth) const
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

	Index String::substitute(const String& to_replace, const String& replacing)
	{
		Size replaced_size = to_replace.size();

		Index found = 0;
		if (to_replace != "")
		{
			found = (Index)find(to_replace);
		}

		if (found != (Index)npos)
		{
			replace(found, replaced_size, replacing);
		}

		return found;
	}
 

	void String::validateIndex_(Index& index) const
	{
		// indices may be given as negative arguments: start from the end
		// -1 therefore means the last bit.
		Size string_size = size();
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
	{
		Size string_size = size();
		
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

		if (len == npos)
		{
			len = string_size - from;
		}
		
		if (len > (string_size - from))
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, (Index)len, string_size);
		}
 	}

	void Substring::validateRange_(Index& from, Size& len) const
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

		if (len == string::npos)
		{
			len = size - from;
		}
		
		if (len > (size - from))
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, (Index)len, size);
		}
 	}

	void String::validateCharPtrRange_(Index& from, Size& len, const char* char_ptr)
	{
		if (char_ptr == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}

		Size total_len = strlen(char_ptr);

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

		if (len == npos)
		{
			len = total_len - from;
		}
		
		if (len > (total_len - from))
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, (Index)len, total_len);
		}
 	}


#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/DATATYPE/string.iC>
#	endif

} // namespace BALL
