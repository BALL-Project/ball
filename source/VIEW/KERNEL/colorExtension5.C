// $Id: colorExtension5.C,v 1.4 1999/12/30 18:05:46 oliver Exp $
#include <BALL/VIEW/KERNEL/colorExtension5.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		ColorExtension5::ColorExtension5()
			:	ColorExtension4(),
				color5_()
		{
		}

		ColorExtension5::ColorExtension5
			(const ColorExtension5& color_extension, bool deep)
				:
				ColorExtension4(color_extension, deep),
				color5_(color_extension.color5_, deep)
		{
		}

		ColorExtension5::~ColorExtension5()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " 
					<< RTTI::getName<ColorExtension5>() << endl;
			#endif 

			destroy();
		}

		void ColorExtension5::clear()
		{
			ColorExtension4::clear();
			color5_.clear();
		}

		void ColorExtension5::destroy()
		{
			clear();
		}

		void ColorExtension5::set
			(const ColorExtension5& color_extension, bool deep)
		{
			ColorExtension4::set(color_extension, deep);

			color5_.set(color_extension.color5_, deep);
		}

		ColorExtension5& ColorExtension5::operator =
			(const ColorExtension5& color_extension)
		{
			set(color_extension);

			return *this;
		}

		void ColorExtension5::get
			(ColorExtension5& color_extension, bool deep) const
		{
			color_extension.set(*this, deep);
		}

		void ColorExtension5::swap(ColorExtension5& color_extension)
		{
			ColorExtension4::swap(color_extension);

			color5_.swap(color_extension.color5_);
		}

		bool ColorExtension5::isValid() const
		{
			return (bool)(ColorExtension4::isValid()
										&& color5_.isValid());
		}

		void ColorExtension5::dump
			(ostream& s, Size depth) const
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
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void ColorExtension5::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/colorExtension5.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
