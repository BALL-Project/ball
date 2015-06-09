// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DATATYPE/colorUnitHue.h>
#include <BALL/COMMON/rtti.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		ColorUnitHue::ColorUnitHue()
			:	value_((float)0)
		{
		}

		ColorUnitHue::ColorUnitHue
			(const ColorUnitHue& color_h_unit)
			:	value_(color_h_unit.value_)
		{
		}

		ColorUnitHue::ColorUnitHue(const char* char_ptr)
		{
			value_ = hexToFloat_(char_ptr);
		}

		ColorUnitHue::ColorUnitHue(const String& is)
		{
			value_ = hexToFloat_(is.c_str());
		}

		ColorUnitHue::ColorUnitHue(const short s)
		{
			if ((s < 0) || (s > 360))
			{
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__, s);
			}

			value_ = (float)s / (float)360;
		}

		ColorUnitHue::ColorUnitHue(const unsigned short s)
		{
      if (s > 360)
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__, s);
			}

			value_ = (float)s / (float)360;
		}

		ColorUnitHue::ColorUnitHue(const int i)
		{
      if ((i < 0) || (i > 360))
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__, (float) i);
			}

			value_ = (float)i / (float)360;
		}

		ColorUnitHue::ColorUnitHue(const unsigned int i)
		{
      if (i > 360)
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__, (float) i);
			}

			value_ = (float)i / (float)360;
		}

		ColorUnitHue::ColorUnitHue(const long l)
		{
      if ((l < 0) || (l > 360))
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__, l); 
			}

			value_ = (float)l / (float)360;
		}

		ColorUnitHue::ColorUnitHue(const unsigned long l)
		{
      if (l > 360)
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__, l);
			}

			value_ = (float)l / (float)360;
		}

		ColorUnitHue::ColorUnitHue(const float f)
		{
      if ((f < 0.0) || (f > 1.0))
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__, f);
			}

			value_ = f;
		}

		ColorUnitHue::ColorUnitHue(const double d)
		{
      if ((d < 0.0) || (d > 1.0))
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__, d);
			}

			value_ = (float)d;
		}

		ColorUnitHue::~ColorUnitHue()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this << " of class " << RTTI::getName<ColorUnitHue>() << endl;
			#endif 
		}

		void ColorUnitHue::clear()
		{
			value_ = (float)0;
		}

		void ColorUnitHue::set(const ColorUnitHue& color_h_unit)
		{
			value_ = color_h_unit.value_;
		}

		const ColorUnitHue& ColorUnitHue::operator = (const ColorUnitHue& color_h_unit)
		{
			set(color_h_unit);
			return *this;
		}

		void ColorUnitHue::set(const char* char_ptr)
		{
			value_ = hexToFloat_(char_ptr);
		}

		const ColorUnitHue& ColorUnitHue::operator = (const char* char_ptr)
		{
			set(char_ptr);
			return *this;
		}
			
		void ColorUnitHue::get(char* char_ptr) const
		{
			sprintf(char_ptr, "%x", (unsigned char)(value_ * 255.0));
		}

		void ColorUnitHue::set(const String& s)
		{
			value_ = hexToFloat_(s.c_str());
		}

		const ColorUnitHue& ColorUnitHue::operator = (const String& s)
		{
			set(s);
			return *this;
		}
			
		void ColorUnitHue::get(String& s) const
		{
			char temp[6];
			sprintf(&temp[0], "%x", (unsigned char)(value_ * 360.0));
			s.set(&temp[0]);
		}

		void ColorUnitHue::set(const short s)
		{
      if ((s < 0) || (s > 360))
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__, s);
			}

			value_ = (float)s / (float)360;
		}

		const ColorUnitHue& ColorUnitHue::operator =
			(const short s)
		{
			set(s);
			return *this;
		}

		void ColorUnitHue::get(short& s) const
		{
			s = (short)(value_ * 360.0);
		}

		void ColorUnitHue::set(const unsigned short s)
		{
      if (s > 360)
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__, s);
			}

			value_ = (float)s / (float)360;
		}

		const ColorUnitHue& ColorUnitHue::operator = (const unsigned short s)
		{
			set(s);
			return *this;
		}

		void ColorUnitHue::get(unsigned short& s) const
		{
			s = (unsigned short)(value_ * 360.0);
		}

		void ColorUnitHue::set(const int i)
		{
      if ((i < 0) || (i > 360))
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__, i);
			}

			value_ = (float)i / (float)360;
		}

		const ColorUnitHue& ColorUnitHue::operator = (const int i)
		{
			set(i);

			return *this;
		}

		void ColorUnitHue::get(int& i) const
		{
			i = (int)(value_ * 360.0);
		}

		void ColorUnitHue::set(const unsigned int i)
		{
      if (i > 360)
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__, i);
			}

			value_ = (float)i / (float)360;
		}

		const ColorUnitHue& ColorUnitHue::operator = (const unsigned int i)
		{
			set(i);

			return *this;
		}

		void ColorUnitHue::get(unsigned int& i) const
		{
			i = (unsigned int)(value_ * 360.0);
		}

		void ColorUnitHue::set(const long l)
		{
      if ((l < 0) || (l > 360))
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__, l); 
			}

			value_ = (float)l / (float)360;
		}

		const ColorUnitHue& ColorUnitHue::operator = (const long l)
		{
			set(l);

			return *this;
		}

		void ColorUnitHue::get(long& l) const
		{
			l = (long)(value_ * 360.0);
		}

		void ColorUnitHue::set(const unsigned long l)
		{
      if (l > 360)
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__, l);
			}

			value_ = (float)l / (float)360;
		}

		const ColorUnitHue& ColorUnitHue::operator = (const unsigned long l)
		{
			set(l);

			return *this;
		}

		void ColorUnitHue::get(unsigned long& l) const
		{
			l = (unsigned long)(value_ * 360.0);
		}

		void ColorUnitHue::set(const float f)
		{
      if ((f < 0.0) || (f > 1.0))
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__, f);
			}

			value_ = f;
		}

		const ColorUnitHue& ColorUnitHue::operator = (const float f)
		{
			set(f);

			return *this;
		}

		void ColorUnitHue::get(float& f) const
		{
			f = (float)(value_ * 360.0);
		}

		void ColorUnitHue::set(const double d)
		{
      if ((d < 0.0) || (d > 1.0))
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__, d);
			}

			value_ = (float)d;
		}

		const ColorUnitHue& ColorUnitHue::operator = (const double d)
		{
			set(d);

			return *this;
		}

		void ColorUnitHue::get(double& d) const
		{
			d = (double)(value_ * 360.0);
		}

		void ColorUnitHue::swap(ColorUnitHue& color_h_unit)
		{
			float temp = value_;
			value_ = color_h_unit.value_;
			color_h_unit.value_ = temp;
		}

		void ColorUnitHue::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "hunit : " << (int)(value_ * 360.0) << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		istream& operator >> (istream& s, ColorUnitHue& color_h_unit)
		{
			s >> color_h_unit.value_;

			return s;
		}

		ostream& operator << (ostream& s, const ColorUnitHue& color_h_unit)
		{
			s << (int)(color_h_unit.value_ * 360.0);
			
			return s;
		}

		float ColorUnitHue::hexToFloat_(const char* char_ptr)
		{
			int number = 0;

			#ifdef BALL_VIEW_DEBUG
			if (strlen(char_ptr) > 3)
			{
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__, strlen(char_ptr));
			}
			#endif

			if (strlen(char_ptr) == 2)
			{
				char c = char_ptr[0];
				
				if (c >= '0' && c <= '9')
				{
					number += (c - 48) * 16;
				}
				else if (c >= 'a' && c <= 'f')
				{
					number += (c - 87) * 16;
				}
				else if (c >= 'A' && c <= 'F')
				{
					number += (c - 55) * 16;
				}
				else
				{
					throw ColorUnit::NotInHexFormat(__FILE__, __LINE__, char_ptr);
				}

				c = char_ptr[1];

				if (c >= '0' && c <= '9')
				{
					number += (c - 48);
				}
				else if (c >= 'a' && c <= 'f')
				{
					number += (c - 97) + 10;
				}
				else if (c >= 'A' && c <= 'F')
				{
					number += (c - 65) + 10;
				}
				else
				{
					throw ColorUnit::NotInHexFormat(__FILE__, __LINE__, char_ptr);
				}
			}
			else // length = 3 ( number up to 168h)
			{
				char c = char_ptr[0];
				
				if (c >= '0' && c <= '1')
				{
					number += (c - 48) * 16 * 16;
				}
				else
				{
					throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__, c);
				}

				c = char_ptr[1];
				
				if (c >= '0' && c <= '6')
				{
					number += (c - 48) * 16;
				}
				else
				{
					throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__, c);
				}

				c = char_ptr[2];

				if (c >= '0' && c <= '8')
				{
					number += (c - 48);
				}
				else if (c >= 'a' && c <= 'f' && char_ptr[1] < '6')
				{
					number += (c - 97) + 10;
				}
				else if (c >= 'A' && c <= 'F' && char_ptr[1] < '6')
				{
					number += (c - 65) + 10;
				}
				else
				{
					throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__, c); 
				}
			}
			
			return (float)number / (float)360;
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorUnitHue.iC>
#		endif 

	} // namespace VIEW
} // namespace BALL
