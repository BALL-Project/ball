// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_DATATYPE_STRING_H
#define BALL_DATATYPE_STRING_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif
#ifndef BALL_COMMON_GLOBAL_H
#	include <BALL/COMMON/global.h>
#endif
#ifndef BALL_COMMON_CREATE_H
#	include <BALL/COMMON/create.h>
#endif
#ifndef BALL_COMMON_MACROS_H
#	include <BALL/COMMON/macros.h>
#endif
#ifndef BALL_COMMON_EXCEPTION_H
#	include <BALL/COMMON/exception.h>
#endif
#ifndef BALL_COMMON_DEBUG_H
#	include <BALL/COMMON/debug.h>
#endif

#include <string>
#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#ifdef BALL_HAS_SSTREAM
# include <sstream>
#else
# include <strstream>
#endif

using std::string;

class QString;
class QByteArray;

namespace BALL 
{
	// forward declaration
	class Substring;

	/**	\defgroup String String
			An improved version of STL string.
    	
			\ingroup  DatatypeMiscellaneous
	*/
	//@{
				
	/**	Extended String class.
			\ingroup String
	*/
 	class BALL_EXPORT String
	{
		///
		friend class Substring;

		public:
				
		// String has no copy constructor taking String&, bool as arguments.
		// the compiler would confuse it with another copy constructor,
		// cast true to 1 and copy only the string from the second character
		// on! We could use BALL_CREATE_NODEEP, but this leads to trouble with
		// inline constructors, so we code it by hand (here and in string.C)
		virtual void* create(bool /* deep */ = true, bool empty = false) const;
	
		/**	@name	Enums and Constants */
		//@{

		/**	Constants to set the compare mode.
				Use one of these constants to set the mode you need.
				These modes affect all  \link compare compare \endlink  methods. As these
				methods are also used in the implementation of comparison operators,
				all comparison operations will get affected from a change. \par
				You may change the comparison mode by invoking setCompareMode. \par
		*/
		enum CompareMode
		{ 
			/// Constant to set to case sensitive comparisons (default)
			CASE_SENSITIVE   = 0, 

			/// Constant to set to case insensitive comparisons
			CASE_INSENSITIVE = 1 
		};

		/**	Constant indicating the end of the string.
				Use this constant instead of <tt>string::npos</tt> to indicate an invalid 
				position inside the string or the end of the string in those methods
				requiring indices.
		*/
		static const Size EndPos;

		/** Constant with a value of the greatest posible value for an element of type size_t.
		 */
		static const size_t npos = -1;

		//@}
		/**	@name	Predefined character classes
				There exist several predefined character classes, that may
				be used in several functions (e.g. trim methods) to represent 
				a set of characters. 
		*/
		//@{

		/// Character class containing all letters (lower and upper case)
		static const char* CHARACTER_CLASS__ASCII_ALPHA;

		/// Character class containing all letters and digits
		static const char* CHARACTER_CLASS__ASCII_ALPHANUMERIC;

		/// Character class containing all lower case letters
		static const char* CHARACTER_CLASS__ASCII_LOWER;

		/// Character class containing all upper case letters
		static const char* CHARACTER_CLASS__ASCII_UPPER;

		/// Character class containing the digits from 0 to 9
		static const char* CHARACTER_CLASS__ASCII_NUMERIC;

		/// Character class containing the digits from 0 to 9 and a dot
		static const char* CHARACTER_CLASS__ASCII_FLOAT;

		/**	Character class containing all whitespace characters.
				Whitespace characters are: \par

					- blank " "
					- horizontal tab $ "\backslash t" $
					- new-line $ "\backslash n" $
					- line-feed $ "\backslash r" $
					- vertical tab $ "\backslash v" $
					- form-feed $ "\backslash f" $
				
		*/
		static const char* CHARACTER_CLASS__WHITESPACE;

		/**	Character class containing double quotes.
		*/
		static const char* CHARACTER_CLASS__QUOTES;

		//@}
		/** @name	Constructors and Destructors 
		*/
		//@{

		/// Default Constructor
		String();

		/// STL string copy constructor
		String(const string& string);

		/// Copy constructor
		String(const String& s);

#ifdef BALL_STD_STRING_HAS_RVALUE_REFERENCES
		/// Move constructor
		String(String&& s);

		/// Move constructor for STL string
		String(string&& s);

		/// Move assigment operator
		String& operator=(String&& s);

		/// Move assignment operator for STL string
		String& operator=(string&& s);
#endif

		/// QString copy constructor
		explicit String(const QString& string);

		/// QByteArray copy constructor
		explicit String(const QByteArray& string);

		/** Creates a new string from a given range of another string.
				@see 		String:Indices
				@exception Exception::IndexUnderflow if <tt>from < 0</tt>
				@exception Exception::IndexOverflow if <tt>from >= size()</tt>
		*/
		String(const String& s, Index from, Size len = EndPos);
	
		/**	Creates a new string from a C type string.
				The new string contains the contents of <b>s</b> until 
				it has reached a length of <b>len</b> or contains a zero character
				(whichever comes first). Default value for <b>len</b> is <b>EndPos</b>,
				meaning as long as possible.
				@exception Exception::IndexUnderflow if <tt>from < 0</tt>
				@exception Exception::IndexOverflow if <tt>from >= size()</tt>
				@exception Exception::NullPointer if <tt>char_ptr == NULL</tt>
		*/
		String(const char* char_ptr, Index from = 0, Size len = EndPos);

		/**	Creates a string using <b>sprintf</b>.
				This constructor creates a new string and sets its content
				to the result of a call to <b>sprintf</b> using <b>format</b> as a
				format string and all additional parameters as arguments. \par
				The result of the sprintf call is intermediately written to a buffer
				of a maximum size of <b>buffer_size</b> characters, so choose an 
				appropriate size for this variables. \par
				@exception IndexUnderflow, if the buffer size specified is not larger than 0
				@exception NullPointer, if <tt>format == 0</tt>
		*/
		String(Size buffer_size, const char* format, ... );

		/**	Create a new string from the contents of a <b>stringstream</b>.
				The contents of the <tt>stringstream</tt> are not modified, i.e.
				successive construction of multiple strings from the same <tt>stringstream</tt>
				object leads to identical copies.
		*/
#ifdef BALL_HAS_SSTREAM
		String(std::stringstream& s);
#else
		String(std::strstream& s);
#endif

		/** Creates a new string from len copies of c.
		*/
		String(const char c, Size len = 1);

		/// Creates a string just containing an unsigned character
		String(const unsigned char uc);

		/// Construct a String from a short
		String(short s);

		/// Construct a String from an unsigned short
		String(unsigned short us);

		/// Construct a String from an int
		String(int i);

		/// Construct a String from an unsigned int
		String(unsigned int ui);

		/// Construct a String from a long
		String(long l);

		/// Construct a String from an unsigned long
		String(unsigned long);

#ifdef BALL_ALLOW_LONG64_TYPE_OVERLOADS
		/// Construct a String from a signed 64 bit integer
		String(LongIndex l);

		/// Construct a String from an unsigned 64 bit integer
		String(LongSize);
#endif

		/// Construct a String from a float value
		String(float f);

		/// Construct a String from a double value
		String(double d);

		/// Cast the String to a std::string reference
		operator string&();

		/// Cast the String to a std::string reference, const version
		operator string const&() const;

		/// Destructor
		virtual ~String();

		/// Clear the string (reset to the empty string)
		void destroy();

		//@}

		/**	@name	Assignment methods 
		*/
		//@{

		/**	Assign a string */
		void set(const String& s);

		/**	Assign a String from a range of another string
				@exception Exception::IndexOverflow if <tt>from < 0</tt>
				@exception Exception::IndexUnderflow if <tt>from >= size()</tt>
		*/
		void set(const String& string, Index from, Size len = EndPos);

		/** Assign a String from a C type string 
				@exception Exception::IndexUnderflow if <tt>from < 0</tt>
				@exception Exception::IndexOverflow if <tt>from >= size()</tt>
				@exception Exception::NullPointer if <tt>char_ptr == NULL</tt>
		*/
		void set(const char* char_ptr, Index from = 0, Size len = EndPos);
	
		/** Assign a string to the result of a <b>sprintf</b> call
				@exception Exception::IndexUnderflow, if the buffer size is zero
				@exception Exception::NullPointer, <tt>format</tt> is a NULL pointer
		*/
		void set(Size buffer_size, const char *format, ...);

		/** Assign a String from a <b>stringstream</b>.
				The contents of the <tt>stringstream</tt> object are not modified.
		*/
#ifdef BALL_HAS_SSTREAM
		void set(std::stringstream& s);
#else
		void set(std::strstream& s);
#endif

		/// Assign a String from the result of repeating <b>c</b> <b>len</b> times
		void set(char c, Size len = 1);

		///	Assign a String from an unsigned char
		void set(unsigned char uc);

		/// Assign a String from a short
		void set(short s);

		/// Assign a String from an unsigned short
		void set(unsigned short us);

		/// Assign a String from an int
		void set(int i);

		/// Assign a String from an unsigned int
		void set(unsigned int ui);

		/// Assign a String from a long 
		void set(long l);

		/// Assign a String from an unsigned long
		void set(unsigned long ul);

#ifdef BALL_ALLOW_LONG64_TYPE_OVERLOADS
		/// Assign a String from a 64 bit integer
		void set(LongIndex l);

		/// Assign a String from an unsigned 64 bit integer
		void set(LongSize ul);
#endif

		/// Assign a String from a float value
		void set(float f);

		/// Assign a String from a double value
		void set(double d);

		/** Assign to a C type string
				The resulting string contains the contents of <b>this</b> until 
				it has reached a length of <b>len</b> or contains a zero character
				(whichever comes first). Default value for <b>len</b> is <b>EndPos</b>,
				meaning as long as possible.
				@exception Exception::IndexUnderflow if <tt>from < 0</tt>
				@exception Exception::IndexOverflow if <tt>from >= size()</tt>
				@exception Exception::NullPointer if <tt>char_ptr == NULL</tt>
		 */
		void get(char* char_ptr, Index from = 0, Size len = EndPos) const;

		/// Assign a String from another String
		const String& operator = (const String& s);

		/** Assign a String from a C type string
				@exception Exception::NullPointer if <tt>pc == NULL</tt>
		 */
		const String& operator = (const char* pc);

		/** Assign a string from a <b>stringstream</b>.
				The contents of the <tt>stringstream</tt> object are not modified.
		*/
#ifdef BALL_HAS_SSTREAM
		const String& operator = (std::stringstream& s);
#else
		const String& operator = (std::strstream& s);
#endif

		/// Assign a String from a single char
		const String& operator = (char c);

		/// Assign a String from an unsigned char
		const String& operator = (unsigned char uc);

		/// Assign a String from a short
		const String& operator = (short s);

		/// Assign a String from an unsigned short
		const String& operator = (unsigned short us);

		/// Assign a String from an int
		const String& operator = (int i);

		/// Assign a String from an unsigned int
		const String& operator = (unsigned int ui);

		/// Assign a String from a long
		const String& operator = (long l);

		/// Assign a String from an unsigned long
		const String& operator = (unsigned long ul);

#ifdef BALL_ALLOW_LONG64_TYPE_OVERLOADS
		/// Assign a String from a 64 bit integer
		const String& operator = (LongIndex l);

		/// Assign a String from an unsigned 64 bit integer
		const String& operator = (LongSize ul);
#endif

		/// Assign a String from a float
		const String& operator = (float f);

		/// Assign a String from a double
		const String& operator = (double d);
		//@}

		/** @name Compare mode-related methods.
				All string comparisons can be made case-sensitive or 
				case insensitive. The behavior can be toggled globally
				for all strings.
		*/
		//@{
		/// Set the compareison mode for all string comparisons
		static void setCompareMode(CompareMode compare_mode);

		/// Return the current comparison mode
		static CompareMode getCompareMode();
		//@}

		/** @name Converters 
		*/
		//@{

		/**	Converts the string to a bool value.
				This method returns <b>false</b>, if the string contains the string <tt>false</tt>
				(may be surrounded by whitespaces), or <b>true</b> otherwise.
		*/
		bool toBool() const;

		///	Return the first character of the string
		char toChar() const;

		/// Return the first character of the string converted to an unsigned char
		unsigned char toUnsignedChar() const;

		/** Convert the string to a short
		 *  @exception Exception::InvalidFormat
		 */
		short toShort() const;

		/** Convert the string to an unsigned short
		 *  @exception Exception::InvalidFormat
		 */
		unsigned short toUnsignedShort() const;

		/** Convert the string to an int
		 *  @exception Exception::InvalidFormat
		 */
		int toInt() const;

		/** Convert the string to an unsigned int
		 *  @exception Exception::InvalidFormat
		 */
		unsigned int toUnsignedInt() const;

		/** Convert the string to a long
		 *  @exception Exception::InvalidFormat
		 */
		long toLong() const;

		/** Convert the string to an unsigned long
		 *  @exception Exception::InvalidFormat
		 */
		unsigned long toUnsignedLong() const;

		/**  Convert the string to a float
		 *  @exception Exception::InvalidFormat
		 */
		float toFloat() const;

		/** Convert the string to a double
		 *  @exception Exception::InvalidFormat
		 */
		double toDouble() const;
		//@}


		/**	@name	Case Conversion 
		*/
		//@{			

		/** Convert all characters in the given range to lower case
		 *	@exception Exception::IndexUnderflow if <tt>from < 0</tt>
		 *	@exception Exception::IndexOverflow if <tt>from || len >= size()</tt>
		 */
		void toLower(Index from = 0, Size len = EndPos);
			
		/** Convert all characters in the given range to upper case
		 *	@exception Exception::IndexUnderflow if <tt>from < 0</tt>
		 *	@exception Exception::IndexOverflow if <tt>from || len >= size()</tt>
		 */
		void toUpper(Index from = 0, Size len = EndPos);

		//@}
		/**	@name Substring Definition 
		*/
		//@{

		/** Return a substring
		 *	@exception Exception::IndexUnderflow if <tt>from < 0</tt>
		 *  @exception Exception::IndexOverflow if <tt>from >= size()</tt>
		 */
		Substring getSubstring(Index from = 0, Size len = EndPos) const;

		/** Return a substring
		 *	@exception Exception::IndexUnderflow if <tt>from < 0</tt>
		 *  @exception Exception::IndexOverflow if <tt>from >= size()</tt>
		 */
		Substring operator () (Index from, Size len = EndPos) const;

		/** Return a substring containing the string before the first occurence of <b>s</b>
		*/
		Substring before(const String& s, Index from = 0) const;
 
		/** Return a substring containing the beginning of the string including the first occurence of <b>s</b>
		*/
		Substring through(const String& s, Index from = 0) const;
 
		/** Return a substring containing the string from the first occurence of <b>s</b> on
		*/
		Substring from(const String& s, Index from = 0) const;

		/** Return a substring containing the string after the first occurence of <b>s</b>.
		*/
		Substring after(const String& s, Index from = 0) const;

		//@}
		/**	@name	AWK style field operations 
		*/
		//@{

		/** Count the fields that are separated by a defined set of delimiters
		 *	@exception Exception::NullPointer if <tt>delimiters == NULL</tt>
		 */  
		Size countFields(const char* delimiters = CHARACTER_CLASS__WHITESPACE) const;

		/** Count the fields and respect quote characters.
		 *	@exception Exception::NullPointer if <tt>delimiters == NULL</tt> or <tt>quotes == NULL</tt> 
		 */
		Size countFieldsQuoted(const char* delimiters = CHARACTER_CLASS__WHITESPACE, 
													 const char* quotes = CHARACTER_CLASS__QUOTES) const;

		/** Return a given field as a substring
		 *	@exception Exception::IndexUnderflow if <tt>from < 0</tt>
		 *	@exception Exception::NullPointer if <tt>delimiters == NULL</tt>
		 */	
		String getField(Index index, const char* delimiters = CHARACTER_CLASS__WHITESPACE, Index* from = 0) const;

		/** Return a given field and respect quote characters.
		 *	@exception Exception::IndexUnderflow if <tt>from < 0</tt>
		 *	@exception Exception::NullPointer if <tt>delimiters == NULL</tt>
		 */
		String getFieldQuoted(Index index, const char* delimiters = CHARACTER_CLASS__WHITESPACE, 
													const char* quotes = CHARACTER_CLASS__QUOTES, Index* from = 0) const;

		/** Split the string into fields and assign these field to an array of strings
		 *	@exception Exception::IndexUnderflow if <tt>from < 0</tt>
		 *	@exception Exception::NullPointer if <tt>delimiters == NULL</tt>
		 */
		Size split(String string_array[], Size array_size, const char* delimiters = CHARACTER_CLASS__WHITESPACE, Index from = 0) const;

		/** Split the string into fields and assign these field to a vector of strings.
				The vector of strings is cleared in any case. Its final size is returned.
				@exception IndexOverflow if <tt>from < 0</tt>
				@exception NullPointer if <tt>delimiters == 0</tt>
		*/
		Size split(std::vector<String>& strings, const char* delimiters = CHARACTER_CLASS__WHITESPACE, Index from = 0) const;

		/** Split the string into fields and respect quote characters.
				Similar to  \link split split \endlink , but delimiters that are inside quote characters (default is  \link CHARACTER_CLASS__QUOTES CHARACTER_CLASS__QUOTES \endlink )
				are not considered to split the string.
				The vector of strings is cleared in any case. Its final size is returned.
				@exception IndexOverflow if <tt>from < 0</tt>
				@exception NullPointer if <tt>delimiters == 0</tt>
		*/
		Size splitQuoted(std::vector<String>& strings, const char* delimiters = CHARACTER_CLASS__WHITESPACE, 
							 const char* quotes = CHARACTER_CLASS__QUOTES, Index from = 0) const;

		//@}
		/**	@name BASIC style string operations 
		*/
		//@{

		/** Strips all characters in <b>trimmed</b> from the left of the string.
				trimLeft stops at the first character encountered that is not in <b>trimmed</b>.
				Using its default parameter CHARACTER_CLASS__WHITESPACE, it is usually handy to 
				remove blanks from the beginning of a string.
				Strings consisting of character from <tt>trimmed</tt> only yield an empty string.
		*/
		String& trimLeft(const char* trimmed = CHARACTER_CLASS__WHITESPACE);

		/** Strips all characters in <b>trimmed</b> from the right of the string.
				trimRight stops at the first character encountered that is not in <b>trimmed</b>.
				Using its default parameter CHARACTER_CLASS__WHITESPACE, it is usually handy to 
				remove blanks from the end of a string.
				Strings consisting of character from <tt>trimmed</tt> only yield an empty string.
		*/
		String& trimRight(const char* trimmed = CHARACTER_CLASS__WHITESPACE);

		/**	Strips all characters in <b>trimmed</b> from both sides of the string.
				trim calls <tt>trimRight(trimmed).trimLeft(trimmed)</tt>.
		*/
		String& trim(const char* trimmed = CHARACTER_CLASS__WHITESPACE);

		// ?????
		/**	Strips all characters in <b>trimmed</b> from both sides of the string.
				trim calls <tt>trimRight(trimmed).trimLeft(trimmed)</tt>.
		*/
		String trim(const char* trimmed = CHARACTER_CLASS__WHITESPACE) const;

		/// Truncate the string to length <b>size</b>
		String& truncate(Size size);

		/// Return a substring containing the <b>len</b> leftmost characters of the string
		Substring left(Size len) const;

		/// Return a substring containing the <b>len</b> rightmost characters of the string
		Substring right(Size len) const;

		/** Return a substring containing the first occurence of <b>pattern</b> in the string.
				If the pattern is not contained in the string, an empty Substring is returned.
				The search for the pattern may also start from an index different from zero, allowing
				incremental search.
				@return	Substring containing the search pattern, empty if not found
				@param	pattern the search pattern
				@param  from		the index in the string to start the search from
		*/
		Substring instr(const String& pattern, Index from = 0) const;

		//@}
		/**	@name	String Operations  
		*/
		//@{
		
		// NOTE: please, please, pretty please, only try to optimize away operator+ definitions
		//       if you *really* know what you are doing. We didn't, and we definitely don't want
		//       to touch this stinking heap of C++ garbage ever again!
    //       (dstoeckel & anhi)
		///	Concatenates two strings
		BALL_EXPORT
		friend String operator + (const String& s1, const string& s2);
		
		///	Concatenates two strings
		BALL_EXPORT
		friend String operator + (const string& s1, const String& s2);

		///	Concatenates two strings
		BALL_EXPORT
		friend String operator + (const String& s1, const String& s2);
		
		/// Concatenates a string and a C type string
		BALL_EXPORT
		friend String operator + (const String& s1, const char* char_ptr);

		/// Concatenates a C type string and a string
		BALL_EXPORT
		friend String operator + (const char* char_ptr, const String& s);

		/// Concatenates a string and a character
		BALL_EXPORT
		friend String operator + (const String& s, char c);
		
		/// Concatenates a character and a string
		BALL_EXPORT
		friend String operator + (char c, const String& s);

#ifdef BALL_STD_STRING_HAS_RVALUE_REFERENCES
		///	Concatenates two strings
		BALL_EXPORT
		friend String operator + (String&& s1, const string& s2);
	
		///	Concatenates two strings
		BALL_EXPORT
		friend String operator + (String&& s1, const String& s2);
	
		///	Concatenates two strings
		BALL_EXPORT
		friend String operator + (String&& s1, String&& s2);

		BALL_EXPORT
		friend String operator + (const String& s1, string&& s2);

		BALL_EXPORT
		friend String operator + (string&& s1, const String& s2);

		BALL_EXPORT
		friend String operator + (const string& s1, String&& s2);

		///	Concatenates two strings
		BALL_EXPORT
		friend String operator + (const String& s1, String&& s2);

		/// Concatenates a string and a C type string
		BALL_EXPORT
		friend String operator + (String&& s1, const char* char_ptr);

		/// Concatenates a C type string and a string
		BALL_EXPORT
		friend String operator + (const char* char_ptr, String&& s);

		/// Concatenates a string and a character
		BALL_EXPORT
		friend String operator + (String&& s, char c);
		
		/// Concatenates a character and a string
		BALL_EXPORT
		friend String operator + (char c, String&& s);
#endif

		/// Swaps the contents with another String
		void swap(String& s);

		/** Reverses the string.
				If called without arguments, this method simply reverses the character sequence of the string.
				By giving arguments for the indices, only a subsequence of the string may be reversed.
				@param	from first index of the sequence to be reversed
				@param	to last index of the sequence to be reversed
				@see		String:Indices
				@exception Exception::IndexUnderflow if <tt>from < 0</tt>
				@exception Exception::IndexOverflow if <tt>from >= size()</tt>
		*/
		String& reverse(Index from = 0, Size len = EndPos);

		/** Substitute the first occurence of <b>to_replace</b> by the content of <b>replacing</b>.
				@return the first position of the substitution or  \link EndPos EndPos \endlink  if <b>to_replace</b> is not found
		*/
		Size substitute(const String& to_replace, const String& replacing);

		/** Substitute *all* occurences of <b>to_replace</b> by the content of <b>replacing</b>. */
		void substituteAll(const String& to_replace, const String& replacing);

		//@}

		/**	@name	Predicates 
		*/
		//@{

		/// True, if the string contains character <b>c</b>
		bool has(char c) const;

		/// True, if the string contains the substring <b>s</b> after index <b>from</b>
		bool hasSubstring(const String& s, Index from = 0) const;

		/// True, if the string starts with <b>s</b>
		bool hasPrefix(const String& s) const;

		/// True, if the string ends with <b>s</b>
		bool hasSuffix(const String& s) const;

		/// True, if the string has size 0
		bool isEmpty() const;

		/** True, if the string only contains letters (any case).	
				It returns also <b>true</b>, if called for an empty string.
		*/
		bool isAlpha() const;

		/** True, if the string only contains letters and digits.
				It returns also <b>true</b>, if called for an empty string.
		*/
		bool isAlnum() const;

		/** True, if the string only contains digits.
				It returns also <b>true</b>, if called for an empty string.
		*/
		bool isDigit() const;

		/** True, if the string is a floating number.
				(It contains only numbers and maybe a dot).
				It returns also <b>true</b>, if called for an empty string.
		*/
		bool isFloat() const;

		/** True, if the string only contains spaces.
				It returns also <b>true</b>, if called for an empty string.
		*/
		bool isSpace() const;

		/** True, if the string only contains whitespace characters.
				Whitespaces are defined in CHARACTER_CLASS__WHITESPACE.
				It returns also <b>true</b>, if called for an empty string.
		*/
		bool isWhitespace() const;

		/// True, if the character is a letter (any case)
		static bool isAlpha(char c);
	
		/// True, if the character is a letter or a digit
		static bool isAlnum(char c);

		/// True, if the character is a digit
		static bool isDigit(char c);

		/// True, if the character is a space
		static bool isSpace(char c);

		/** True, if the character is any whitespace character.
				Whitespaces are defined in CHARACTER_CLASS__WHITESPACE
		*/
		static bool isWhitespace(char c);
		
		//@}
		/** @name Base64 String methods
		*/
		//@{
		
		/// Convert a string to a base 64 string
		String encodeBase64();

		/** Decode a base 64 string.
				Return an empty string, if base64 string is not right encoded. 
		*/
		String decodeBase64();
		
		//@}
		/**	@name	Comparators 
		*/
		//@{

		/** compare to a string.
		 *	@exception Exception::IndexUnderflow if <tt>from < 0</tt>
		 *	@exception Exception::IndexOverflow if <tt>from >= size()</tt>
		 */
		int compare(const String& string, Index from = 0) const;

		/** compare to a string.
		 *	@exception Exception::IndexUnderflow if <tt>from < 0</tt>
		 *	@exception Exception::IndexOverflow if <tt>from >= size()</tt>
		 */
		int compare(const String& string, Index from, Size len) const;


		/** compare to c-style string.
		 *	@exception Exception::IndexUnderflow if <tt>from < 0</tt>
		 *	@exception Exception::IndexOverflow if <tt>from >= size()</tt>
		 *	@exception Exception::NullPointer if <tt>char_ptr == NULL</tt>
		 */
		int compare(const char* char_ptr, Index from = 0) const;

		/** compare to c-style string.
		 *	@exception Exception::IndexUnderflow if <tt>from < 0</tt>
		 *	@exception Exception::IndexOverflow if <tt>from >= size()</tt>
		 *	@exception Exception::NullPointer if <tt>char_ptr == NULL</tt>
		 */
		int compare(const char* char_ptr, Index from, Size len) const;

		/** compare to character
		 *	@exception Exception::IndexUnderflow if <tt>from < 0</tt>
		 *	@exception Exception::IndexOverflow if <tt>from >= size()</tt>
		 */
		int compare(char c, Index from = 0) const;

		/** Equality operator.
		 */ 
		BALL_EXPORT
		friend bool operator == (const String& s1, const String& s2);

		/** Inequality operator.
		 */ 
		BALL_EXPORT
		friend bool operator != (const String& s1, const String& s2);

		/** Less than comparison
		 */ 
		BALL_EXPORT
		friend bool operator < (const String& s1, const String& s2);

		/** Less than or equal comparison
		 */ 
		BALL_EXPORT
		friend bool operator <= (const String& s1, const String& s2);

		/** Greater than comparison
		 */ 
		BALL_EXPORT
		friend bool operator > (const String& s1, const String& s2);

		/** Greater than or equal comparison
		 */ 
		BALL_EXPORT
		friend bool operator >= (const String& s1, const String& s2);

		/** Equality operator.
		 *	@exception Exception::NullPointer if <tt>char_ptr == NULL</tt>
		 */ 
		BALL_EXPORT
		friend bool operator == (const char* char_ptr, const String& string);

		/** Inequality operator.
		 *	@exception Exception::NullPointer if <tt>char_ptr == NULL</tt>
		 */ 
		BALL_EXPORT
		friend bool operator != (const char* char_ptr, const String& string);

		/** Less than comparison
		 *	@exception Exception::NullPointer if <tt>char_ptr == NULL</tt>
		 */ 
		BALL_EXPORT
		friend bool operator < (const char* char_ptr, const String& string);

		/** Less than or equal comparison
		 *	@exception Exception::NullPointer if <tt>char_ptr == NULL</tt>
		 */ 
		BALL_EXPORT
		friend bool operator <= (const char* char_ptr, const String& string);
		
		/** Greater than comparison
		 *	@exception Exception::NullPointer if <tt>char_ptr == NULL</tt>
		 */ 
		BALL_EXPORT
		friend bool operator > (const char* char_ptr, const String& string);

		/** Greater than or equal comparison
		 *	@exception Exception::NullPointer if <tt>char_ptr == NULL</tt>
		 */ 
		BALL_EXPORT
		friend bool operator >= (const char* char_ptr, const String& string);

		/** Equality operator.
		 *	@exception Exception::NullPointer if <tt>char_ptr == NULL</tt>
		 */ 
		bool operator == (const char* char_ptr) const;

		/** Inequality operator.
		 *	@exception Exception::NullPointer if <tt>char_ptr == NULL</tt>
		 */ 
		bool operator != (const char* char_ptr) const;

		/** Less than comparison
		 *	@exception Exception::NullPointer if <tt>char_ptr == NULL</tt>
		 */ 
		bool operator < (const char* char_ptr) const;

		/** Less than or equal comparison
		 *	@exception Exception::NullPointer if <tt>char_ptr == NULL</tt>
		 */ 
		bool operator <= (const char* char_ptr) const;

		/** Greater than comparison
		 *	@exception Exception::NullPointer if <tt>char_ptr == NULL</tt>
		 */ 
		bool operator > (const char* char_ptr) const;

		/** Greater than or equal comparison
		 *	@exception Exception::NullPointer if <tt>char_ptr == NULL</tt>
		 */ 
		bool operator >= (const char* char_ptr) const;

		///
		BALL_EXPORT
		friend bool operator == (char c, const String& string);

		///
		BALL_EXPORT
		friend bool operator != (char c, const String& string);

		///
		BALL_EXPORT
		friend bool operator < (char c, const String& string);

		///
		BALL_EXPORT
		friend bool operator <= (char c, const String& string);
		
		///
		BALL_EXPORT
		friend bool operator > (char c, const String& string);

		///
		friend bool operator >= (char c, const String& string);

		///
		bool operator == (char c) const;

		///
		bool operator != (char c) const;

		///
		bool operator < (char c) const;

		///
		bool operator <= (char c) const;

		///
		bool operator > (char c) const;

		///
		bool operator >= (char c) const;

		//@}
		/**	@name	Debugging and Diagnostics 
		*/
		//@{

		///
		bool isValid() const;

		///
		void dump(std::ostream& s = std::cout, Size depth = 0) const;

		//@}			
		/**	@name	Stream Operations 
		*/
		//@{

		///
		std::istream& getline(std::istream& s = std::cin, char delimiter = '\n');

		///
//		BALL_EXPORT
//		friend std::istream& getline(std::istream& s,  String& string,  char delimiter);

		//@}

		/// Constant empty string.
		static const String EMPTY;

		/**@ std::string compatibility layer
		 */
	  //@{

		/** name typedefs
		 */
		//@{
		typedef string::value_type             valuetype;
		typedef string::traits_type            traits_type;
		typedef string::allocator_type         allocator_type;
		typedef string::reference              reference;
		typedef string::const_reference        const_reference;
		typedef string::pointer                pointer;
		typedef string::const_pointer          const_pointer;
		typedef string::iterator               iterator;
		typedef string::const_iterator         const_iterator;
		typedef string::reverse_iterator       reverse_iterator;
		typedef string::const_reverse_iterator const_reverse_iterator;
		typedef string::difference_type        difference_type;
		typedef string::size_type              size_type;
		//@}

		/** @name Iterators
		 */
		//@{

		///
		iterator begin() BALL_NOEXCEPT;
		///
		const_iterator begin() const BALL_NOEXCEPT;
		///
		iterator end() BALL_NOEXCEPT;
		///
		const_iterator end() const BALL_NOEXCEPT;
		///
		reverse_iterator rbegin() BALL_NOEXCEPT;
		///
		const_reverse_iterator rbegin() const BALL_NOEXCEPT;
		///
		reverse_iterator rend() BALL_NOEXCEPT;
		///
		const_reverse_iterator rend() const BALL_NOEXCEPT;

#ifdef BALL_HAS_STD_STRING_CONST_ITERATORS
		///
		const_iterator cbegin() const BALL_NOEXCEPT;
		///
		const_iterator cend() const BALL_NOEXCEPT;
		///
		const_reverse_iterator crbegin() const BALL_NOEXCEPT;
		///
		const_reverse_iterator crend() const BALL_NOEXCEPT;
#endif

		//@}

		/** @name Capacity
		 */
		//@{

		///
		size_t size() const BALL_NOEXCEPT;
		///
		size_t length() const BALL_NOEXCEPT;
		///
		size_t max_size() const BALL_NOEXCEPT;
		///
		void resize(size_t n);
		///
		void resize(size_t n, char c);
		///
		size_t capacity() const BALL_NOEXCEPT;
		///
		void reserve(size_t n = 0);
		///
		void clear() BALL_NOEXCEPT;
		///
		bool empty() const BALL_NOEXCEPT;

#ifdef BALL_HAS_STD_STRING_SHRINK_TO_FIT
		///
		void shrink_to_fit();
#endif
		//@}

		/** @name Element Access
		 */
		//@{

		///
		char& operator[] (size_t pos);
		///
		const char& operator[] (size_t pos) const;
		///
		char& at(size_t pos);
		///
		const char& at(size_t pos) const;

#ifdef BALL_HAS_STD_STRING_FRONT_BACK
		///
		char& front();
		///
		const char& front() const;
		///
		char& back();
		///
		const char& back() const;
#endif

		//@}

		/** @name Modifiers
		 */
		//@{
		///
		String& operator += (const String& str);
		///
		String& operator += (const string& str);
		///
		String& operator += (const char* s);
		///
		String& operator += (char c);
#ifdef BALL_HAS_INITIALIZER_LISTS
		///
		String& operator += (std::initializer_list<char> il);
#endif
		///
		String& append(const String& str);
		///
		String& append(const string& str);
		///
		String& append(const string& str, size_t subpos, size_t sublen);
		///
		String& append(const char* s);
		///
		String& append(const char* s, size_t n);
		///
		String& append(size_t n, char c);
		///
		template <class InputIterator>
		String& append(InputIterator first, InputIterator last);
#ifdef BALL_HAS_INITIALIZER_LISTS
		///
		String& append(std::initializer_list<char> li);
#endif
		///
		void push_back(char c);
		///
		String& assign(const String& str);
		///
		String& assign(const string& str);
		///
		String& assign(const string& str, size_t subpos, size_t sublen);
		///
		String& assign(const char* s);
		///
		String& assign(const char* s, size_t n);
		///
		String& assign(size_t n, char c);
		///
		template <class InputIterator>
		String& assign(InputIterator first, InputIterator last);
#ifdef BALL_HAS_INITIALIZER_LISTS
		///
		String& assign(std::initializer_list<char> li);
#endif
#ifdef BALL_STD_STRING_HAS_RVALUE_REFERENCES
		///
		String& assign(string&& str) BALL_NOEXCEPT;
#endif

		///
		String& insert(size_t pos, const string& str);
		///
		String& insert(size_t pos, const string& str, size_t subpos, size_t sublen);
		///
		String& insert(size_t pos, const char* s);
		///
		String& insert(size_t pos, const char* s, size_t n);
		///
		String& insert(size_t pos, size_t n, char c);
#ifdef BALL_HAS_STD_STRING_CONST_ITERATOR_FUNCTIONS
		///
		iterator insert(const_iterator p, size_t n, char c);
		///
		iterator insert(const_iterator p, char c);
#else
		///
		void insert(iterator p, size_t n, char c);
		///
		iterator insert(iterator p, char c);
#endif
		///
		template <class InputIterator>
		iterator insert(iterator p, InputIterator first, InputIterator last);
#if defined(BALL_HAS_INITIALIZER_LISTS) && defined(BALL_HAS_STD_STRING_CONST_ITERATOR_INITLIST_INSERT)
		///
		String& insert(const_iterator p, std::initializer_list<char> li);
#endif

		///
		String& erase(size_t pos = 0, size_t len = npos);
#ifdef BALL_HAS_STD_STRING_CONST_ITERATOR_FUNCTIONS
		///
		iterator erase(const_iterator p);
		///
		iterator erase(const_iterator first, const_iterator last);
#else
		///
		iterator erase(iterator p);
		///
		iterator erase(iterator first, iterator last);
#endif

		///
		String& replace(size_t pos, size_t len, const string& str);
		///
		String& replace(size_t pos, size_t len, const string& str, size_t subpos, size_t sublen);
		///
		String& replace(size_t pos, size_t len, const char* s);
		///
		String& replace(size_t pos, size_t len, const char* s, size_t n);
		///
		String& replace(size_t pos, size_t len, size_t n, char c);
#ifdef BALL_HAS_STD_STRING_CONST_ITERATOR_FUNCTIONS
		///
		String& replace(const_iterator i1, const_iterator i2, const string& str);
		///
		String& replace(const_iterator i1, const_iterator i2, const char* s);
		///
		String& replace(const_iterator i1, const_iterator i2, const char* s, size_t n);
		///
		String& replace(const_iterator i1, const_iterator i2, size_t n, char c);
		///
		template <class InputIterator>
		String& replace(const_iterator i1, const_iterator i2, InputIterator first, InputIterator last);
#else
		///
		String& replace(iterator i1, iterator i2, const string& str);
		///
		String& replace(iterator i1, iterator i2, const char* s);
		///
		String& replace(iterator i1, iterator i2, const char* s, size_t n);
		///
		String& replace(iterator i1, iterator i2, size_t n, char c);
		///
		template <class InputIterator>
		String& replace(iterator i1, iterator i2, InputIterator first, InputIterator last);
#endif
#if defined(BALL_HAS_INITIALIZER_LISTS) && defined(BALL_HAS_STD_STRING_CONST_ITERATOR_FUNCTIONS)
		///
		String& replace(const_iterator i1, const_iterator i2, std::initializer_list<char> li);
#endif

		///
		void swap(string& str);

#ifdef BALL_HAS_STD_STRING_POP_BACK
		///
		void pop_back();
#endif
		//@}

		/** @name String Operations
		 */
		//@{

		///
		const char* c_str() const BALL_NOEXCEPT;
		///
		const char* data() const BALL_NOEXCEPT;
		///
		allocator_type get_allocator() const BALL_NOEXCEPT;
		///
		size_t copy(char* s, size_t len, size_t pos = 0) const;

		///
		size_t find(const string& str, size_t pos = 0) const BALL_NOEXCEPT;
		///
		size_t find(const char* s, size_t pos = 0) const;
		///
		size_t find(const char* s, size_t pos, size_t n) const;
		///
		size_t find(char c, size_t pos = 0) const BALL_NOEXCEPT;

		///
		size_t rfind(const string& str, size_t pos = npos) const BALL_NOEXCEPT;
		///
		size_t rfind(const char* s, size_t pos = npos) const;
		///
		size_t rfind(const char* s, size_t pos, size_t n) const;
		///
		size_t rfind(char c, size_t pos = npos) const BALL_NOEXCEPT;
		
		///
		size_t find_first_of(const string& str, size_t pos = 0) const BALL_NOEXCEPT;
		///
		size_t find_first_of(const char* s, size_t pos = 0) const;
		///
		size_t find_first_of(const char* s, size_t pos, size_t n) const;
		///
		size_t find_first_of(char c, size_t pos = 0) const BALL_NOEXCEPT;

		///
		size_t find_last_of(const string& str, size_t pos = npos) const BALL_NOEXCEPT;
		///
		size_t find_last_of(const char* s, size_t pos = npos) const;
		///
		size_t find_last_of(const char* s, size_t pos, size_t n) const;
		///
		size_t find_last_of(char c, size_t pos = npos) const BALL_NOEXCEPT;

		///
		size_t find_first_not_of(const string& str, size_t pos = 0) const BALL_NOEXCEPT;
		///
		size_t find_first_not_of(const char* s, size_t pos = 0) const;
		///
		size_t find_first_not_of(const char* s, size_t pos, size_t n) const;
		///
		size_t find_first_not_of(char c, size_t pos = 0) const BALL_NOEXCEPT;

		///
		size_t find_last_not_of(const string& str, size_t pos = npos) const BALL_NOEXCEPT;
		///
		size_t find_last_not_of(const char* s, size_t pos = npos) const;
		///
		size_t find_last_not_of(const char* s, size_t pos, size_t n) const;
		///
		size_t find_last_not_of(char c, size_t pos = npos) const BALL_NOEXCEPT;

		///
		string substr(size_t pos = 0, size_t len = npos) const;

		///
		int compare(const string& str) const BALL_NOEXCEPT;
		///
		int compare(size_t pos, size_t len, const string& str) const;
		///
		int compare(size_t pos, size_t len, const string& str, size_t subpos, size_t sublen) const;
		///
		//int compare(const char* s) const;
		///
		int compare(size_t pos, size_t len, const char* s) const;
		///
		int compare(size_t pos, size_t len, const char* s, size_t n) const;

		//@}
		
	  //@}

		protected:
	
		// the validate...  methods check perform a thorough
		// index checking and an index translation
		// Indices below zero are interpreted as indices
		// relative to the end of the string
		// All methods throw IndexUnder|Overflow exceptions
		//
		void validateIndex_(Index& index) const;
	
		void validateRange_(Index& from, Size& len) const; 

		static void validateCharPtrRange_(Index& from, Size& len, const char* char_ptr);
		
		static void valudateCharPtrIndex_(Index& index);
		
		private:

		/// The encapsulated std::string
		string str_;

		static int compareAscendingly_(const char* a,  const char* b);

		static int compareDescendingly_(const char* a,  const char* b);

		static CompareMode compare_mode_;

		static char B64Chars_[64];

		static int Index_64_[128];
	};

	/**	A substring class.
			The Substring class represents an efficient way to deal with substrings
			of  \link String String \endlink . Each Substring is bound to an instance of String and 
			is defined by a start and end index. It can be used like a String (with several
			restrictions) but only affects the given range of the string it is bound to. \par
			
			\ingroup String
	*/
	class BALL_EXPORT Substring
	{
		friend class String;

		public:

		BALL_CREATE_DEEP(Substring)

		/**	@name	Exceptions
		*/
		//@{

		/**	Exception thrown if an unbound substring is accessed.
				This exception is thrown by most accessors and predicates of
				Substring if the substring is not bound to a string.
		*/
		class BALL_EXPORT UnboundSubstring
			:	public Exception::GeneralException
		{
			public:
			UnboundSubstring(const char* file, int line); 
		};

		/**	Exception thrown if an invalid substring is accessed.
				This exception is thrown if an invalid substring
				is to be used.
				@see isValid
		*/
		class BALL_EXPORT InvalidSubstring
			:	public Exception::GeneralException
		{
			public:
			InvalidSubstring(const char* file, int line); 
		};

		//@}
		/**	@name	Constructors and Destructors
		*/
		//@{

		/** Default constructor.
				Create an empty string.
		*/
		Substring();

		/** Copy constructor.
				Create a substring from another substring.
				@param substring the substring to be copied
				@param deep ignored
		*/
		Substring(const Substring& substring, bool deep = true);

		/** Create a substring from a string and two indices.
				@param	string the string the substring is bound to.
				@param	from the start index of the substring
				@param	len the length of the substring (default <tt>EndPos</tt>: to the end of the string)
				@exception Exception::IndexUnderflow if <tt>from < 0</tt>
				@exception Exception::IndexOverflow if <tt>from || len >= size()</tt>
		*/
		Substring(const String& string, Index from = 0, Size len = String::EndPos);

		/** Destructor.
				Destruct the substring.
		*/
		virtual ~Substring();

		/** Clear the substrings contents.
				Unbind the substring from its string and set the
				start and the end position to 0.
		*/
		void destroy();

		/** Clear the substrings contents.
				Unbind the substring from its string and set the
				start and the end position to 0.
		*/
		virtual void clear();

		//@}
		/**	@name Converters 
		*/
		//@{

		/** Convert a substring to a String.
	   *	Return a copy of the substring's contents.
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		operator String() const;

		/** Convert a substring to a std::string.
	   *	Return a copy of the substring's contents.
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		//explicit operator string() const;

		/** Convert a substring to a string.
		 *	Return a copy of the substring's contents.
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		String toString() const;

		//@}
		/**	@name Binding and Unbinding Substrings 
		*/
		//@{
		
		/** Bind the substring to a string.
				@param string the string to bind to
				@param from the start position in the string (default is the beginning of the string)
				@param len the substring's length (default is to the end of the string)

		  	@exception Exception::IndexUnderflow if <tt>index < 0</tt>
		  	@exception Exception::IndexOverflow if <tt>index >= size()</tt>
		*/
		Substring& bind(const String& string, Index from = 0, Size len = String::EndPos);

		/** Bind the substring to the same string another substring is bound to.
		 *	@param	substring the substring that is bound to a string
		 *	@exception Exception::IndexUnderflow if <tt>index < 0</tt>
		 *	@exception Exception::IndexOverflow if <tt>index >= size()</tt>
		*/
		Substring& bind(const Substring& substring, Index from = 0, Size len = String::EndPos);

		/// unbinds the substring from the string it is bound to
		void unbind();

		/// Return a pointer to the bound String
		String* getBoundString();

		/// Retunrs a const pointer to the bound String
		const String* getBoundString() const
;

		//@}
		/**	@name	Assignment 
		*/
		//@{
	
		/** Sets the substring to a certain string
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		void set(const String& string);

		/** Copies a substring from another substring
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		*/
		void set(const Substring& s);

		/** Assigns a substring from a char pointer
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 *	@exception Exception::IndexUnderflow
		 *	@exception Exception::NullPointer if <tt>char_ptr == NULL</tt>
		 */
		void set(const char* char_ptr, Size size = String::EndPos);

		/** String assignment operator
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		const Substring& operator = (const String& string);

		/** Substring assignment operator
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		const Substring& operator = (const Substring& substring);

		/** char pointer assignment operator
		 *	@exception Exception::NullPointer if <tt>char_ptr == NULL</tt>
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		const Substring& operator = (const char* char_ptr);

		//@}
		/**	@name	Accessors and Mutators 
		*/
		//@{	
		
		/** Return a pointer to the substring's contents
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		char* c_str();

		/** Return a const pointer to the substring's contents
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		const char* c_str() const;

		/** Return the first index of the substring.
	 	 *	This means the starting point in the bound string.
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		*/
		Index getFirstIndex() const;

		/** Return the last index of the substring
		 *	This means the end point in the bound string.
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		*/
		Index getLastIndex() const;

		/// Return the substring size
		Size size() const;

		/** Mutable random access to a character of the substring
		 *	@exception Exception::IndexUnderflow if <tt>index < 0</tt>
		 *	@exception Exception::IndexOverflow if <tt>index >= size()</tt>
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		char& operator [] (Index index);

		/** Random access to a character of the substring (const method).
		 *	@exception Exception::IndexUnderflow if <tt>index < 0</tt>
		 *	@exception Exception::IndexOverflow if <tt>index >= size()</tt>
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		char operator [] (Index index) const;

		/** Converts the substring to lower case characters
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		Substring& toLower();

		/** Converts the substring to lower case characters
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		Substring& toUpper();
			
		//@}
		/**	@name Predicates 
		*/
		//@{

		/// Return true, if the substring is bound to a String
		bool isBound() const;
		
		/// Return true, if the substring is empty or unbound
		bool isEmpty() const;

		//@}
		/**	@name	Comparison Operators 
		*/
		//@{

		/** returns true, if the contents of the two substrings are equal
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		bool operator == (const Substring& substring) const;

		/** Return true, if the contents of the two substrings are not equal
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		bool operator != (const Substring& substring) const;

		/** Return true, if the contents of the substring and the string are equal
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		bool operator == (const String& string) const;

		/** Return true, if the contents of the substring and the string are not equal
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		bool operator != (const String& string) const;

		/** Return true, if the contents of the substring and the string are equal
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		BALL_EXPORT
		friend bool operator == (const String& string, const Substring& substring);

		/** Return true, if the contents of the substring and the string are not equal
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		BALL_EXPORT
		friend bool operator != (const String& string, const Substring& substring);

		/** Return true, if the contents of the substring are equal to the contents of the C-string
		 *	@exception Exception::NullPointer if <tt>delimiters == NULL</tt>
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		bool operator == (const char* char_ptr) const;

		/** Return true, if the contents of the substring are not equal to the contents of the C-string
		 *	@exception Exception::NullPointer if <tt>delimiters == NULL</tt>
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		bool operator != (const char* char_ptr) const;

		/** Return true, if the substring has length 1 and contains the given char
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		bool operator == (char c) const;

		/** Return true, if the substring is differnet from the given char
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		bool operator != (char c) const;

		//@}
		/**	@name	Stream I/O 
		*/
		//@{

		/// Writes the substring to a stream
		BALL_EXPORT
		friend std::ostream& operator << (std::ostream& s, const Substring& substring);

		//@}
		/**	@name	Debugging and Diagnostics 
		*/
		//@{

		/** Return true, if the string is bound to a string and its indices are valid.
				Valid indices means that the first index is not greater than the last index, 
				both indices are non-negative and lesser than the size of the bound string.
		*/
		bool isValid() const;

		/**	Dumps the substring object (including the values of its private members)
		 *  @exception Substring::UnboundSubstring if this Substring is not correctly bound
		 */
		void dump(std::ostream& s = std::cout, Size depth = 0) const;

		//@}
		
		protected:
			
		// throws IndexUnderflow|IndexOverflow
		void validateRange_(Index& from, Size& len) const;

		private:
		
		/*_	@name	Attributes
		*/
		//_@{

		//_ pointer to the bound String
		String* 	bound_;

		//_ start index in the bound String
		Index 		from_;

		//_ end index in the bound String
		Index 		to_;
		//_@}
	};
	
	// non-member functions of String

	/** Equality operator.
	 */ 
	BALL_EXPORT
	bool operator == (const String& s1, const String& s2);

	/** Inequality operator.
	 */ 
	BALL_EXPORT
	bool operator != (const String& s1, const String& s2);

	/** Less than comparison
	 */ 
	BALL_EXPORT
	bool operator < (const String& s1, const String& s2);

	/** Less than or equal comparison
	 */ 
	BALL_EXPORT
	bool operator <= (const String& s1, const String& s2);

	/** Greater than comparison
	 */ 
	BALL_EXPORT
	bool operator > (const String& s1, const String& s2);

	/** Greater than or equal comparison
	 */ 
	BALL_EXPORT
	bool operator >= (const String& s1, const String& s2);

	///	Concatenates two strings
	BALL_EXPORT
	String operator + (const String& s1, const string& s2);
		
	///	Concatenates two strings
	BALL_EXPORT
	String operator + (const string& s1, const String& s2);

	///	Concatenates two strings
	BALL_EXPORT
	String operator + (const String& s1, const String& s2);
		
	/// Concatenates a string and a C type string
	BALL_EXPORT
	String operator + (const String& s1, const char* char_ptr);

	/// Concatenates a C type string and a string
	BALL_EXPORT
	String operator + (const char* char_ptr, const String& s);

	/// Concatenates a string and a character
	BALL_EXPORT
	String operator + (const String& s, char c);
		
	/// Concatenates a character and a string
	BALL_EXPORT
	String operator + (char c, const String& s);

#ifdef BALL_STD_STRING_HAS_RVALUE_REFERENCES
	///	Concatenates two strings
	BALL_EXPORT
	String operator + (String&& s1, const string& s2);
	
	///	Concatenates two strings
	BALL_EXPORT
	String operator + (String&& s1, const String& s2);
	
	///	Concatenates two strings
	BALL_EXPORT
	String operator + (String&& s1, String&& s2);

	///	Concatenates two strings
	BALL_EXPORT
	String operator + (const String& s1, string&& s2);

	///	Concatenates two strings
	BALL_EXPORT
	String operator + (string&& s1, const String& s2);

	///	Concatenates two strings
	BALL_EXPORT
	String operator + (const string& s1, String&& s2);

	///	Concatenates two strings
	BALL_EXPORT
	String operator + (const String& s1, String&& s2);

	/// Concatenates a string and a C type string
	BALL_EXPORT
	String operator + (String&& s1, const char* char_ptr);

	/// Concatenates a C type string and a string
	BALL_EXPORT
	String operator + (const char* char_ptr, String&& s);

	/// Concatenates a string and a character
	BALL_EXPORT
	String operator + (String&& s, char c);
		
	/// Concatenates a character and a string
	BALL_EXPORT
	String operator + (char c, String&& s);
#endif
} // namespace BALL

namespace std
{
	// Non-member functions for string

	///
	istream& operator>> (istream& is, BALL::String& str);

	///
	ostream& operator<< (ostream& os, BALL::String const& str);

	///
	istream& getline(istream& is, BALL::String& str, char delim);
	///
	istream& getline(istream& is, BALL::String& str);


#ifdef BALL_STD_STRING_HAS_RVALUE_REFERENCES
	///
	istream& getline(istream& is, BALL::String&& str, char delim);
	///
	istream& getline(istream& is, BALL::String&& str);
#endif
}

	//@}

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/DATATYPE/string.iC>
#	endif

#endif // BALL_DATATYPE_STRING_H
