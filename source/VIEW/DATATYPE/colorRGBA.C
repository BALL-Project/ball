// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DATATYPE/colorRGBA.h>
#include <BALL/VIEW/DATATYPE/colorHSV.h>
#include <QtGui/QColor>

using namespace std;

namespace BALL
{
	namespace VIEW
	{
		ColorRGBA BALL_SELECTED_COLOR(255, 255, 0, 255);
		PreciseTime  BALL_SELECTED_COLOR_CHANGE_TIME(PreciseTime::now());
		
		ColorRGBA::ColorRGBA()
			:	red_(),
				green_(),
				blue_(),
				alpha_((float)1)
		{
		}

		ColorRGBA::ColorRGBA(const ColorRGBA& color)
			:	red_(color.red_),
				green_(color.green_),
				blue_(color.blue_),
				alpha_(color.alpha_)
		{
		}
		 
		ColorRGBA::ColorRGBA(const ColorHSV& color_HSV)
			:	alpha_((float)1)
		{
			color_HSV.calculateRGB_(red_, green_, blue_);
		}
		 
		ColorRGBA::ColorRGBA(const char *char_ptr)
		{
			stringToRGBA_(String(char_ptr));
		}
		 
		ColorRGBA::ColorRGBA(const String &s)
		{
			stringToRGBA_(s);
		}
		 
		ColorRGBA::ColorRGBA(const ColorUnit& red, const ColorUnit& green,
												 const ColorUnit& blue, const ColorUnit& alpha)
			:	red_(red),
				green_(green),
				blue_(blue),
				alpha_(alpha)
		{
		}

		ColorRGBA::ColorRGBA(const QColor& color)
		{
			set(color);
		}

		ColorRGBA::ColorRGBA(const QColor& color, const ColorUnit& alpha)
		{
			set(color);
			setAlpha(alpha);
		}
		 
		ColorRGBA::~ColorRGBA()
		{
			#ifdef BALL_VIEW_DEBUG
			Log.error() << "Destructing object " << this << " of class ColorRGBA" << endl;
			#endif 
		}

		void ColorRGBA::clear()
		{
			red_ = green_ = blue_ = (float)0;
			alpha_ = ((float)1);
		}

		ColorRGBA::operator String() const
		{
			char temp[10];

			sprintf(&temp[0], "%02x%02x%02x%02x",
				(unsigned char)red_, (unsigned char)green_,
				(unsigned char)blue_, (unsigned char)alpha_);

			return String(&temp[0]);  
		}

		void ColorRGBA::set(const ColorRGBA& color)
		{
			red_ = color.red_;
			green_ = color.green_;
			blue_ = color.blue_;
			alpha_ = color.alpha_;
		}

		const ColorRGBA& ColorRGBA::operator = (const ColorRGBA& color)
		{
			set(color);
			return *this;
		}

		void ColorRGBA::set(const ColorHSV& color_HSV)
		{
			color_HSV.calculateRGB_(red_, green_, blue_);
			alpha_ = (float)1;
		}

		const ColorRGBA& ColorRGBA::operator = (const ColorHSV& color_HSV)
		{
			set(color_HSV);
			return *this;
		}

		void ColorRGBA::get(ColorHSV& color_HSV) const
		{
			color_HSV.set(*this);
		}

		void ColorRGBA::set(const char* char_ptr)
		{
			stringToRGBA_(String(char_ptr));
		}

		const ColorRGBA& ColorRGBA::operator = (const char* char_ptr)
		{
			set(char_ptr);
			return *this;
		}

		void ColorRGBA::get(char *char_ptr) const
		{
			sprintf(char_ptr, "%2X%2X%2X%2X",
				(unsigned char)red_, (unsigned char)green_,
				(unsigned char)blue_, (unsigned char)alpha_);

			for (Position i = 0; i < 7; i++)
			{
				if (char_ptr[i] == ' ') char_ptr[i] = '0';
			}

			char_ptr[8] = '\0';
		}

		void ColorRGBA::set(const String& s)
		{
			stringToRGBA_(s);
		}

		const ColorRGBA& ColorRGBA::operator = (const String& s)
		{
			set(s);
			return *this;
		}

		void ColorRGBA::get(String& s) const
		{
			char temp[9];

			sprintf(temp, "%2X%2X%2X%2X",
				(unsigned char)red_, (unsigned char)green_,
				(unsigned char)blue_, (unsigned char)alpha_);

			for (Position i = 0; i < 7; i++)
			{
				if (temp[i] == ' ') temp[i] = '0';
			}

			temp[8] = '\0';

			s.set(&temp[0]);
		}

		void ColorRGBA::get(QColor& color) const
		{
			color.setRgb(getRed(), getGreen(), getBlue());
		}

		QColor ColorRGBA::getQColor() const
		{
			QColor color;
			get(color);
			return color;
		}

		bool ColorRGBA::set(const QColor& color)
		{
			if (!color.isValid()) return false;

			set(color.red(), color.green(), color.blue(), color.alphaF());
			return false;
		}

		void ColorRGBA::swap(ColorRGBA& color)
		{
			red_.swap(color.red_);
			green_.swap(color.green_);
			blue_.swap(color.blue_);
			alpha_.swap(color.alpha_);
		}

		void ColorRGBA::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "color RGBA: " << endl;

			red_.dump(s, depth + 1);
			green_.dump(s, depth + 1);
			blue_.dump(s, depth + 1);
			alpha_.dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);  
		}

		BALL_VIEW_EXPORT istream& operator >> (istream& s, ColorRGBA& color)
		{
			char dummy[20];
			s >> dummy >> color.red_ 
				>> dummy >> color.green_ 
				>> dummy >> color.blue_ 
   			>> dummy >> color.alpha_
				>> dummy;
			
			return s;
		}

		BALL_VIEW_EXPORT ostream& operator << (ostream& s, const ColorRGBA& color)
		{
			s << "(" << (short)color.red_ << ","
					 << (short)color.green_ << ","
					 << (short)color.blue_ << ","
					 << (short)color.alpha_ << ")";
			
			return s;
		}

		void ColorRGBA::stringToRGBA_(const String& s)
		{
			int length = (int)s.size();

			#ifdef BALL_VIEW_DEBUG
					
				if ((length != 6) && (length != 8))
				{
					throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
				}
				
			#endif

			char buffer[5];
			char *char_ptr = (char *)s.c_str();
			
			buffer[2] = (char)0;
			
			buffer[0] = char_ptr[0];
			buffer[1] = char_ptr[1];
			
			red_ = buffer;
			
			buffer[0] = char_ptr[2];
			buffer[1] = char_ptr[3];
			
			green_ = buffer;
			
			buffer[0] = char_ptr[4];
			buffer[1] = char_ptr[5];

			blue_ = buffer;

			if (length > 6)
			{
				buffer[0] = char_ptr[6];
				buffer[1] = char_ptr[7];
				
				alpha_ = buffer;
			}
		}

		ColorRGBA ColorRGBA::getInverseColor() const
		{
			ColorRGBA color;
			color.set(255 - (Position) getRed(),
								255 - (Position) getGreen(),
								255 - (Position) getBlue());
			return color;
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorRGBA.iC>
#		endif

	} // namespace VIEW
} // namespace BALL
