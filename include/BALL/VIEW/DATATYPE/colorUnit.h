// $Id: colorUnit.h,v 1.1 1999/08/26 07:53:14 oliver Exp $

#ifndef BALL_VIEW_DATATYPE_COLORUNIT_H
#define BALL_VIEW_DATATYPE_COLORUNIT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#include <iostream.h>
#include <stdio.h>


namespace BALL
{
	namespace VIEW
	{

		/**
		*/
		class ColorUnit
		{
			public:
			
			/**	@name Constructors and Destructors
			*/
			//@{

			ColorUnit();
		
			ColorUnit(const ColorUnit& color_unit, bool deep = true);

			ColorUnit(const char* value);

			ColorUnit(const String& value);

			ColorUnit(const unsigned char value);

			ColorUnit(const short value);

			ColorUnit(const unsigned short value);

			ColorUnit(const int value);
		
			ColorUnit(const unsigned int value);

			ColorUnit(const long value);

			ColorUnit
				(const unsigned long value);
		
			ColorUnit
				(const float value);
		
			ColorUnit
				(const double value);

			ColorUnit
				(const long double value);

			virtual ~ColorUnit();

			virtual void clear();

			virtual void destroy();
				
			//@}

			/**	@name	Exceptions
			*/
			//@{
			
			class NotInHexFormat
				:	public Exception::GeneralException
			{
				public:

				NotInHexFormat(const char* file, int line, const string& data);
			};
			//@}
				
			//@}
			
			/**	@name	Converters
			*/
			//@{

			operator String () const;

			operator char () const;

			operator unsigned char () const;

			operator short () const;
			
			operator unsigned short	() const;
			
			operator int () const;

			operator unsigned int () const;

			operator long () const;

			operator unsigned long () const;

			operator float () const;

			operator double () const;
			
			operator long double () const;

			//@}

			/**	@name	Assignment
			*/
			//@{
			
			void set(const ColorUnit& color_unit, bool deep = true);

			ColorUnit& operator = (const ColorUnit& color_unit);
		
			void get(ColorUnit& color_unit, bool deep = true) const;

			void set(const char* value);

			ColorUnit& operator = (const char* value);
		
			void get(char* value) const;

			void set(const String& value);

			ColorUnit& operator = (const String& value);
		
			void get(String& value) const;

			void set(const unsigned char value);

			ColorUnit& operator = (const unsigned char value);

			void get(unsigned char& value) const;

			void set(const short value);

			ColorUnit& operator = (const short value);

			void get(short& value) const;

			void set(const unsigned short value);

			ColorUnit& operator = (const unsigned short value);

			void get(unsigned short& value) const;

			void set(const int value);

			ColorUnit& operator = (const int value);

			void get(int& value) const;

			void set(const unsigned int value);

			ColorUnit& operator = (const unsigned int value);

			void get(unsigned int& value) const;

			void set(const long value);

			ColorUnit& operator = (const long value);

			void get(long& value) const;

			void set(const unsigned long value);

			ColorUnit& operator = (const unsigned long value);

			void get(unsigned long& value) const;

			void set(const float value);

			ColorUnit& operator = (const float value);

			void get(float& value) const;

			void set(const double value);

			ColorUnit& operator = (const double value);

			void get(double& value) const;

			void set(const long double value);

			ColorUnit& operator = (const long double value);

			void get(long double& value) const;
			
			void swap(ColorUnit& color_unit);
			//@}

			/**	@name	Predicates
			*/
			//@{

			bool operator ==
				(const ColorUnit& color_unit) const;

			bool operator !=
				(const ColorUnit& color_unit) const;
			
			bool operator <
				(const ColorUnit& color_unit) const;
			
			bool operator <=
				(const ColorUnit& color_unit) const;
			
			bool operator >
				(const ColorUnit& color_unit) const;

			bool operator >=
				(const ColorUnit& color_unit) const;
			//@}
			
			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(ostream& s = cout, unsigned long depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(istream& s);

			virtual void write(ostream& s) const;

			friend istream& operator >>
				(istream& s, ColorUnit& color_unit);

			friend ostream& operator <<
				(ostream& s, const ColorUnit& color_unit);
			//@}

			private:
		
			float hexToFloat_(const char* value);
			
			float value_;
		
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorUnit.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_DATATYPE_COLORUNIT_H
