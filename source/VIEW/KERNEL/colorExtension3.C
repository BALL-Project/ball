// $Id: colorExtension3.C,v 1.7 2001/05/13 14:28:37 hekl Exp $

#include <BALL/VIEW/KERNEL/colorExtension3.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{


		ColorExtension3::ColorExtension3()
			throw()
			:	ColorExtension2(),
				color3_()
		{
		}

		ColorExtension3::ColorExtension3
			(const ColorExtension3& color_extension)
			throw()
			:	ColorExtension2(color_extension),
				color3_(color_extension.color3_)
		{
		}

		ColorExtension3::~ColorExtension3()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " 
					<< RTTI::getName<ColorExtension3>() << endl;
			#endif 

			destroy();
		}

		void ColorExtension3::clear()
			throw()
		{
			ColorExtension2::clear();
			color3_.clear();
		}

		void ColorExtension3::destroy()
			throw()
		{
		}

		void ColorExtension3::set
			(const ColorExtension3& color_extension)
			throw()
		{
			ColorExtension2::set(color_extension);

			color3_.set(color_extension.color3_);
		}

		const ColorExtension3& ColorExtension3::operator = (const ColorExtension3& color_extension)
			throw()
		{
			set(color_extension);

			return *this;
		}

		void ColorExtension3::get
			(ColorExtension3& color_extension) const
			throw()
		{
			color_extension.set(*this);
		}

		void ColorExtension3::swap(ColorExtension3& color_extension)
			throw()
		{
			ColorExtension2::swap(color_extension);

			color3_.swap(color_extension.color3_);
		}

		void ColorExtension3::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			ColorExtension2::dump(s, depth);

			BALL_DUMP_DEPTH(s, depth);
			s << "color3 : " << color3_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void ColorExtension3::read(istream & /* s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void ColorExtension3::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/colorExtension3.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
