// $Id: colorExtension1.C,v 1.5 2001/02/04 16:14:27 hekl Exp $

#include <BALL/VIEW/KERNEL/colorExtension1.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		ColorExtension::ColorExtension()
			:	color_()
		{
		}

		ColorExtension::ColorExtension
			(const ColorExtension& color_extension)
			:	color_(color_extension.color_)
		{
		}

		ColorExtension::~ColorExtension()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " 
					<< RTTI::getName<ColorExtension>() << endl;
			#endif 

			destroy();
		}

		void ColorExtension::clear()
		{
			color_.clear();
		}

		void ColorExtension::destroy()
		{
		}

		void ColorExtension::set
			(const ColorExtension& color_extension)
		{
			color_.set(color_extension.color_);
		}

		ColorExtension& ColorExtension::operator =
			(const ColorExtension& color_extension)
		{
			set(color_extension);

			return *this;
		}

		void ColorExtension::get
			(ColorExtension& color_extension) const
		{
			color_extension.set(*this);
		}

		void ColorExtension::swap(ColorExtension& color_extension)
		{
			color_.swap(color_extension.color_);
		}

		void ColorExtension::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "color : " << color_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void ColorExtension::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void ColorExtension::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/colorExtension1.iC>
#		endif 
		
	} // namespace VIEW

} // namespace BALL
