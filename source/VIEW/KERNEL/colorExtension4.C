// $Id: colorExtension4.C,v 1.7 2001/05/13 14:28:37 hekl Exp $

#include <BALL/VIEW/KERNEL/colorExtension4.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		ColorExtension4::ColorExtension4()
			throw()
			:	ColorExtension3(),
				color4_()
		{
		}

		ColorExtension4::ColorExtension4
			(const ColorExtension4& color_extension)
			throw()
			:	ColorExtension3(color_extension),
				color4_(color_extension.color4_)
		{
		}

		ColorExtension4::~ColorExtension4()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " 
					<< RTTI::getName<ColorExtension4>() << endl;
			#endif 

			destroy();
		}

		void ColorExtension4::clear()
			throw()
		{
			ColorExtension3::clear();
			color4_.clear();
		}

		void ColorExtension4::destroy()
			throw()
		{
		}

		void ColorExtension4::set
			(const ColorExtension4& color_extension)
			throw()
		{
			ColorExtension3::set(color_extension);

			color4_.set(color_extension.color4_);
		}

		const ColorExtension4& ColorExtension4::operator = (const ColorExtension4& color_extension)
			throw()
		{
			set(color_extension);

			return *this;
		}

		void ColorExtension4::get
			(ColorExtension4& color_extension) const
			throw()
		{
			color_extension.set(*this);
		}

		void ColorExtension4::swap(ColorExtension4& color_extension)
			throw()
		{
			ColorExtension3::swap(color_extension);

			color4_.swap(color_extension.color4_);
		}

		void ColorExtension4::dump
			(ostream& s, Size depth) const
			throw()
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
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void ColorExtension4::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/colorExtension4.iC>
#		endif 
	
	} // namespace VIEW

} // namespace BALL
