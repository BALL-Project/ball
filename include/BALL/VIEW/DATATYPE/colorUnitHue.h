// $Id: colorUnitHue.h,v 1.1 1999/08/26 07:53:14 oliver Exp $

#ifndef BALL_VIEW_DATATYPE_COLORUNITHUE_H
#define BALL_VIEW_DATATYPE_COLORUNITHUE_H

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

		class ColorUnitHue
		{
			public:
			
			/**	@name	Constructors and Destructors
			*/
			//@{

			ColorUnitHue();
		
			ColorUnitHue
				(const ColorUnitHue& colcor_h_unit, bool deep = true);

			ColorUnitHue
				(const char* char_ptr);

			ColorUnitHue
				(const String& is);

			ColorUnitHue
				(const short s);

			ColorUnitHue
				(const unsigned short s);

			ColorUnitHue
				(const int i);
		
			ColorUnitHue
				(const unsigned int i);

			ColorUnitHue
				(const long l);

			ColorUnitHue
				(const unsigned long l);
		
			ColorUnitHue
				(const float f);
		
			ColorUnitHue
				(const double d);

			ColorUnitHue
				(const long double ld);

			virtual ~ColorUnitHue();

			virtual void clear();

			virtual void destroy();
			//@}
				
			/**	@name	Converters
			*/
			//@{
			operator String () const;

			operator short () const;
			
			operator unsigned short () const;
			
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
			
			void set(const ColorUnitHue& color_unit_hue, bool deep = true);

			ColorUnitHue& operator = (const ColorUnitHue& color_unit_hue);
		
			void get(ColorUnitHue& color_unit_hue, bool deep = true) const;

			void set(const char* value);

			ColorUnitHue& operator = (const char* value);
		
			void get(char* value) const;

			void set(const String& s);

			ColorUnitHue& operator = (const String& s);
		
			void get(String& s) const;

			void set(const short s);

			ColorUnitHue& operator = (const short value);

			void get(short& value) const;

			void set(const unsigned short value);

			ColorUnitHue& operator = (const unsigned short value);

			void get(unsigned short& value) const;

			void set(const int value);

			ColorUnitHue& operator = (const int value);

			void get(int& value) const;

			void set(const unsigned int value);

			ColorUnitHue& operator = (const unsigned int value);

			void get(unsigned int& value) const;

			void set(const long value);

			ColorUnitHue& operator = (const long value);

			void get(long& value) const;

			void set(const unsigned long value);

			ColorUnitHue& operator = (const unsigned long value);

			void get(unsigned long& value) const;

			void set(const float value);

			ColorUnitHue& operator = (const float init);

			void get(float& value) const;

			void set(const double value);

			ColorUnitHue& operator = (const double value);

			void get(double& value) const;

			void set(const long double value);

			ColorUnitHue& operator = (const long double value);

			void get(long double& value) const;
			
			void swap(ColorUnitHue& color_unit_hue);
			//@}

			/**	@name	Predicates
			*/
			//@{

			bool operator ==
				(const ColorUnitHue& color_unit_hue) const;

			bool operator !=
				(const ColorUnitHue& color_unit_hue) const;
			
			bool operator <
				(const ColorUnitHue& color_unit_hue) const;
			
			bool operator <=
				(const ColorUnitHue& color_unit_hue) const;
			
			bool operator >
				(const ColorUnitHue& color_unit_hue) const;

			bool operator >=
				(const ColorUnitHue& color_unit_hue) const;
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

			virtual void read
				(istream& s);

			virtual void write
				(ostream& s) const;

			friend istream& operator >>
				(istream& s, ColorUnitHue& color_unit_hue);

			friend ostream& operator <<
				(ostream& s, const ColorUnitHue &color_unit_hue);
			//@}

			private:
			
			float hexToFloat_(const char* string);
			
			float value_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorUnitHue.iC>
#		endif

	} // namspace VIEW
	
} // namespace BALL

#endif // BALL_VIEW_DATATYPE_COLORUNITHUE_H
