// $Id: colorExtension4.C,v 1.4 1999/12/30 18:05:46 oliver Exp $

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
			(const ColorExtension4& color_extension, bool deep)
			:	ColorExtension3(color_extension, deep),
				color4_(color_extension.color4_, deep)
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
			clear();
		}

		void ColorExtension4::set
			(const ColorExtension4& color_extension, bool deep)
		{
			ColorExtension3::set(color_extension, deep);

			color4_.set(color_extension.color4_, deep);
		}

		ColorExtension4& ColorExtension4::operator =
			(const ColorExtension4& color_extension)
		{
			set(color_extension);

			return *this;
		}

		void ColorExtension4::get
			(ColorExtension4& color_extension, bool deep) const
		{
			color_extension.set(*this, deep);
		}

		void ColorExtension4::swap(ColorExtension4& color_extension)
		{
			ColorExtension3::swap(color_extension);

			color4_.swap(color_extension.color4_);
		}

		bool ColorExtension4::isValid() const
		{
			return (bool)(ColorExtension3::isValid()
				&& color4_.isValid());
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
