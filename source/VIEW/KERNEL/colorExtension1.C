// $Id: colorExtension1.C,v 1.6 2001/05/13 14:28:37 hekl Exp $

#include <BALL/VIEW/KERNEL/colorExtension1.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		ColorExtension::ColorExtension()
			throw()
			:	color_()
		{
		}

		ColorExtension::ColorExtension
			(const ColorExtension& color_extension)
			throw()
			:	color_(color_extension.color_)
		{
		}

		ColorExtension::~ColorExtension()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " 
					<< RTTI::getName<ColorExtension>() << endl;
			#endif 

			destroy();
		}

		void ColorExtension::clear()
			throw()
		{
			color_.clear();
		}

		void ColorExtension::destroy()
			throw()
		{
		}

		void ColorExtension::set
			(const ColorExtension& color_extension)
			throw()
		{
			color_.set(color_extension.color_);
		}

		const ColorExtension& ColorExtension::operator =
			(const ColorExtension& color_extension)
			throw()
		{
			set(color_extension);

			return *this;
		}

		void ColorExtension::get
			(ColorExtension& color_extension) const
			throw()
		{
			color_extension.set(*this);
		}

		void ColorExtension::swap(ColorExtension& color_extension)
			throw()
		{
			color_.swap(color_extension.color_);
		}

		void ColorExtension::dump
			(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "color : " << color_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void ColorExtension::read(istream & /* s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void ColorExtension::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/colorExtension1.iC>
#		endif 
		
	} // namespace VIEW

} // namespace BALL
