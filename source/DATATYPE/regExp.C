// $Id: regExp.C,v 1.5 2000/05/23 14:16:41 oliver Exp $ 

#include <BALL/DATATYPE/regExp.h>

using std::endl;
using std::istream;
using std::ostream;

namespace BALL 
{

	const String RegularExpression::ALPHA("^[:alpha:]$"); // "[A-Za-z]+"
	const String RegularExpression::ALPHANUMERIC("^[:alnum:]$"); // "[0-9A-Za-z]+"
	const String RegularExpression::REAL("^-?(([0-9]+\\.[0-9]*)|([0-9]+)|(\\.[0-9]+))([eE][---+]?[0-9]+)?$");
	const String RegularExpression::IDENTIFIER("^[A-Za-z_][A-Za-z0-9_]*$");
	const String RegularExpression::INTEGER("^-?[:digit:]$"); // "-?[0-9]+"
	const String RegularExpression::HEXADECIMAL_INTEGER("^-?(0x|0X|)[:xdigit:]$");
	const String RegularExpression::LOWERCASE("^[:lower:]$"); // "[a-z]+"
	const String RegularExpression::NON_ALPHA("^[^A-Za-z]+$");
	const String RegularExpression::NON_ALPHANUMERIC("^[^0-9A-Za-z]+$");
	const String RegularExpression::NON_NUMERIC("^[^0-9]+$");
	const String RegularExpression::NON_WHITESPACE("^[^ \n\t\r\f\v]+$");
	const String RegularExpression::UPPERCASE("^[:upper:]$"); // "[A-Z]+"
	const String RegularExpression::WHITESPACE("^[ \n\t\r\f\v]+$");



	RegularExpression::RegularExpression()
		:	pattern_(BALL_REGULAR_EXPRESSION_DEFAULT_PATTERN),
			valid_pattern_(false)
	{
		compilePattern_();
	}

	RegularExpression::RegularExpression(const RegularExpression &regular_expression,bool deep)
		:	pattern_(regular_expression.pattern_),
			valid_pattern_(false)
	{
		BALL_PRECONDITION
			(deep == true, 
			 BALL_REGULAR_EXPRESSION_ERROR_HANDLER(RegularExpression::ERROR__NOT_IMPLEMENTED));

		compilePattern_();
	}

	RegularExpression::RegularExpression(const String& pattern, bool wildcard_pattern)
		:	pattern_(pattern),
			valid_pattern_(false)
	{
		if (wildcard_pattern == true)
		{
			toExtendedRegularExpression_();
		}

		compilePattern_();
	}

	RegularExpression::~RegularExpression()
	{
		freeCompiledPattern_();
	}

	bool RegularExpression::match
		(const char* text, const char* pattern,
		 int compile_flags, int execute_flags)
	{
		BALL_PRECONDITION
			(text != 0, 
			 BALL_REGULAR_EXPRESSION_STATIC_ERROR_HANDLER(RegularExpression::ERROR__POINTER_IS_NULL));

		BALL_PRECONDITION
			(pattern != 0, 
			 BALL_REGULAR_EXPRESSION_STATIC_ERROR_HANDLER(RegularExpression::ERROR__POINTER_IS_NULL));

		regex_t regex;

		if (regcomp(&regex, pattern, compile_flags) != 0)
		{
			return false;
		}

		int status = regexec(&regex, text, (size_t)0, 0, execute_flags);

		regfree(&regex);

		return (bool)(status == 0);
	}

	bool RegularExpression::match(const String& text, Index from, int execute_flags) const
	{
		if (valid_pattern_ == false)
		{
			return false;
		}

		BALL_PRECONDITION
			(from >= 0, 
			 BALL_REGULAR_EXPRESSION_ERROR_HANDLER(RegularExpression::ERROR__INDEX_UNDERFLOW));

		BALL_PRECONDITION
			(from <= (Index)text.size(), 
			 BALL_REGULAR_EXPRESSION_ERROR_HANDLER(RegularExpression::ERROR__INDEX_OVERFLOW));

		return (bool)(regexec(&regex_, text.c_str() + from, (size_t)0, 0, execute_flags) == 0);
	}

	bool RegularExpression::match
		(const Substring& text,
		 Index from, int execute_flags) const
	{
		if (valid_pattern_ == false)
		{
			return false;
		}

		BALL_PRECONDITION
			(text.isValid() == true,
			 BALL_REGULAR_EXPRESSION_ERROR_HANDLER(RegularExpression::ERROR__INVALID_SUBSTRING));

		BALL_PRECONDITION
			(from >= 0, 
			 BALL_REGULAR_EXPRESSION_ERROR_HANDLER(RegularExpression::ERROR__INDEX_UNDERFLOW));

		BALL_PRECONDITION
			(from < (Index)text.size(), 
			 BALL_REGULAR_EXPRESSION_ERROR_HANDLER(RegularExpression::ERROR__INDEX_OVERFLOW));

		char* end_of_substring = (char *)(text.c_str() + text.size());
		char c = *end_of_substring;
		*end_of_substring = '\0';
		int status = regexec(&regex_, text.c_str() + from, (size_t)0, 0, execute_flags);
		*end_of_substring = c;

		return (bool)(status == 0);
	}

	bool RegularExpression::match(const char* text, int execute_flags) const
	{
		if (valid_pattern_ == false)
		{
			return false;
		}

		BALL_PRECONDITION
			(text != 0, 
			 BALL_REGULAR_EXPRESSION_ERROR_HANDLER(RegularExpression::ERROR__POINTER_IS_NULL));

		return (bool)(regexec(&regex_, text, (size_t)0, 0, execute_flags) == 0);
	}

	bool RegularExpression::find
		(const char* text,
		 const char* pattern,
		 const char** found_substring_from,
		 const char** found_substring_to,
		 int compile_flags,
		 int execute_flags)
	{
		if (text == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}
		if (pattern == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}

		regex_t regex;

		if (regcomp(&regex, pattern, compile_flags) != 0)
		{
			return false;
		}

		regmatch_t regmatch[1];
		
		if (regexec(&regex, text, (size_t)1, regmatch, execute_flags) == 0)
		{
			if (found_substring_from != 0)
			{
				*found_substring_from = text + regmatch[0].rm_so;
			}

			if (found_substring_to != 0)
			{
				*found_substring_to = text + regmatch[0].rm_eo - 1;
			}

			regfree(&regex);

			return true;
		} else {
			regfree(&regex);

			return false;
		}
	}

	bool RegularExpression::find
		(const String& text, Substring& found,
		 Index from, int execute_flags) const
	{
		if (valid_pattern_ == false)
		{
			return false;
		}
		if (from < 0)
		{
			throw Exception::IndexUnderflow(__FILE__, __LINE__, from, 0);
		}
		if (from >= (Index)text.size())
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, from, text.size());
		}

		regmatch_t regmatch[20];

		if (regexec(&regex_, text.c_str() + from, (size_t)20, regmatch, execute_flags) == 0)
		{
			found.bind
				(text, from + (Index)regmatch[0].rm_so, 
				 (Index)(regmatch[0].rm_eo - (Index)regmatch[0].rm_so));

			return true;
		}
		else
		{
			found.unbind();

			return false;
		}
	}

	bool RegularExpression::find
		(const String& text,
		 Substring found_subexpression[],
		 Size number_of_subexpresions,
		 Index from,
		 int execute_flags) const
	{
		if (valid_pattern_ == false)
		{
			return false;
		}
		if (from < 0)
		{
			throw Exception::IndexUnderflow(__FILE__, __LINE__, from, 0);
		}
		if (from >= (Index)text.size())
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, from, text.size());
		}


		for (Index index = 0; index < (Index)number_of_subexpresions; ++index)
		{
			found_subexpression[index].unbind();
		}

		number_of_subexpresions = BALL_MIN(number_of_subexpresions, (Size)regex_.re_nsub + 1);

		regmatch_t *regmatch_ptr = new regmatch_t[number_of_subexpresions];

		if (regexec(&regex_, text.c_str() + from, (size_t)number_of_subexpresions, regmatch_ptr, execute_flags) == 0)
		{
			for (Index index = 0; index < (Index)number_of_subexpresions; ++index)
			{
				found_subexpression[index].bind
					(text, from + (Index)regmatch_ptr[index].rm_so, 
					 (Index)(regmatch_ptr[index].rm_eo - (Index)regmatch_ptr[index].rm_so));
			}

			delete [] regmatch_ptr;
			
			return true;
		}
		else
		{
			delete [] regmatch_ptr;

			return false;
		}
	}

	bool RegularExpression::find
		(const Substring &text,
		 Substring &found,
		 Index from,
		 int execute_flags) const
	{
		if (valid_pattern_ == false)
		{
			return false;
		}
		if (!text.isValid())
		{
			throw Substring::UnboundSubstring(__FILE__, __LINE__);
		}
		if (from < 0)
		{
			throw Exception::IndexUnderflow(__FILE__, __LINE__, from, 0);
		}
		if (from >= (Index)text.size())
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, from, text.size());
		}

		regmatch_t regmatch[1];
		char* end_of_substring = (char*)(text.c_str() + text.size());
		char c = *end_of_substring;
		*end_of_substring = '\0';

		if (regexec(&regex_, text.c_str() + from, (size_t)1, regmatch, execute_flags) == 0)
		{
			found.bind
				(text, from + (Index)regmatch[0].rm_so,
				 (Index)regmatch[0].rm_eo - (Index)regmatch[0].rm_so);

			*end_of_substring = c;

			return true;
		}
		else
		{
			found.unbind();

			*end_of_substring = c;

			return false;
		}
	}

	bool RegularExpression::find
		(const Substring &text,
		 Substring found_subexpression[],
		 Size number_of_subexpresions,
		 Index from,
		 int execute_flags) const
	{
		if (valid_pattern_ == false)
		{
			return false;
		}
		if (!text.isValid())
		{
			throw Substring::UnboundSubstring(__FILE__, __LINE__);
		}
		if (from < 0)
		{
			throw Exception::IndexUnderflow(__FILE__, __LINE__, from, 0);
		}
		if (from >= (Index)text.size())
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, from, text.size());
		}

		for (Index index = 0; index < (Index)number_of_subexpresions; ++index)
		{
			found_subexpression[index].unbind();
		}

		number_of_subexpresions = BALL_MIN(number_of_subexpresions, (Size)regex_.re_nsub + 1);

		regmatch_t *regmatch_ptr = new regmatch_t[number_of_subexpresions];
		char *end_of_substring = (char *)(text.c_str() + text.size());
		char c = *end_of_substring;
		*end_of_substring = '\0';

		if (regexec(&regex_, text.c_str() + from, (size_t)number_of_subexpresions, regmatch_ptr, execute_flags) == 0)
		{
			for (Index index = 0; index < (Index)number_of_subexpresions;++index)
		 
			{
				found_subexpression[index].bind
					(text,  from	+ (Index)regmatch_ptr[index].rm_so,
					 (Index)regmatch_ptr[index].rm_eo - (Index)regmatch_ptr[index].rm_so);
			}

			*end_of_substring = c;
			delete [] regmatch_ptr;

			return true;
		}
		else
		{
			*end_of_substring = c;
			delete [] regmatch_ptr;
			
			return false;
		}
	}

	bool RegularExpression::find
		(const char* text,
		 const char** found_substring_from,
		 const char** found_substring_to,
		 int execute_flags) const
	{
		if (valid_pattern_ == false)
		{
			return false;
		}
		if (text == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}
		
		regmatch_t regmatch[1];
		
		if (regexec(&regex_, text, (size_t)1, regmatch, execute_flags) == 0)
		{
			if (found_substring_from != 0)
			{
				*found_substring_from = text + regmatch[0].rm_so;
			}

			if (found_substring_to != 0)
			{
				*found_substring_to = text + regmatch[0].rm_eo - 1;
			}

			return true;
		}
		else
		{
			return false;
		}
	}

	void RegularExpression::dump
		(ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_DEPTH(s, depth);

		BALL_DUMP_DEPTH(s, depth);
		s << "  pattern: " << pattern_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  is valid: " << valid_pattern_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  compiled subexpressions: " << countSubexpressions() << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	ostream& operator << (ostream& s, const RegularExpression& regular_expression)
	{
		s << regular_expression.pattern_ << ' ';

		return s;
	}

	istream& operator >> (istream& s, RegularExpression& regular_expression)
	{
		String pattern;
		
		s >> pattern;

		regular_expression.set(pattern);

		return s;
	}

	void RegularExpression::compilePattern_()
	{
		valid_pattern_ = (bool)!::regcomp(&regex_, pattern_.c_str(), REG_EXTENDED);
	}

	const char* RegularExpression::getSystemErrorMessage_(int error_code, regex_t& regex)
	{
		size_t buffer_size = regerror(error_code, &regex, (char *)0, (size_t)0);

		if (buffer_size == 0)
		{
			return "Unknown error.";
		} else {
			static char* buffer = new char[buffer_size];
			regerror(error_code, &regex, buffer, buffer_size);

			return buffer;
		}
	}

	void RegularExpression::freeCompiledPattern_()
	{
		::regfree(&regex_);
	}

	void RegularExpression::toExtendedRegularExpression_()
	{
		const char* pattern = pattern_.c_str();
		String regexp;

		for (; *pattern != '\0'; ++pattern) 
		{
			switch(*pattern)
			{
				case '*': regexp += ".*";  break;
				case '?': regexp += '.';   break;
				case '.': regexp += "\\."; break;
				default:  regexp += *pattern;
			}
		}

		regexp.swap(pattern_);
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/DATATYPE/regExp.iC>
#	endif

} // namespace BALL
