// $Id: colorExtension4.C,v 1.6 2001/02/04 16:14:27 hekl Exp $

#include <BALL/VIEW/KERNEL/colorExtension4.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		ColorExtension4::ColorExtension4()
			:	ColorExtension3(),
				color4_()
		{
		}

		ColorExtension4::ColorExtension4
			(const ColorExtension4& color_extension)
			:	ColorExtension3(color_extension),
				color4_(color_extension.color4_)
		{
		}

		ColorExtension4::~ColorExtension4()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " 
					<< RTTI::getName<ColorExtension4>() << endl;
			#endif 

			destroy();
		}

		void ColorExtension4::clear()
		{
			ColorExtension3::clear();
			color4_.clear();
		}

		void ColorExtension4::destroy()
		{
		}

		void ColorExtension4::set
			(const ColorExtension4& color_extension)
		{
			ColorExtension3::set(color_extension);

			color4_.set(color_extension.color4_);
		}

		ColorExtension4& ColorExtension4::operator = (const ColorExtension4& color_extension)
		{
			set(color_extension);

			return *this;
		}

		void ColorExtension4::get
			(ColorExtension4& color_extension) const
		{
			color_extension.set(*this);
		}

		void ColorExtension4::swap(ColorExtension4& color_extension)
		{
			ColorExtension3::swap(color_extension);

			color4_.swap(color_extension.color4_);
		}

		void ColorExtension4::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			ColorExtension3::dump(s, depth);

			BALL_DUMP_DEPTH(s, depth);
			s << "color4 : " << color4_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void ColorExtension4::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void ColorExtension4::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/colorExtension4.iC>
#		endif 
	
	} // namespace VIEW

} // namespace BALL
