// $Id: colorRGBA.C,v 1.1 2001/02/04 16:14:25 hekl Exp $

#include <BALL/VIEW/DATATYPE/colorRGBA.h>
#include <BALL/VIEW/DATATYPE/colorHSV.h>
using namespace std;

namespace BALL
{

	namespace VIEW
	{

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
		 
		ColorRGBA::ColorRGBA
			(const ColorUnit& red, const ColorUnit& green,
			 const ColorUnit& blue, const ColorUnit& alpha)
			:	red_(red),
				green_(green),
				blue_(blue),
				alpha_(alpha)
		{
		}
		 
		ColorRGBA::~ColorRGBA()
		{
			#ifdef BALL_VIEW_DEBUG
			cout << "Destructing object " << (void *)this << " of class " 
					 << RTTI::getName<ColorRGBA>() << endl;
			#endif 

			destroy();
		}

		void ColorRGBA::clear()
		{
			red_ = green_ = blue_ = (float)0;
			alpha_ = ((float)1);
		}

		void ColorRGBA::destroy()
		{
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

		ColorRGBA& ColorRGBA::operator = (const ColorRGBA& color)
		{
			set(color);

			return *this;
		}

		void ColorRGBA::get(ColorRGBA& color) const
		{
		 color.set(*this);
		}

		void ColorRGBA::set(const ColorHSV& color_HSV)
		{
			color_HSV.calculateRGB_(red_, green_, blue_);

			alpha_ = (float)1;
		}

		ColorRGBA& ColorRGBA::operator = (const ColorHSV& color_HSV)
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

		ColorRGBA& ColorRGBA::operator = (const char* char_ptr)
		{
			set(char_ptr);

			return *this;
		}

		void ColorRGBA::get(char *char_ptr) const
		{
			sprintf(char_ptr, "%x%x%x%x",
				(unsigned char)red_, (unsigned char)green_,
				(unsigned char)blue_, (unsigned char)alpha_);
		}

		void ColorRGBA::set
			(const String& s)
		{
			stringToRGBA_(s);
		}

		ColorRGBA& ColorRGBA::operator = (const String& s)
		{
			set(s);

			return *this;
		}

		void ColorRGBA::get(String& s) const
		{
			char temp[10];

			sprintf(&temp[0], "%x%x%x%x",
				(unsigned char)red_, (unsigned char)green_,
				(unsigned char)blue_, (unsigned char)alpha_);

			s.set(&temp[0]);
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

		void ColorRGBA::read(istream& /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void ColorRGBA::write(ostream&  /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		istream& operator >> (istream& s, ColorRGBA& color)
		{
			char dummy[20];
			s >> dummy >> color.red_ 
				>> dummy >> color.green_ 
				>> dummy >> color.blue_ 
   			>> dummy >> color.alpha_
				>> dummy;
			
			return s;
		}

		ostream& operator << (ostream& s, const ColorRGBA& color)
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


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorRGBA.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
