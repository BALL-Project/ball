// $Id: colorUnitHue.C,v 1.2 1999/12/19 17:15:34 oliver Exp $

#include <BALL/VIEW/DATATYPE/colorUnitHue.h>
#include <BALL/VIEW/DATATYPE/colorUnit.h>
#include <stdio.h>

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
			(const ColorUnitHue& color_h_unit, bool /* deep */)
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
			#ifdef BALL_VIEW_DEBUG

			if ((s < 0) || (s > 360))
			{
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}
				
			#endif

			value_ = (float)s / (float)360;
		}

		ColorUnitHue::ColorUnitHue(const unsigned short s)
		{
			#ifdef BALL_VIEW_DEBUG
					
      if (s > 360)
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}
 				
			#endif

			value_ = (float)s / (float)360;
		}

		ColorUnitHue::ColorUnitHue(const int i)
		{
			#ifdef BALL_VIEW_DEBUG
					
      if ((i < 0) || (i > 360))
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}
 				
			#endif

			value_ = (float)i / (float)360;
		}

		ColorUnitHue::ColorUnitHue(const unsigned int i)
		{
			#ifdef BALL_VIEW_DEBUG
					
      if (i > 360)
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}
 				
			#endif

			value_ = (float)i / (float)360;
		}

		ColorUnitHue::ColorUnitHue(const long l)
		{
			#ifdef BALL_VIEW_DEBUG
					
      if ((l < 0) || (l > 360))
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}
 				
			#endif

			value_ = (float)l / (float)360;
		}

		ColorUnitHue::ColorUnitHue(const unsigned long l)
		{
			#ifdef BALL_VIEW_DEBUG
					
      if (l > 360)
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}
				
			#endif

			value_ = (float)l / (float)360;
		}

		ColorUnitHue::ColorUnitHue(const float f)
		{
			#ifdef BALL_VIEW_DEBUG
					
      if ((f < 0.0) || (s > 1.0))
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}
				
			#endif

			value_ = f;
		}

		ColorUnitHue::ColorUnitHue(const double d)
		{
			#ifdef BALL_VIEW_DEBUG
					
      if ((d < 0.0) || (d > 1.0))
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}
 				
			#endif

			value_ = (float)d;
		}

		ColorUnitHue::ColorUnitHue(const long double ld)
		{
			#ifdef BALL_VIEW_DEBUG
					
      if ((ld < 0.0) || (ld > 1.0))
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}
 				
			#endif

			value_ = (float)ld;
		}

		ColorUnitHue::~ColorUnitHue()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " << RTTI<ColHUnit>::getName() << endl;
			#endif 

			destroy();
		}

		void ColorUnitHue::clear()
		{
			value_ = (float)0;
		}

		void ColorUnitHue::destroy()
		{
			clear();
		}

		void ColorUnitHue::set
			(const ColorUnitHue& color_h_unit, bool /* deep */)
		{
			value_ = color_h_unit.value_;
		}

		ColorUnitHue& ColorUnitHue::operator =
			(const ColorUnitHue& color_h_unit)
		{
			set(color_h_unit);

			return *this;
		}

		void ColorUnitHue::get
			(ColorUnitHue& color_h_unit, bool deep) const
		{
			color_h_unit.set(*this, deep);
		}

		void ColorUnitHue::set(const char* char_ptr)
		{
			value_ = hexToFloat_(char_ptr);
		}

		ColorUnitHue& ColorUnitHue::operator = (const char* char_ptr)
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

		ColorUnitHue& ColorUnitHue::operator = (const String& s)
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
			#ifdef BALL_VIEW_DEBUG
					
      if ((s < 0) || (s > 360))
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}
 				
			#endif

			value_ = (float)s / (float)360;
		}

		ColorUnitHue& ColorUnitHue::operator =
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
			#ifdef BALL_VIEW_DEBUG
					
      if (s > 360)
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}
 				
			#endif

			value_ = (float)s / (float)360;
		}

		ColorUnitHue& ColorUnitHue::operator = (const unsigned short s)
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
			#ifdef BALL_VIEW_DEBUG
					
      if ((i < 0) || (i > 360))
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}
 				
			#endif

			value_ = (float)i / (float)360;
		}

		ColorUnitHue& ColorUnitHue::operator = (const int i)
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
			#ifdef BALL_VIEW_DEBUG
					
      if (i > 360)
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}
 				
			#endif

			value_ = (float)i / (float)360;
		}

		ColorUnitHue& ColorUnitHue::operator = (const unsigned int i)
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
			#ifdef BALL_VIEW_DEBUG
					
      if ((l < 0) || (l > 360))
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}
 				
			#endif

			value_ = (float)l / (float)360;
		}

		ColorUnitHue& ColorUnitHue::operator = (const long l)
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
			#ifdef BALL_VIEW_DEBUG
					
      if (l > 360)
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}
 				
			#endif

			value_ = (float)l / (float)360;
		}

		ColorUnitHue& ColorUnitHue::operator = (const unsigned long l)
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
			#ifdef BALL_VIEW_DEBUG
					
      if ((f < 0.0) || (f > 1.0))
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}
 				
			#endif

			value_ = f;
		}

		ColorUnitHue& ColorUnitHue::operator = (const float f)
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
			#ifdef BALL_VIEW_DEBUG
					
      if ((d < 0.0) || (d > 1.0))
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}
 				
			#endif

			value_ = (float)d;
		}

		ColorUnitHue& ColorUnitHue::operator = (const double d)
		{
			set(d);

			return *this;
		}

		void ColorUnitHue::get(double& d) const
		{
			d = (double)(value_ * 360.0);
		}

		void ColorUnitHue::set(const long double ld)
		{
			#ifdef BALL_VIEW_DEBUG
					
      if ((ld < 0.0) || (ld > 1.0))
      {
        throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}
 				
			#endif

			value_ = (float)ld;
		}

		ColorUnitHue& ColorUnitHue::operator = (const long double ld)
		{
			set(ld);

			return *this;
		}

		void ColorUnitHue::get(long double& ld) const
		{
			ld = (long double)(value_ * 360.0);
		}
				
		void ColorUnitHue::swap(ColorUnitHue& color_h_unit)
		{
			float temp = value_;
			value_ = color_h_unit.value_;
			color_h_unit.value_ = temp;
		}

		bool ColorUnitHue::isValid() const
		{
			return true;
		}

		void ColorUnitHue::dump
			(ostream& s, unsigned long depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "hunit : " << (int)(value_ * 360.0) << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void ColorUnitHue::read(istream& /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void ColorUnitHue::write(ostream& /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		istream& operator >>
			(istream& s, ColorUnitHue& color_h_unit)
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
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
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
					throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
				}

				c = char_ptr[1];
				
				if (c >= '0' && c <= '6')
				{
					number += (c - 48) * 16;
				}
				else
				{
					throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
				}

				c = char_ptr[2];

				if (c >= '0' && c <= '8')
				{
					number += (c - 48);
				}
				else
				{
					throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
				}
			}
			
			return (float)number / (float)360;
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorUnitHue.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
