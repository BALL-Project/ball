// $Id: color.C,v 1.2 1999/12/19 17:15:34 oliver Exp $

#include <BALL/VIEW/DATATYPE/color.h>
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

		ColorRGBA::ColorRGBA(const ColorRGBA& color, bool /* deep */ )
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
					 << RTTI<ColorRGBA>::getName() << endl;
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
			clear();
		}

		ColorRGBA::operator String() const
		{
			char temp[10];

			sprintf(&temp[0], "%x%x%x%x",
				(unsigned char)red_, (unsigned char)green_,
				(unsigned char)blue_, (unsigned char)alpha_);

			return String(&temp[0]);  
		}

		void ColorRGBA::set(const ColorRGBA& color, bool /* deep */)
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

		void ColorRGBA::get(ColorRGBA& color, bool deep) const
		{
		 color.set(*this, deep);
		}

		void ColorRGBA::set(const ColorHSV& color_HSV, bool /* deep */)
		{
			color_HSV.calculateRGB_(red_, green_, blue_);

			alpha_ = (float)1;
		}

		ColorRGBA& ColorRGBA::operator = (const ColorHSV& color_HSV)
		{
			set(color_HSV);

			return *this;
		}

		void ColorRGBA::get(ColorHSV& color_HSV, bool deep) const
		{
			color_HSV.set(*this, deep);
		}

		void ColorRGBA::set(const char* char_ptr, bool /* deep */)
		{
			stringToRGBA_(String(char_ptr));
		}

		ColorRGBA& ColorRGBA::operator = (const char* char_ptr)
		{
			set(char_ptr);

			return *this;
		}

		void ColorRGBA::get(char *char_ptr, bool /* deep */) const
		{
			sprintf(char_ptr, "%x%x%x%x",
				(unsigned char)red_, (unsigned char)green_,
				(unsigned char)blue_, (unsigned char)alpha_);
		}

		void ColorRGBA::set
			(const String& s, bool /* deep */)
		{
			stringToRGBA_(s);
		}

		ColorRGBA& ColorRGBA::operator = (const String& s)
		{
			set(s);

			return *this;
		}

		void ColorRGBA::get(String& s, bool /* deep */) const
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

		void ColorRGBA::setHSV
			(const ColorUnitHue& hue,
			 const ColorUnit& saturation,
			 const ColorUnit& value)
		{
			ColorHSV color_HSV(hue, saturation, value);

			set(color_HSV);
		}

		void ColorRGBA::getHSV
			(ColorUnitHue& hue,
			 ColorUnit& saturation,
			 ColorUnit& value)
		{
			ColorHSV color_HSV;

			get(color_HSV);

			color_HSV.get(hue, saturation, value);
		}

		void ColorRGBA::dump
			(ostream& s, unsigned long depth) const
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
			s >>color.red_ >>color.green_ >>color.blue_ >>color.alpha_;
			
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



		ColorHSV::ColorHSV()
			:	hue_(),
				saturation_(),
				value_()
		{
		}

		ColorHSV::ColorHSV(const ColorHSV& color_HSV,bool /* deep */)
			:	hue_(color_HSV.hue_),
				saturation_(color_HSV.saturation_),
				value_(color_HSV.value_)
		{
		}

		ColorHSV::ColorHSV(const ColorRGBA& color)
		{
			calculateHSV_(color.red_, color.green_, color.blue_);
		}

		ColorHSV::ColorHSV(const char* char_ptr)
		{
			stringToHSV_(String(char_ptr));
		}

		ColorHSV::ColorHSV(const String& s)
		{
			stringToHSV_(s);
		}

		ColorHSV::ColorHSV
			(const ColorUnitHue& hue,
			 const ColorUnit& saturation,
			 const ColorUnit& value)
			:	hue_(hue),
				saturation_(saturation),
				value_(value)
		{
		}

		ColorHSV::~ColorHSV()
		{
			#ifdef BALL_VIEW_DEBUG
			cout << "Destructing object " << (void *)this << " of class " 
					 << RTTI<ColorHSV>::getName() << endl;
			#endif 

			destroy();
		}

		void ColorHSV::clear()
		{
			hue_ = (float)0;
			saturation_ = (float)0;
			value_ = (float)0;
		}

		void ColorHSV::destroy()
		{
			clear();
		}

		ColorHSV::operator String() const
		{
			char temp[10];

			sprintf(&temp[0], "%x%x%x",
				(short)hue_,
				(unsigned char)saturation_,
				(unsigned char)value_);

			return String(&temp[0]);  
		}

		void ColorHSV::set
			(const ColorHSV& color_HSV, bool /* deep */)
		{
			hue_ = color_HSV.hue_;
			saturation_ = color_HSV.saturation_;
			value_ = color_HSV.value_;
		}

		ColorHSV& ColorHSV::operator = (const ColorHSV& color_HSV)
		{
			set(color_HSV);

			return *this;
		}

		void ColorHSV::get(ColorHSV& color_HSV, bool deep) const
		{
			color_HSV.set(*this, deep);
		}

		void ColorHSV::set(const ColorRGBA& color, bool /* deep */)
		{
			calculateHSV_(color.red_, color.green_, color.blue_);
		}

		ColorHSV& ColorHSV::operator = (const ColorRGBA& color)
		{
			set(color);

			return *this;
		}

		void ColorHSV::get(ColorRGBA& color, bool deep) const
		{
		 color.set(*this, deep);
		}

		void ColorHSV::set(const char* char_ptr, bool /* deep */)
		{
			stringToHSV_(String(char_ptr));
		}

		ColorHSV& ColorHSV::operator = (const char* char_ptr)
		{
			set(char_ptr);

			return *this;
		}

		void ColorHSV::get(char* char_ptr, bool /* deep */) const
		{
			sprintf(char_ptr, "%x%x%x",
							(short)hue_, (unsigned char)saturation_,
							(unsigned char)value_);
		}

		void ColorHSV::set(const String& s, bool /* deep */)
		{
			stringToHSV_(s);
		}

		ColorHSV& ColorHSV::operator = (const String& s)
		{
			set(s);

			return *this;
		}

		void ColorHSV::get(String& s, bool /* deep */) const
		{
			char temp[10];

			sprintf(&temp[0], "%x%x%x",
							(short)hue_, (unsigned char)saturation_,
							(unsigned char)value_);

			s.set(&temp[0]);  
		}

		void ColorHSV::swap(ColorHSV& color_HSV)
		{
			hue_.swap(color_HSV.hue_);
			saturation_.swap(color_HSV.saturation_);
			value_.swap(color_HSV.value_);
		}

		void ColorHSV::dump
			(ostream& s, unsigned long depth) const
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
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void ColorHSV::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		istream& operator >> (istream& s, ColorHSV& color_HSV)
		{
			s >> color_HSV.hue_
					 >> color_HSV.saturation_
					 >> color_HSV.value_;
			
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
#			include <BALL/VIEW/DATATYPE/color.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
