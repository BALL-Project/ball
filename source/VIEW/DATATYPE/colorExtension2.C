// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorExtension2.C,v 1.1 2003/08/26 13:14:47 oliver Exp $

#include <BALL/VIEW/DATATYPE/colorExtension2.h>
#include <BALL/COMMON/rtti.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		ColorExtension2::ColorExtension2()
			throw()
			:	color2_()
		{
		}

		ColorExtension2::ColorExtension2(const ColorExtension2& color_extension)
			throw()
			: color2_(color_extension.color2_)
		{
		}

		ColorExtension2::~ColorExtension2()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this << " of class " 
									 << RTTI::getName<ColorExtension2>() << std::endl;
			#endif 
		}

		void ColorExtension2::clear()
			throw()
		{
			color2_.clear();
		}

		void ColorExtension2::set(const ColorExtension2& color_extension)
			throw()
		{
			color2_.set(color_extension.color2_);
		}

		const ColorExtension2& ColorExtension2::operator = (const ColorExtension2& color_extension)
			throw()
		{
			set(color_extension);
			return *this;
		}

		void ColorExtension2::get(ColorExtension2& color_extension) const
			throw()
		{
			color_extension.set(*this);
		}

		void ColorExtension2::swap(ColorExtension2& color_extension)
			throw()
		{
			color2_.swap(color_extension.color2_);
		}

		void ColorExtension2::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "color2 : " << color2_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

	
#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorExtension2.iC>
#		endif 

	} // namespace VIEW
} // namespace BALL
