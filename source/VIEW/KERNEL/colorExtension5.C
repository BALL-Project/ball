// $Id: colorExtension5.C,v 1.7 2001/05/13 14:28:38 hekl Exp $
#include <BALL/VIEW/KERNEL/colorExtension5.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		ColorExtension5::ColorExtension5()
			throw()
			:	ColorExtension4(),
				color5_()
		{
		}

		ColorExtension5::ColorExtension5
			(const ColorExtension5& color_extension)
			throw()
				:
				ColorExtension4(color_extension),
				color5_(color_extension.color5_)
		{
		}

		ColorExtension5::~ColorExtension5()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " 
					<< RTTI::getName<ColorExtension5>() << endl;
			#endif 

			destroy();
		}

		void ColorExtension5::clear()
			throw()
		{
			ColorExtension4::clear();
			color5_.clear();
		}

		void ColorExtension5::destroy()
			throw()
		{
		}

		void ColorExtension5::set
			(const ColorExtension5& color_extension)
			throw()
		{
			ColorExtension4::set(color_extension);

			color5_.set(color_extension.color5_);
		}

		const ColorExtension5& ColorExtension5::operator = (const ColorExtension5& color_extension)
			throw()
		{
			set(color_extension);

			return *this;
		}

		void ColorExtension5::get
			(ColorExtension5& color_extension) const
			throw()
		{
			color_extension.set(*this);
		}

		void ColorExtension5::swap(ColorExtension5& color_extension)
			throw()
		{
			ColorExtension4::swap(color_extension);

			color5_.swap(color_extension.color5_);
		}

		void ColorExtension5::dump
			(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			ColorExtension4::dump(s, depth);

			BALL_DUMP_DEPTH(s, depth);
			s << "color5 : " << color5_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void ColorExtension5::read(istream & /* s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void ColorExtension5::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/colorExtension5.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
