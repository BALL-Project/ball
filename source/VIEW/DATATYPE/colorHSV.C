// $Id: colorHSV.C,v 1.2 2001/05/13 14:28:34 hekl Exp $

#include <BALL/VIEW/DATATYPE/colorHSV.h>
#include <BALL/VIEW/DATATYPE/colorRGBA.h>
using namespace std;

namespace BALL
{

	namespace VIEW
	{

		ColorHSV::ColorHSV()
			throw()
			:	hue_(),
				saturation_(),
				value_()
		{
		}

		ColorHSV::ColorHSV(const ColorHSV& color_HSV)
			throw()
			:	hue_(color_HSV.hue_),
				saturation_(color_HSV.saturation_),
				value_(color_HSV.value_)
		{
		}

		ColorHSV::ColorHSV(const ColorRGBA& color)
			throw()
		{
			calculateHSV_(color.red_, color.green_, color.blue_);
		}

		ColorHSV::ColorHSV(const char* char_ptr)
			throw(Exception::InvalidRange, ColorUnit::NotInHexFormat)
		{
			stringToHSV_(String(char_ptr));
		}

		ColorHSV::ColorHSV(const String& s)
			throw(Exception::InvalidRange, ColorUnit::NotInHexFormat)
		{
			stringToHSV_(s);
		}

		ColorHSV::ColorHSV
			(const ColorUnitHue& hue,
			 const ColorUnit& saturation,
			 const ColorUnit& value)
			throw()
			:	hue_(hue),
				saturation_(saturation),
				value_(value)
		{
		}

		ColorHSV::~ColorHSV()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
			cout << "Destructing object " << (void *)this << " of class " 
					 << RTTI::getName<ColorHSV>() << endl;
			#endif 

			destroy();
		}

		void ColorHSV::clear()
			throw()
		{
			hue_ = (float)0;
			saturation_ = (float)0;
			value_ = (float)0;
		}

		void ColorHSV::destroy()
			throw()
		{
			clear();
		}

		ColorHSV::operator String() const
			throw()
		{
			char temp[10];

			sprintf(&temp[0], "%x%x%x",
				(short)hue_,
				(unsigned char)saturation_,
				(unsigned char)value_);

			return String(&temp[0]);  
		}

		void ColorHSV::set
			(const ColorHSV& color_HSV)
			throw()
		{
			hue_ = color_HSV.hue_;
			saturation_ = color_HSV.saturation_;
			value_ = color_HSV.value_;
		}

		const ColorHSV& ColorHSV::operator = (const ColorHSV& color_HSV)
			throw()
		{
			set(color_HSV);

			return *this;
		}

		void ColorHSV::get(ColorHSV& color_HSV) const
			throw()
		{
			color_HSV.set(*this);
		}

		void ColorHSV::set(const ColorRGBA& color)
			throw()
		{
			calculateHSV_(color.red_, color.green_, color.blue_);
		}

		const ColorHSV& ColorHSV::operator = (const ColorRGBA& color)
			throw()
		{
			set(color);

			return *this;
		}

		void ColorHSV::get(ColorRGBA& color) const
			throw()
		{
		 color.set(*this);
		}

		void ColorHSV::set(const char* char_ptr)
			throw(Exception::InvalidRange, ColorUnit::NotInHexFormat)
		{
			stringToHSV_(String(char_ptr));
		}

		const ColorHSV& ColorHSV::operator = (const char* char_ptr)
			throw(Exception::InvalidRange, ColorUnit::NotInHexFormat)
		{
			set(char_ptr);

			return *this;
		}

		void ColorHSV::get(char* char_ptr) const
			throw()
		{
			sprintf(char_ptr, "%x%x%x",
							(short)hue_, (unsigned char)saturation_,
							(unsigned char)value_);
		}

		void ColorHSV::set(const String& s)
			throw(Exception::InvalidRange, ColorUnit::NotInHexFormat)
		{
			stringToHSV_(s);
		}

		const ColorHSV& ColorHSV::operator = (const String& s)
			throw(Exception::InvalidRange, ColorUnit::NotInHexFormat)
		{
			set(s);

			return *this;
		}

		void ColorHSV::get(String& s) const
			throw()
		{
			char temp[10];

			sprintf(&temp[0], "%x%x%x",
							(short)hue_, (unsigned char)saturation_,
							(unsigned char)value_);

			s.set(&temp[0]);  
		}

		void ColorHSV::swap(ColorHSV& color_HSV)
			throw()
		{
			hue_.swap(color_HSV.hue_);
			saturation_.swap(color_HSV.saturation_);
			value_.swap(color_HSV.value_);
		}

		void ColorHSV::dump
			(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "color HSV: " << endl;

			hue_.dump(s, depth + 1);
			saturation_.dump(s, depth + 1);
			value_.dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);  
		}

		void ColorHSV::read(istream& /* s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void ColorHSV::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		istream& operator >> (istream& s, ColorHSV& color_HSV)
		{
			char dummy[20];

			s >> dummy >> color_HSV.hue_
				>> dummy >> color_HSV.saturation_
				>> dummy >> color_HSV.value_
				>> dummy;
			
			return s;
		}

		ostream& operator << (ostream& s, const ColorHSV& color_HSV)
		{
			s << "(" << color_HSV.hue_ << ","
				<< color_HSV.saturation_ << ","
				<< color_HSV.value_ << ")";
			
			return s;
		}

		void ColorHSV::calculateHSV_
			(const ColorUnit red,
			 const ColorUnit green,
			 const ColorUnit blue)
			throw()
		{
			int tmp_red = (int)red;
			int tmp_green = (int)green;
			int tmp_blue = (int)blue;

			unsigned int max_component = tmp_red;
			unsigned int min_component = tmp_red;
			int max_number = 0; // 0 = red, 1 = green, 2 = blue

			if ((unsigned int)tmp_green > max_component)
			{
				max_component = tmp_green;
				max_number    = 1;
			}
		 
			if ((unsigned int)tmp_blue > max_component)
			{
				max_component = tmp_blue;
				max_number    = 2;
			}
			
			if ((unsigned int)tmp_green < min_component)
			{
				min_component = tmp_green;
			}

			if ((unsigned int)tmp_blue < min_component)
			{
				min_component = tmp_blue;
			}

			int delta = max_component - min_component;

			value_ = max_component;

			saturation_ 
				= max_component ? (510 * delta + max_component) / (2 * max_component) : 0;


			switch (max_number)
			{
				case 0:
					if (tmp_green >= tmp_blue)
					{
						hue_ = (120 * (tmp_green - tmp_blue) + delta) / (2 * delta);
					}
					else
					{
						hue_ = (120 * (tmp_green - tmp_blue + delta) + delta) / (2 * delta) + 300;
					}
					break;

				case 1:
					if (tmp_blue > tmp_red)
					{
						hue_ = 120 + (120 * (tmp_blue - tmp_red) + delta) / (2 * delta);
					}
					else
					{
						hue_ = 60 + (120 * (tmp_blue - tmp_red + delta) + delta) / (2 * delta);
					}
					break;

				case 2:
					if (tmp_red > tmp_green)
					{
						hue_ = 240 + (120 * (tmp_red - tmp_green) + delta) / (2 * delta);
					}
					else
					{
						hue_ = 180 + (120 * (tmp_red - tmp_green + delta) + delta) / (2 * delta);
					}
					break;
			}
		}

		void ColorHSV::calculateRGB_
			(ColorUnit& red, ColorUnit& green, ColorUnit& blue) const
			throw()
		{
			red = green = blue = value_; 

			unsigned int hue = hue_;
			unsigned int saturation = saturation_;
			unsigned int value = value_;

			unsigned int mod_hue = hue % 60;
			unsigned int div_hue = hue / 60;

			unsigned int p = (unsigned int)(2 * value* (255 - saturation) + 255) / 510;

			if (div_hue & 1)
			{
				unsigned int q 
					= (unsigned int)(2 * value * (15300 - saturation * mod_hue) + 15300) / 30600;

				switch (div_hue)
				{
					case 1:
						red   = q;
						green = value;
						blue  = p;
						break;

					case 3:
						red   = p;
						green = q;
						blue  = value;
						break;

					case 5:
						red   = value;
						green = p;
						blue  = q;
						break;
				}
			}
			else
			{
				unsigned int q = (unsigned int)(2 * value 
					* (15300 - (saturation * (60 - mod_hue))) + 15300) / 30600;

				switch (div_hue)
				{
					case 0:
						red   = value;
						green = q;
						blue  = p;
						break;

					case 2:
						red   = p;
						green = value;
						blue  = q;
						break;

					case 4:
						red   = q;
						green = p;
						blue  = value;
						break;
				}
			}
		}

		void ColorHSV::stringToHSV_(const String& s)
			throw(Exception::InvalidRange, ColorUnit::NotInHexFormat)
		{
			int length = (int)s.size();

			#ifdef BALL_VIEW_DEBUG
					
				if ((length != 6) && (length != 7))
				{
					throw ::BALL::Exception::InvalidRange(__FILE__, __LINE__);
				}
								
			#endif

			char buffer[5];
			char *char_ptr = (char *)s.c_str();

			if (length == 7)
			{
				buffer[0] = char_ptr[0];
				buffer[1] = char_ptr[1];
				buffer[2] = char_ptr[2];
				buffer[3] = (char)0;

				hue_ = buffer;

				buffer[0] = char_ptr[3];
				buffer[1] = char_ptr[4];
				buffer[2] = (char)0;
				
				saturation_ = buffer;
				
				buffer[0] = char_ptr[5];
				buffer[1] = char_ptr[6];
				
				value_ = buffer;
			}
			else
			{
				buffer[0] = char_ptr[0];
				buffer[1] = char_ptr[1];
				buffer[2] = (char)0;

				hue_ = buffer;

				buffer[0] = char_ptr[2];
				buffer[1] = char_ptr[3];
				
				saturation_ = buffer;
				
				buffer[0] = char_ptr[4];
				buffer[1] = char_ptr[5];
				
				value_ = buffer;
			}
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorHSV.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
