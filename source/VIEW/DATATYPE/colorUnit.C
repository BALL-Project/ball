// $Id: colorUnit.C,v 1.6 2001/05/13 14:28:35 hekl Exp $

#include <BALL/VIEW/DATATYPE/colorUnit.h>
#include <stdio.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		ColorUnit::NotInHexFormat::NotInHexFormat(const char* file, int line, const string& data)
			throw()
			:	Exception::GeneralException(file, line, string("NotInHexFormat"), string("value was not in hex format: ") + data)
		{
		}

		ColorUnit::ColorUnit()
			throw()
			:	value_((float)0)
		{
		}

		ColorUnit::ColorUnit(const ColorUnit& color)
			throw()
			:	value_(color.value_)
		{
		}

		ColorUnit::ColorUnit(const char *char_ptr)
			throw(Exception::InvalidRange, NotInHexFormat)
		{
			value_ = hexToFloat_(char_ptr);
		}

		ColorUnit::ColorUnit(const String& s)
			throw(Exception::InvalidRange, NotInHexFormat)
		{
			value_ = hexToFloat_(s.c_str());
		}

		ColorUnit::ColorUnit(const unsigned char c)
			throw()
		{
			value_ = (float)c / (float)255;
		}

		ColorUnit::ColorUnit(const short s)
			throw(Exception::InvalidRange)
		{
			if ((s < 0) || (s > 255))
			{
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}

			value_ = (float)s / (float)255;
		}

		ColorUnit::ColorUnit(const unsigned short us)
			throw(Exception::InvalidRange)
		{
			if (us > 255)
      {
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}

			value_ = (float)us / (float)255;
		}

		ColorUnit::ColorUnit(const int i)
			throw(Exception::InvalidRange)
		{
			if ((i < 0) || (i > 255))
      {
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}

			value_ = (float)i / (float)255;
		}

		ColorUnit::ColorUnit(const unsigned int ui)
			throw(Exception::InvalidRange)
		{
			if (ui > 255)
      {
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}

			value_ = (float)ui / (float)255;
		}

		ColorUnit::ColorUnit(const long l)
			throw(Exception::InvalidRange)
		{
			if ((l < 0) || (l > 255))
      {
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}

			value_ = (float)l / (float)255;
		}

		ColorUnit::ColorUnit(const unsigned long ul)
			throw(Exception::InvalidRange)
		{
			if (ul > 255)
      {
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}
 
			value_ = (float)ul / (float)255;
		}

		ColorUnit::ColorUnit(const float f)
			throw(Exception::InvalidRange)
		{
			if ((f < 0.0) || (f > 1.0))
      {
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}

			value_ = f;
		}

		ColorUnit::ColorUnit(const double d)
			throw(Exception::InvalidRange)
		{
			if ((d < 0.0) || (d > 1.0))
      {
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}

			value_ = (float)d;
		}

		ColorUnit::ColorUnit(const long double ld)
			throw(Exception::InvalidRange)
		{
			if ((ld < 0.0) || (ld > 1.0))
      {
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}

			value_ = (float)ld;
		}

		ColorUnit::~ColorUnit()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " 
						 << RTTI::getName<ColorUnit>() << endl;
			#endif 

			destroy();
		}

		void ColorUnit::clear()
			throw()
		{
			value_ = (float)0;
		}

		void ColorUnit::destroy()
			throw()
		{
		}

		void ColorUnit::set(const ColorUnit& color)
			throw()
		{
			value_ = color.value_;
		}

		const ColorUnit& ColorUnit::operator = (const ColorUnit& color)
			throw()
		{
			set(color);

			return *this;
		}

		void ColorUnit::get(ColorUnit& color) const
			throw()
		{
			color.set(*this);
		}

		void ColorUnit::set(const char* char_ptr)
			throw(Exception::InvalidRange, NotInHexFormat)
		{
			value_ = hexToFloat_(char_ptr);
		}

		const ColorUnit& ColorUnit::operator = (const char* char_ptr)
			throw(Exception::InvalidRange, NotInHexFormat)
		{
			set(char_ptr);

			return *this;
		}
			
		void ColorUnit::get(char* char_ptr) const
			throw()
		{
			sprintf(char_ptr, "%x", (unsigned char)(value_ * 255.0));
		}

		void ColorUnit::set(const String& s)
			throw(Exception::InvalidRange, NotInHexFormat)
		{
			value_ = hexToFloat_(s.c_str());
		}

		const ColorUnit& ColorUnit::operator = (const String& s)
			throw(Exception::InvalidRange, NotInHexFormat)
		{
			set(s);

			return *this;
		}
			
		void ColorUnit::get(String& values) const
			throw()
		{
			char temp[4];

			sprintf(&temp[0], "%x", (unsigned char)(value_ * 255.0));

			values.set(&temp[0]);
		}

		void ColorUnit::set(const unsigned char c)
			throw()
		{
			value_ = (float)c / (float)255;
		}

		const ColorUnit& ColorUnit::operator = (const unsigned char c)
			throw()
		{
			set(c);

			return *this;
		}

		void ColorUnit::get(unsigned char& c) const
			throw()
		{
			c = (unsigned char)(value_ * 255.0);
		}

		void ColorUnit::set(const short s)
			throw(Exception::InvalidRange)
		{
			if ((s < 0) || (s > 255))
			{
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}

			value_ = (float)s / (float)255;
		}

		const ColorUnit& ColorUnit::operator = (const short s)
			throw(Exception::InvalidRange)
		{
			set(s);

			return *this;
		}

		void ColorUnit::get(short& s) const
			throw()
		{
			s = (short)(value_ * 255.0);
		}

		void ColorUnit::set(const unsigned short us)
			throw(Exception::InvalidRange)
		{
			if (us > 255)
      {
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}

			value_ = (float)us / (float)255;
		}

		const ColorUnit& ColorUnit::operator = (const unsigned short us)
			throw(Exception::InvalidRange)
		{
			set(us);

			return *this;
		}

		void ColorUnit::get(unsigned short& us) const
			throw()
		{
			us = (unsigned short)(value_ * 255.0);
		}

		void ColorUnit::set(const int i)
			throw(Exception::InvalidRange)
		{
			if ((i < 0) || (i > 255))
			{
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}

			value_ = (float)i / (float)255;
		}

		const ColorUnit& ColorUnit::operator = (const int i)
			throw(Exception::InvalidRange)
		{
			set(i);

			return *this;
		}

		void ColorUnit::get(int& i) const
			throw()
		{
			i = (int)(value_ * 255.0);
		}

		void ColorUnit::set(const unsigned int ui)
			throw(Exception::InvalidRange)
		{
			if (ui > 255)
      {
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}

			value_ = (float)ui / (float)255;
		}

		const ColorUnit& ColorUnit::operator = (const unsigned int ui)
			throw(Exception::InvalidRange)
		{
			set(ui);

			return *this;
		}

		void ColorUnit::get(unsigned int& i) const
			throw()
		{
			i = (unsigned int)(value_ * 255.0);
		}

		void ColorUnit::set(const long l)
			throw(Exception::InvalidRange)
		{
			if ((l < 0) || (l > 255))
      {
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}

			value_ = (float)l / (float)255;
		}

		const ColorUnit& ColorUnit::operator = (const long l)
			throw(Exception::InvalidRange)
		{
			set(l);

			return *this;
		}

		void ColorUnit::get(long& l) const
			throw()
		{
			l = (long)(value_ * 255.0);
		}

		void ColorUnit::set(const unsigned long ul)
			throw(Exception::InvalidRange)
		{
			if (ul > 255)
      {
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}

			value_ = (float)ul / (float)255;
		}

		const ColorUnit& ColorUnit::operator = (const unsigned long ul)
			throw(Exception::InvalidRange)
		{
			set(ul);

			return *this;
		}

		void ColorUnit::get(unsigned long& ul) const
			throw()
		{
			ul = (unsigned long)(value_ * 255.0);
		}

		void ColorUnit::set(const float f)
			throw(Exception::InvalidRange)
		{
			if ((f < 0.0) || (f > 1.0))
      {
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}

			value_ = f;
		}

		const ColorUnit& ColorUnit::operator = (const float f)
			throw(Exception::InvalidRange)
		{
			set(f);

			return *this;
		}

		void ColorUnit::get(float& f) const
			throw()
		{
			f = (float)(value_ * 255.0);
		}

		void ColorUnit::set(const double d)
			throw(Exception::InvalidRange)
		{
			if ((d < 0.0) || (d > 1.0))
      {
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}

			value_ = (float)d;
		}

		const ColorUnit& ColorUnit::operator = (const double d)
			throw(Exception::InvalidRange)
		{
			set(d);

			return *this;
		}

		void ColorUnit::get(double& d) const
			throw()
		{
			d = (double)(value_ * 255.0);
		}

		void ColorUnit::set(const long double ld)
			throw(Exception::InvalidRange)
		{
			if ((ld < 0.0) || (ld > 1.0))
      {
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}

			value_ = (float)ld;
		}

		const ColorUnit& ColorUnit::operator = (const long double ld)
			throw(Exception::InvalidRange)
		{
			set(ld);

			return *this;
		}

		void ColorUnit::get(long double& ld) const
			throw()
		{
			ld = (long double)(value_ * 255.0);
		}
				
		void ColorUnit::swap(ColorUnit& color)
			throw()
		{
			float temp = value_;
			value_ = color.value_;
			color.value_ = temp;
		}

		void ColorUnit::dump
			(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "unit : " << (int)(value_ * 255.0) << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void ColorUnit::read(istream & /* s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void ColorUnit::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		istream& operator >> (istream& s, ColorUnit& color)
		{
			s >> color.value_;

			return s;
		}

		ostream& operator << (ostream& s, const ColorUnit& color)
		{
			s << (int)(color.value_ * 255.0);
			
			return s;
		}

		float ColorUnit::hexToFloat_(const char *char_ptr)
			throw(Exception::InvalidRange, NotInHexFormat)
		{
			int number = 0;

			if (strlen(char_ptr) != 2)
			{
				throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
			}

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
				throw NotInHexFormat(__FILE__, __LINE__, char_ptr);
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
				throw NotInHexFormat(__FILE__, __LINE__, char_ptr);
			}
			
			return (float)number / (float)255;
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorUnit.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
