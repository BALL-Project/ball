// $Id: regExp.h,v 1.8 2000/10/05 08:27:56 oliver Exp $

#ifndef BALL_DATATYPE_REGEXP_H
#define BALL_DATATYPE_REGEXP_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

// for SGI CC: turn of warnings #1174: function "regcomp" was declared but never referenced
#if !defined(__GNUC__) && !defined(__KAI__) && defined(IRIX)
#	pragma set woff 1174
#endif
#include <sys/types.h>
#include <regex.h>
#if !defined(__GNUC__) && !defined(__KAI__) && defined(IRIX)
#	pragma reset woff 1174
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#define BALL_REGULAR_EXPRESSION_DEFAULT_PATTERN   ""

namespace BALL 
{

	/**	Regular expression class
	*/
	class RegularExpression
	{
		public:

		BALL_CREATE_DEEP(RegularExpression)

		/**	@name	String constants 
		*/
		//@{
			
		/**
		*/
		static const String ALPHA;
		/**
		*/
		static const String ALPHANUMERIC;
		/**
		*/
		static const String REAL;
		/**
		*/
		static const String IDENTIFIER;
		/**
		*/
		static const String INTEGER;
		/**
		*/
		static const String HEXADECIMAL_INTEGER;
		/**
		*/
		static const String LOWERCASE;
		/**
		*/
		static const String NON_ALPHA;
		/**
		*/
		static const String NON_ALPHANUMERIC;
		/**
		*/
		static const String NON_NUMERIC;
		/**
		*/
		static const String NON_WHITESPACE;
		/**
		*/
		static const String UPPERCASE;
		/**
		*/
		static const String WHITESPACE;
		//@}

		/**	@name	Constructors and Destructors 
		*/
		//@{
			
		/**	Default constructor.
				Creates a new RegularExpression object.
		*/
		RegularExpression();

		/**	
		*/
		RegularExpression(const RegularExpression& regular_expression, bool deep = true);

		/**	
		*/
		RegularExpression(const String& pattern, bool wildcard_pattern = false);

		/**	
		*/
		virtual ~RegularExpression();

		/**	
		*/
		virtual void clear();

		/**	
		*/
		void destroy();
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	
		*/
		void set(const RegularExpression& regular_expression, bool deep = true);

		/**	
		*/
		void set(const String& pattern, bool wildcard_pattern = false);

		/**	
		*/
		void get(RegularExpression& regular_expression, bool deep = true) const;
		//@}

		/**	
		*/
		const String& getPattern() const;

		/**	
		*/
		Size countSubexpressions() const;

		/**	
		*/
		static bool match
			(const char* text, const char* pattern,
			 int compile_flags = 0 | REG_EXTENDED | REG_NOSUB, int execute_flags = 0 );
		
		/**	
		*/
		bool match(const String& text, Index from = 0, int execute_flags = 0 ) const;
	
		/**	
		*/
		bool match(const Substring& text, Index from = 0, int execute_flags = 0) const;
	
		/**	
		*/
		bool match(const char* text, int execute_flags = 0) const;
	
		/**	
		*/
		static bool find
			(const char* text, const char* pattern,
			 const char** found_substring_from = 0,
			 const char** found_substring_to = 0,
			 int compile_flags = 0 | REG_EXTENDED,
			 int execute_flags = 0);
			
		/**	
		*/
		bool find
			(const String& text, Substring& found,
			 Index from = 0, int execute_flags = 0) const;
			
		/**	
		*/
		bool find
			(const String& text, Substring found_subexpression[],
			 Size number_of_subexpressions, Index from = 0, int execute_flags = 0) const;
			
		/**	
		*/
		bool find
			(const Substring& text, Substring& found,
			 Index from = 0, int execute_flags = 0) const;
			
		/**	
		*/
		bool find
			(const Substring& text, Substring found_subexpressions[],
			 Size number_of_subexpressions, Index from = 0, int execute_flags = 0) const;
			
		/**	
		*/
		bool find
			(const char* text, const char** found_substrings_from = 0,
			 const char** found_substring_to = 0, int execute_flags = 0) const;
			
		/**	@name	Predicates
		*/
		//@{

		/**	
		*/
		bool isEmpty() const;

		/**	
		*/
		bool operator == (const RegularExpression& regular_expression) const;

		/**	
		*/
		bool operator != (const RegularExpression& regular_expression) const;

		/**	
		*/
		bool operator < (const RegularExpression& regular_expression) const;

		/**	
		*/
		bool operator <= (const RegularExpression& regular_expression) const;

		/**	
		*/
		bool operator >= (const RegularExpression& regular_expression) const;

		/**	
		*/
		bool operator > (const RegularExpression& regular_expression) const;
		//@}

		/**	@name	Debugging and Diagnostics
		*/
		//@{
		/**	
		*/
		virtual bool isValid() const;

		/**	
		*/
		virtual void dump(::std::ostream& s = ::std::cout, Size depth = 0) const;
		//@}

		/**	@name Storeres
		*/
		//@{
		/**	
		*/
		friend ::std::ostream& operator << (::std::ostream& s, const RegularExpression& regular_expression);

		/**	
		*/
		friend ::std::istream& operator >> (::std::istream& s, RegularExpression& regular_expression);
		//@}
		
		private:

		void compilePattern_();

		void toExtendedRegularExpression_();

		regex_t regex_;
		String 	pattern_;
		bool 		valid_pattern_;
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/DATATYPE/regExp.iC>
#	endif

} // namespace BALL

#endif // BALL_DATATYPE_REGEXP_H
