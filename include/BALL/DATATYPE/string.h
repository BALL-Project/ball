// $Id: string.h,v 1.21 2000/07/22 16:06:14 amoll Exp $

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
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <strstream>
#include <iostream>

using std::string;

namespace BALL 
{

	/**	@name	String.
			An improved version of STL string.
			{\bf Definition:} \URL{BALL/DATATYPE/string.h}
	*/
	//@{

	class String;

	/**	A substring class.
			The Substring class represents an efficient way to deal with substrings
			of \Ref{String}s. Each Substring is bound to an instance of String and 
			is defined by a start and end index. It can be used like a String (with several
			restrictions) but only affects the given range of the string it is bount to.\\
			{\bf Definition:} \URL{BALL/DATATYPE/string.h}
			\\
	*/
	class Substring
	{
		friend class String;

		public:

		BALL_CREATE(Substring)

		/**	@name	Exceptions
		*/
		//@{

		/**	Exception thrown if an unbound substring is accessed.
				This exception is thrown by most accessors and predicates of
				Substring if the substring is not bound to a string.
		*/
		class UnboundSubstring
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
		class InvalidSubstring
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
				@param	len the length of the substring (default {\tt npos}: to the end of the string)
		*/
		Substring(const String& string, Index from = 0, Size len = string::npos)
			throw(Exception::IndexUnderflow, Exception::IndexOverflow);

		/** Destructor.
				Destruct the substring.
		*/
		virtual ~Substring();

		/** Clear the substrings contents.
				Unbind the substring from its string and set the
				start and the end position to 0.
		*/
		void destroy();
	
		//@}

		/**	@name Converters 
		*/
		//@{

		/** Convert a substring to a string.
				Return a copy of the substring's contents.
		*/
		operator String() const
			throw(Substring::UnboundSubstring);

		/** Convert a substring to a string.
				Return a copy of the substring's contents.
		*/
		String toString() const
			throw(Substring::UnboundSubstring);
		//@}


		/**	@name Binding and Unbinding Substrings 
		*/
		//@{
		
		/** Bind the substring to a string.
				@param string the string to bind to
				@param from the start position in the string (default is the beginning of the string)
				@param len the substring's length (default is to the end of the string)
		*/
		Substring& bind(const String& string, Index from = 0, Size len = string::npos)
			throw(Exception::IndexUnderflow, Exception::IndexOverflow);

		/** Bind the substring to the same string another substring is bound to.
				@param	substring the substring that is bound to a string
		*/
		Substring& bind(const Substring& substring, Index from = 0, Size len = string::npos)
			throw(Exception::IndexUnderflow, Exception::IndexOverflow);

		/// unbinds the substring from the string it is bound to
		void unbind();

		/// Returns a pointer to the bound String
		String* getBoundString();

		/// Retunrs a const pointer to the bound String
		const String* getBoundString() const;

		//@}
		
		/**	@name	Assignment */
		//@{
	
		/** Sets the substring to a certain string
		*/
		void set(const String& string)
			throw(Substring::UnboundSubstring);

		/** Copies a substring from another substring
		*/
		void set(const Substring& s)
			throw(Substring::UnboundSubstring);

		/// Assigns a substring from a char pointer
		void set(const char* char_ptr, Size size = string::npos)
			throw(Substring::UnboundSubstring, Exception::NullPointer, Exception::SizeUnderflow);

		/// String assignment operator
		Substring& operator = (const String& string)
			throw(Substring::UnboundSubstring());

		/// Substring assignment operator
		Substring& operator = (const Substring& substring)
			throw(Substring::UnboundSubstring());

		/// char pointer assignment operator
		Substring& operator = (const char* char_ptr)
			throw(Substring::UnboundSubstring, Exception::NullPointer);

		//@}

		/**	@name	Accessors and Mutators */
		//@{	
		
		/// Returns a pointer to the substring's contents
		char* c_str()
			throw(Substring::UnboundSubstring);

		/// Returns a const pointer to the substring's contents
		const char* c_str() const
			throw(Substring::UnboundSubstring);

		/// Returns the first index of the substring
		Index getFirstIndex() const
			throw(Substring::UnboundSubstring);

		/// Returns the last index of the substring
		Index getLastIndex() const
			throw(Substring::UnboundSubstring);

		/// Returns the substring size
		Size size() const;

		/// Mutable random access to a character of the substring
		char& operator [] (Index index)
			throw(Substring::UnboundSubstring);

		/// Random access to a character of the substring
		char operator [] (Index index) const
			throw(Substring::UnboundSubstring);

		/// Converts the substring to lower case characters
		Substring& toLower()	throw(Substring::UnboundSubstring);

		/// Converts the substring to lower case characters
		Substring& toUpper()	throw(Substring::UnboundSubstring);
			
		//@}

		/**	@name Predicates */
		//@{
		/// Returns true, if the substring is bound to a String
		bool isBound() const;
		
		/// Returns true, if the substring is empty or unbound
		bool isEmpty() const;
		//@}
			

		/**	@name	Comparison Operators */
		//@{

		/// returns true, if the contents of the two substrings are equal
		bool operator == (const Substring& substring) const
			throw(Substring::UnboundSubstring);

		/// Returns true, if the contents of the two substrings are not equal
		bool operator != (const Substring& substring) const
			throw(Substring::UnboundSubstring);

		/// Returns true, if the contents of the substring and the string are equal
		bool operator == (const String& string) const
			throw(Substring::UnboundSubstring);

		/// Returns true, if the contents of the substring and the string are not equal
		bool operator != (const String& string) const
			throw(Substring::UnboundSubstring);

		/// Returns true, if the contents of the substring and the string are equal
		friend bool operator == (const String& string, const Substring& substring);

		/// Returns true, if the contents of the substring and the string are not equal
		friend bool operator != (const String& string, const Substring& substring);

		/// Returns true, if the contents of the substring are equal to the contents of the string the char pointer points to
		bool operator == (const char* char_ptr) const
		throw(Substring::UnboundSubstring, Exception::NullPointer);

		/// Returns true, if the contents of the substring are not equal to the contents of the string the char pointer points to
		bool operator != (const char* char_ptr) const
		throw(Substring::UnboundSubstring, Exception::NullPointer);

		/// Returns truw, if the substring has length 1 and contains the given char
		bool operator == (char c) const
			throw(Substring::UnboundSubstring);

		/// Returns true, if the substring is differnet from the given char
		bool operator != (char c) const
			throw(Substring::UnboundSubstring);
		//@}

		/**	@name	Stream I/O */
		//@{

		/// Writes the substring to a stream
		friend ::std::ostream& operator << (::std::ostream& s, const Substring& substring);
		//@}
	
		/**	@name	Debugging and Diagnostics */
		//@{

		/** Returns true, if the string is bound to a string and its indices are valid.
				Valid indices means that the first index is not greater than the last index, 
				both indices are non-negative and lesser than the size of the bound string.
		*/
		bool isValid() const;

		///	Dumps the substring object (including the values of its private memebers)
		void dump(::std::ostream& s = ::std::cout, Size depth = 0) const 
			throw(Substring::UnboundSubstring);
		//@}
		
		protected:
			
		void validateRange_(Index& from, Size& len) const
			throw(Exception::IndexUnderflow, Exception::IndexOverflow);

		private:
		
		/*_	@name	Attributes
		*/
		//_@{

		///
		String* 	bound_;

		///
		Index 		from_;

		///
		Index 		to_;
		//_@}
	};


	/**	Extended String class.
	*/
 	class String
		: public string
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
	
		/**	@name	Enums */
		//@{

		/**	Constants to set the compare mode.
				Use one of these constants to set the mode you need.
				These modes affect all \Ref{compare} methods. As these
				methods are also used in the implementation of comparison operators,
				all comparison operations will get affected from a change.\\
				You may change the comparison mode by invoking setCompareMode.\\
		*/
		enum CompareMode
		{ 
			/// Constant to set to case sensitive comparisons (default)
			CASE_SENSITIVE   = 0, 

			/// Constant to set to case insensitive comparisons
			CASE_INSENSITIVE = 1 
		};

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
				Whitespace characters are:\\
				\begin{itemize}
					\item blank ({\tt" "})
					\item horizontal tab ({\tt"\\t"})
					\item new-line ({\tt"\\n"})
					\item line-feed ({\tt"\\r"})
					\item vertical tab ({\tt"\\v"})
					\item form-feed ({\tt"\\f"})
				\end{itemize}
		*/
		static const char* CHARACTER_CLASS__WHITESPACE;
		//@}

		/** @name	Constructors and Destructors */
		/// Default Constructor
		String();

		/// Copy constructor 
		String(const String& string);

		/// STL string copy constructor
		String(const string& string);

		/** Creates a new string from a given range of another string.
				@see 		String:Indices
				@exception Exception::IndexUnderflow if {\tt from < 0}
				@exception Exception::IndexOverflow if {\tt from >= size()}
		*/
		String(const String& s, Index from, Size len = npos);
	
		/**	Creates a new string from a C type string.
				The new string contains the contents of {\bf s} until 
				it has reached a length of {\bf len} or contains a zero character
				(whichever comes first). Default value for {\bf len} is {\bf npos}
				meaning as long as possible.
		*/
		String(const char* char_ptr, Index from = 0, Size len = npos);

		/**	Creates a string using {\bf sprintf}.
				This constructor creates a new string and sets its content
				to the result of a call to {\bf sprintf} using {\bf format} as a
				format string and all additional parameters as arguments.\\
				The result of the sprintf call is intermediately written to a buffer
				of a maximum soze of {\bf buffer_size} characters, so choose an 
				appropriate size for this variables.\\
				@exception IndexUnderflow, if the buffer size specified is not larger than 0
				@exception NullPointer, if {\tt format == 0}
		*/
		String(Size buffer_size, const char* format, ... )
			throw(Exception::IndexUnderflow, Exception::NullPointer);

		/**	Create a new string from the contents of a {\bf strstream}.
				The contents of the {\tt strstream} are not modified, i.e.
				successive construction of multiple strings from the same {\tt strstream}
				object leads to identical copies.
		*/
		String(std::strstream& s);

		/** Creates a new string from len copies of c.
		*/
		String(const char c, Size size = 1);

		/// Creates a string just containing an unsigned character
		String(const unsigned char uc);

		/// Constructs a String from a short
		String(short s);

		/// Constructs a String from an unsigned short
		String(unsigned short us);

		/// Constructs a String from an int
		String(int i);

		/// Constructs a String from an unsigned int
		String(unsigned int ui);

		/// Constructs a String from a long
		String(long l);

		/// Constructs a String from an unsigned long
		String(unsigned long );

		/// Constructs a String from a float value
		String(float f);

		/// Constructs a String from a double value
		String(double d);

		/// Destructor
		virtual ~String();

		/// Clears the string
		void destroy();


		/**	@name	Assignment methods */
		//@{

		/**	Assign a String from a range of another string
				@exception Exception::IndexOverflow if {\tt from < 0}
				@exception Exception::IndexUnderflow if {\tt from >= size()}
		*/
		void set(const String& string, Index from = 0, Size len = npos)
			throw(Exception::IndexUnderflow, Exception::IndexOverflow);

		/** Assign a String from a C type string 
				@exception Exception::IndexUnderflow if {\tt from < 0}
				@exception Exception::IndexOverflow if {\tt from >= size()}
		*/
		void set(const char* char_ptr, Index from = 0, Size len = npos)
			throw(Exception::NullPointer, Exception::IndexUnderflow, Exception::IndexOverflow);
	
		/** Assign a string to the result of a {\bf sprintf} call
				@exception Exception::IndexUnderflow, if the buffer size is zero
				@exception Exception::NullPointer, {\tt format} is a NULL pointer
		*/
		void set(Size buffer_size, const char *format, ...)
				throw(Exception::IndexUnderflow, Exception::NullPointer);

		/** Assign a String from a {\bf strstream}.
				The contents of the {\tt strstream} object are not modified.
		*/
		void set(std::strstream& s);

		/// Assign a String from the result of repeating {\bf c} {\bf len} times
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

		/// Assign a String from a float value
		void set(float f);

		/// Assign a String from a float value
		void set(double d);

		/// Assign a C type string
		void get(char* char_ptr, Index from = 0, Size len = npos) const
			throw(Exception::NullPointer, Exception::IndexUnderflow, Exception::IndexOverflow);

		/// Assign a String from another String
		String& operator = (const String& s);

		/// Assign a String from a C type string
		String& operator = (const char* pc)
			throw(Exception::NullPointer);

		/** Assign a string from a {\bf strstream}.
				The contents of the {\tt strstream} obejct are not modified.
		*/
		String& operator = (std::strstream& s);

		/// Assign a String from a single char
		String& operator = (char c);

		/// Assign a String from an unsigned char
		String& operator = (unsigned char uc);

		/// Assign a String from a short
		String& operator = (short s);

		/// Assign a String from ans unsigned short
		String& operator = (unsigned short us);

		/// Assign a String from an int
		String& operator = (int i);

		/// Assign a String from an unsigned int
		String& operator = (unsigned int ui);

		/// Assign a String from a long
		String& operator = (long l);

		/// Assign a String from an unsigned long
		String& operator = (unsigned long );

		/// Assign a String from a float
		String& operator = (float f);

		/// Assign a String from a double
		String& operator = (double d);

		//@}
		

		/// 
		static void setCompareMode(CompareMode compare_mode);

		///
		static CompareMode getCompareMode();

		/** @name Converters */
		//@{

		/**	Converts the string to a bool value.
				This method returns {\bf false}, if the string contains the string {\tt false}
				(may be surrounded by whitespaces), or {\bf true} otherwise.
		*/
		bool toBool() const;

		///	Returns the first character of the string
		char toChar() const;

		/// Returns the first character of the string converted to an unsigned char
		unsigned char toUnsignedChar() const;

		/// Evaluates the string to a short
		short toShort() const
			throw(Exception::InvalidFormat);

		/// Evaluates the string to an unsigned short
		unsigned short toUnsignedShort() const
			throw(Exception::InvalidFormat);

		/// Evaluates the string to an int
		int toInt() const
			throw(Exception::InvalidFormat);

		/// Evaluates the string to an unsigned int
		unsigned int toUnsignedInt() const
			throw(Exception::InvalidFormat);

		/// Evaluates the string to a long
		long toLong() const
			throw(Exception::InvalidFormat);

		/// Evaluates the string to an unsigned long
		unsigned long toUnsignedLong() const
			throw(Exception::InvalidFormat);

		///  Evaluates the string to a float
		float toFloat() const
			throw(Exception::InvalidFormat);

		/// Evaluates the string to a double
		double toDouble() const
			throw(Exception::InvalidFormat);
		//@}
	
		
		/**	@name	Case Conversion */
		//@{			

		/// Converts all characters in the given range to lower case
		void toLower(Index from = 0, Size len = npos)
			throw(Exception::IndexUnderflow, Exception::IndexOverflow);
			
		/// Converts all characters in the given range to upper case
		void toUpper(Index from = 0, Size len = npos)
			throw(Exception::IndexUnderflow, Exception::IndexOverflow);

		//@}

		/**	@name Substring Definition */
		//@{

		/// Returns a substring
		Substring getSubstring(Index from = 0, Size len = npos) const
			throw(Exception::IndexUnderflow, Exception::IndexOverflow);

		/// Returns a substring
		Substring operator () (Index from, Size len = npos) const
			throw(Exception::IndexUnderflow, Exception::IndexOverflow);

		/** Returns a substring containing the string before the first occurence of {\bf s}
		*/
		Substring before(const String& s, Index from = 0) const;
 
		/** Returns a substring containing the beginning of the string including the first occurence of {\bf s}
		*/
		Substring through(const String& s, Index from = 0) const;
 
		/** Returns a substring containing the string from the first occurence of {\bf s} on
		*/
		Substring from(const String& s, Index from = 0) const;

		/** Returns a substring containing the string after the first occurence of {\bf s}.
		*/
		Substring after(const String& s, Index from = 0) const;

		//@}
 
		/**	@name	AWK style field operations */
		//@{

		/// count the fields that are separated by a defined set of delimiters
		Size countFields(const char* delimiters = CHARACTER_CLASS__WHITESPACE) const
			throw(Exception::NullPointer);

		/// Returns a given field as a substring
		String getField(Index index, const char* delimiters = CHARACTER_CLASS__WHITESPACE, Index* from = 0) const
			throw(Exception::IndexUnderflow, Exception::NullPointer);

		/// Split the string into fields and assign these field to an array of strings
		Size split(String string_array[], Size array_size, const char* delimiters = CHARACTER_CLASS__WHITESPACE, Index from = 0) const
			throw(Exception::IndexUnderflow, Exception::NullPointer);

		//@}


		/**	@name BASIC style string operations */
		//@{


		/** Strips all characters in {\bf trimmed} from the left of the string.
				trimLeft stops at the first character encountered that is not in {\bf trimmed}.
				Using its default parameter CHARACTER_CLASS__WHITESPACE, it is usually handy to 
				remove blanks from the beginning of a string.
				Strings consisting of character from {\tt trimmed} only yield an empty string.
		*/
		String& trimLeft(const char* trimmed = CHARACTER_CLASS__WHITESPACE);

		/** Strips all characters in {\bf trimmed} from the right of the string.
				trimRight stops at the first character encountered that is not in {\bf trimmed}.
				Using its default parameter CHARACTER_CLASS__WHITESPACE, it is usually handy to 
				remove blanks from the end of a string.
				Strings consisting of character from {\tt trimmed} only yield an empty string.
		*/
		String& trimRight(const char* trimmed_chars = CHARACTER_CLASS__WHITESPACE);

		/**	Strips all characters in {\bf trimmed} from both sides of the string.
				trim calls {\tt trimRight(trimmed).trimLeft(trimmed)}.
		*/
		String& trim(const char* trimmed_chars = CHARACTER_CLASS__WHITESPACE);

		/// Truncate the string to length {\bf size}
		String& truncate(Size size);

		/// Returns a substring containing the {\bf len} leftmost characters of the string
		Substring left(Size len) const;

		/// Returns a substring containing the {\bf len} rightmost characters of the string
		Substring right(Size len) const;

		/** Returns a substring containing the first occurence of {\bf pattern} in the string.
				If the pattern is not contained in the string, an empty Substring is returned.
				The search for the pattern may also start from an index different from zero, allowing
				incremental search.
				@return	Substring containing the search pattern, empty if not found
				@param	pattern the search pattern
				@from		the index in the string to start the search from
		*/
		Substring instr(const String& pattern, Index from = 0) const;

		//@}


		/**	@name	String Operations  */
		//@{
		///	Concatenates two strings
		String operator + (const string& string) const;

		/// Concatenates a string and a C type string
		String operator + (const char* char_ptr) const;

		/// Concatenates a string and a character
		String operator + (char c) const;

		/// Concatenates a C type string and a string
		friend String operator + (const char* char_ptr, const String& s);

		/// Concatenates a character and a string
		friend String operator + (char c, const String& s);

		/// Swaps the contents with another String
		void swap(String& s);

		/** Reverses the string.
				If called without arguments, this method simply reverses the character sequence of the string.
				By giving arguments for the indices, only a subsequence of the string may be reversed.
				@param	from first index of the sequence to be reversed
				@param	to last index of the sequence to be reversed
				@see		String:Indices
		*/
		String& reverse(Index from = 0, Size len = npos)
			throw(Exception::IndexUnderflow, Exception::IndexOverflow);

		/// Substitutes the first occurence of {\bf to_replace} by the content of {\bf replacing}
		Index substitute(const String& to_replace, const String& replacing);

		//@}
			

		/**	@name	Predicates */
		//@{

		/// True, if the string contains character {\bf c}
		bool has(char c) const;

		/// True, if the string contains the substring {\bf s} after index {\bf from}
		bool hasSubstring(const String& s, Index from = 0) const;

		/// True, if the string starts with {\bf s}
		bool hasPrefix(const String& s) const;

		/// True, if the string ends with {\bf s}
		bool hasSuffix(const String& s) const;

		/// True, if the string has size 0
		bool isEmpty() const;

		/** True, if the string only contains letters (any case).	
				It returns also {\bf true}, if called for an empty string.
		*/
		bool isAlpha() const;

		/** True, if the string only contains letters and digits.
				It returns also {\bf true}, if called for an empty string.
		*/
		bool isAlnum() const;

		/** True, if the string only contains digits.
				It returns also {\bf true}, if called for an empty string.
		*/
		bool isDigit() const;

		/** True, if the string is a floating number.
				(It contains only numbers and maybe a dot).
				It returns also {\bf true}, if called for an empty string.
		*/
		bool isFloat() const;

		/** True, if the string only contains spaces.
				It returns also {\bf true}, if called for an empty string.
		*/
		bool isSpace() const;

		/** True, if the string only contains whitespace characters.
				Whitespaces are defined in CHARACTER_CLASS__WHITESPACE.
				It returns also {\bf true}, if called for an empty string.
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

		/** True, if the character is any whitespace character
				Whitespaces are defined in CHARACTER_CLASS__WHITESPACE
		*/
		static bool isWhitespace(char c);

		//@}

		/**	@name	Comparators */
		//@{

		///
		int compare(const String& string, Index from = 0) const
			throw(Exception::IndexUnderflow, Exception::IndexOverflow);

		///
		int compare(const String& string, Index from, Size len) const
			throw(Exception::IndexUnderflow, Exception::IndexOverflow);


		///
		int compare(const char* char_ptr, Index from = 0) const
			throw(Exception::NullPointer, Exception::IndexUnderflow, Exception::IndexOverflow);

		///
		int compare(const char* char_ptr, Index from, Size len) const
				throw(Exception::NullPointer, Exception::IndexUnderflow, Exception::IndexOverflow);

		///
		int compare(char c, Index from = 0) const
			throw(Exception::IndexUnderflow, Exception::IndexOverflow);

		///
		bool operator == (const String& string) const;

		///
		bool operator != (const String& string) const;

		///
		bool operator < (const String& string) const;

		///
		bool operator <= (const String& string) const;

		///
		bool operator >= (const String& string) const;

		///
		bool operator > (const String& string) const;

		///
		friend bool operator == (const char* char_ptr, const String& string)
			throw(Exception::NullPointer);

		///
		friend bool operator != (const char* char_ptr, const String& string)
			throw(Exception::NullPointer);

		///
		friend bool operator < (const char* char_ptr, const String& string)
			throw(Exception::NullPointer);

		///
		friend bool operator <= (const char* char_ptr, const String& string)
			throw(Exception::NullPointer);
		
		///
		friend bool operator > (const char* char_ptr, const String& string)
			throw(Exception::NullPointer);

		///
		friend bool operator >= (const char* char_ptr, const String& string)
			throw(Exception::NullPointer);

		///
		bool operator == (const char* char_ptr) const
			throw(Exception::NullPointer);

		///
		bool operator != (const char* char_ptr) const
			throw(Exception::NullPointer);

		///
		bool operator < (const char* char_ptr) const
			throw(Exception::NullPointer);

		///
		bool operator <= (const char* char_ptr) const
			throw(Exception::NullPointer);

		///
		bool operator > (const char* char_ptr) const
			throw(Exception::NullPointer);

		///
		bool operator >= (const char* char_ptr) const
			throw(Exception::NullPointer);

		///
		friend bool operator == (char c, const String& string);

		///
		friend bool operator != (char c, const String& string);

		///
		friend bool operator < (char c, const String& string);

		///
		friend bool operator <= (char c, const String& string);
		
		///
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

		/**	@name	Debugging and Diagnostics */
		//@{

		///
		bool isValid() const;

		///
		void dump(::std::ostream& s = ::std::cout, Size depth = 0) const;

		//@}			



		/**	@name	Stream Operations 
		*/
		//@{

		///
		::std::istream& getline(::std::istream& s = ::std::cin, char delimiter = '\n');

		///
		friend ::std::istream& getline(::std::istream& s,  String& string,  char delimiter = '\n');
		//@}


		protected:
	
		// the validate...  methods check perform a thorough
		// index checking and an index translation
		// Indices below zero are interpreted as indices
		// relative to the end of the string
		// All methods throw IndexUnder|Overflow exceptions
		//
		void validateIndex_(Index& index) const
			throw(Exception::IndexUnderflow, Exception::IndexOverflow);
	
		void validateRange_(Index& from, Size& len) const
			throw(Exception::IndexUnderflow, Exception::IndexOverflow); 

		static void validateCharPtrRange_(Index& from, Size& len, const char* char_ptr)
			throw(Exception::NullPointer, Exception::IndexUnderflow, Exception::IndexOverflow);
		
		static void valudateCharPtrIndex_(Index& index);
		


		private:

		static int compareAscendingly_(const char* a,  const char* b);

		static int compareDescendingly_(const char* a,  const char* b);

		static CompareMode compare_mode_;
	};
	
	//@}

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/DATATYPE/string.iC>
#	endif

} // namespace BALL

#endif // BALL_DATATYPE_STRING_H
