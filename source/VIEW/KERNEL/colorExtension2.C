// $Id: colorExtension2.C,v 1.7 2001/05/13 14:28:37 hekl Exp $

#include <BALL/VIEW/KERNEL/colorExtension2.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		ColorExtension2::ColorExtension2()
			throw()
			:	color1_(),
				color2_()
		{
		}

		ColorExtension2::ColorExtension2
			(const ColorExtension2& color_extension)
			throw()
			:	color1_(color_extension.color1_),
				color2_(color_extension.color2_)
		{
		}

		ColorExtension2::~ColorExtension2()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " 
					<< RTTI::getName<ColorExtension2>() << endl;
			#endif 

			destroy();
		}

		void ColorExtension2::clear()
			throw()
		{
			color1_.clear();
			color2_.clear();
		}

		void ColorExtension2::destroy()
			throw()
		{
		}

		void ColorExtension2::set
			(const ColorExtension2& color_extension)
			throw()
		{
			color1_.set(color_extension.color1_);
				
			color2_.set(color_extension.color2_);
		}

		const ColorExtension2& ColorExtension2::operator = (const ColorExtension2& color_extension)
			throw()
		{
			set(color_extension);

			return *this;
		}

		void ColorExtension2::get
			(ColorExtension2& color_extension) const
			throw()
		{
			color_extension.set(*this);
		}

		void ColorExtension2::swap(ColorExtension2& color_extension)
			throw()
		{
			color1_.swap(color_extension.color1_);
			color2_.swap(color_extension.color2_);
		}

		void ColorExtension2::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "color1 : " << color1_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "color2 : " << color2_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void ColorExtension2::read(istream & /* s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void ColorExtension2::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/colorExtension2.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
