// $Id: color.h,v 1.2 1999/12/19 17:19:22 oliver Exp $

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#define BALL_VIEW_DATATYPE_COLOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORUNIT_H
#	include <BALL/VIEW/DATATYPE/colorUnit.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORUNITHUE_H
#	include <BALL/VIEW/DATATYPE/colorUnitHue.h>
#endif

#include <iostream>
#include <stdio.h>

namespace BALL 
{
	namespace VIEW 
	{

		// forward declarations 
		class ColorRGBA;
		class ColorHSV;


		/**
		*/
		class ColorRGBA
		{
			friend class ColorHSV;

			public:

			/**	@name Enums
			*/
			//@{
			enum Component
			{
				COMPONENT__RED    = 0,
				COMPONENT__GREEN  = 1,
				COMPONENT__BLUE   = 2,
				COMPONENT__ALPHA  = 3
			};

			enum ErrorCode
			{
				ERROR__NONE                        = -1,
				ERROR__UNKOWN                      = 0,
				ERROR__NOT_IMPLEMENTED             = 1,
				ERROR__STRING_HAS_INCORRECT_LENGTH = 2,

				NUMBER_OF_COLORRGBA_ERRORS
			};
			//@}

			/**	@name	Constructors and Destructors
			*/	
			//@{

			ColorRGBA();

			ColorRGBA(const ColorRGBA& color, bool deep = true);

			ColorRGBA(const ColorHSV& color);

			ColorRGBA(const char* value);

			ColorRGBA(const String& value);

			ColorRGBA
				(const ColorUnit& red, const ColorUnit& green,
				 const ColorUnit& blue, const ColorUnit& alpha = (float)1);

			virtual ~ColorRGBA();

			virtual void clear();

			virtual void destroy();
			//@}
				
			/**	@name	Converters
			*/
			//@{

			operator String() const;
			//@}

			/**	@name	Assignment
			*/
			//@{

			void set(const ColorRGBA& color, bool deep = true);

			ColorRGBA& operator = (const ColorRGBA& color);

			void get(ColorRGBA& color, bool deep = true) const;

			void set(const ColorHSV& color, bool deep = true);

			ColorRGBA& operator = (const ColorHSV& color);

			void get(ColorHSV& color, bool deep = true) const;

			void set(const char* value, bool deep = true);

			ColorRGBA& operator = (const char* value);

			void get(char* value, bool deep = true) const;

			void set(const String& value, bool deep = true);

			ColorRGBA& operator = (const String& value);

			void get(String& value, bool deep = true) const;

			void swap(ColorRGBA& color);
			//@}

			/**	@name	Accessors
			*/
			//@{

			void setRed(const ColorUnit& red);

			ColorUnit& red();

			const ColorUnit& red() const;

			void setGreen(const ColorUnit& green);

			ColorUnit& green();

			const ColorUnit& green() const;

			void setBlue(const ColorUnit& blue);

			ColorUnit& blue();

			const ColorUnit& blue() const;

			void setAlpha(const ColorUnit& alpha);

			ColorUnit& alpha();

			const ColorUnit& alpha() const;

			void set
				(const ColorUnit& red, const ColorUnit& green,
				 const ColorUnit& blue, const ColorUnit& alpha = (float)1);

			void get
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue, ColorUnit& alpha) const;

			void get
				(ColorUnit& red,
				 ColorUnit& green,
				 ColorUnit& blue) const;
			 
			void setHSV
				(const ColorUnitHue& hue,
				 const ColorUnit& saturation,
				 const ColorUnit& value);

			void getHSV
				(ColorUnitHue& hue,
				 ColorUnit& saturation,
				 ColorUnit& value);
			//@}

			/**	@name	Predicates
			*/
			//@{

			bool operator == (const ColorRGBA& color) const;

			bool operator !=  (const ColorRGBA& color) const;

			bool operator < (const ColorRGBA& color) const;

			bool operator <= (const ColorRGBA& color) const;

			bool operator > (const ColorRGBA& color) const;

			bool operator >= (const ColorRGBA& color) const;
			//@}
				
			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, unsigned long depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;

			friend std::istream& operator >> (std::istream& s, ColorRGBA& color);

			friend std::ostream& operator << (std::ostream& s, const ColorRGBA& color);
			//@}

			private:

			ColorUnit red_;
			ColorUnit green_;
			ColorUnit blue_;
			ColorUnit alpha_;

			void stringToRGBA_(const String& s);
		};



		/**
		*/
		class ColorHSV
		{
			friend class ColorRGBA;

			public:

			enum Component
			{
				COMPONENT__HUE         = 0,
				COMPONENT__SATURATION  = 1,
				COMPONENT__VALUE       = 2
			};

			enum ErrorCode
			{
				ERROR__NONE                        = -1,
				ERROR__UNKOWN                      = 0,
				ERROR__NOT_IMPLEMENTED             = 1,
				ERROR__STRING_HAS_INCORRECT_LENGTH = 2,

				NUMBER_OF_COLORHSV_ERRORS
			};

			/**	@name	Constructors and Destructors
			*/
			//@{

			ColorHSV();

			ColorHSV(const ColorHSV& color, bool deep = true);

			ColorHSV(const ColorRGBA& color);

			ColorHSV(const char* value);

			ColorHSV(const String& value);

			ColorHSV
				(const ColorUnitHue& hue,
				 const ColorUnit& saturation,
				 const ColorUnit& value);

			virtual ~ColorHSV();

			virtual void clear();

			virtual void destroy();
			//@}

			/**	@name	Converters
			*/
			//@{

			operator String () const;
			//@}

			/**	@name	Assignment	
			*/
			//@{

			void set(const ColorHSV& color, bool deep = true);

			ColorHSV& operator = (const ColorHSV& color);

			void get(ColorHSV& color, bool deep = true) const;

			void set(const ColorRGBA& color, bool deep = true);

			ColorHSV& operator = (const ColorRGBA& color);

			void get(ColorRGBA& color, bool deep = true) const;

			void set(const char* value, bool deep = true);

			ColorHSV& operator = (const char* value);

			void get(char* value, bool deep = true) const;

			void set(const String& value, bool deep = true);

			ColorHSV& operator = (const String& value);

			void get(String& value, bool deep = true) const;

			friend ColorHSV operator + (const ColorHSV& a, const ColorHSV& b);

			void swap(ColorHSV& color);
			//@}

			/**	@name	Accessors
			*/
			//@{

			void setHue(const ColorUnitHue& hue);

			ColorUnitHue& hue();

			const ColorUnitHue& hue() const;

			void setSaturation(const ColorUnit& saturation);

			ColorUnit& saturation();

			const ColorUnit& saturation() const;

			void setValue(const ColorUnit& value);

			ColorUnit& value();

			const ColorUnit& value() const;

			void set
				(const ColorUnitHue& hue,
				 const ColorUnit& saturation,
				 const ColorUnit& value);

			void get
				(ColorUnitHue& hue,
				 ColorUnit& saturation,
				 ColorUnit& value) const;

			void setRGB
				(const ColorUnit& red,
				 const ColorUnit& green,
				 const ColorUnit& blue);

			void getRGB
				(ColorUnit& red,
				 ColorUnit& green,
				 ColorUnit& blue);
			//@}
			
			/**	@name	Predicates
			*/
			//@{

			bool operator == (const ColorHSV& color) const;

			bool operator != (const ColorHSV& color) const;

			bool operator < (const ColorHSV& color) const;

			bool operator <= (const ColorHSV& color) const;

			bool operator > (const ColorHSV& color) const;

			bool operator >= (const ColorHSV& color) const;
			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{
	
			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, unsigned long depth = 0) const;

			//@}

			/**	@name	Storers
			*/
			//@{
			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;

			friend std::istream& operator >>
				(std::istream& s, ColorHSV& color);

			friend std::ostream& operator <<
				(std::ostream& s, const ColorHSV& color);
			//@}

			private:

			void calculateHSV_
				(const ColorUnit red,
				 const ColorUnit green,
				 const ColorUnit blue);

			void calculateRGB_
				(ColorUnit& red,
				 ColorUnit& green,
				 ColorUnit& blue) const;

			void stringToHSV_
				(const String& value);

			ColorUnitHue hue_;
			ColorUnit saturation_;
			ColorUnit value_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/color.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // <BALL/VIEW_DATATYPE_COLOR_H
