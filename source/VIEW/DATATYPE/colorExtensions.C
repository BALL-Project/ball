// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorExtensions.C,v 1.1.4.1 2007/03/25 21:56:33 oliver Exp $

#include <BALL/VIEW/DATATYPE/colorExtensions.h>
#include <BALL/COMMON/rtti.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		ColorExtension2::ColorExtension2()
			:	color2_()
		{
		}

		ColorExtension2::ColorExtension2(const ColorExtension2& color_extension)
			: color2_(color_extension.color2_)
		{
		}

		ColorExtension2::~ColorExtension2()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this << " of class " 
									 << RTTI::getName<ColorExtension2>() << std::endl;
			#endif 
		}

		void ColorExtension2::clear()
		{
			color2_.clear();
		}

		void ColorExtension2::set(const ColorExtension2& color_extension)
		{
			color2_.set(color_extension.color2_);
		}

		const ColorExtension2& ColorExtension2::operator = (const ColorExtension2& color_extension)
		{
			set(color_extension);
			return *this;
		}

		void ColorExtension2::swap(ColorExtension2& color_extension)
		{
			color2_.swap(color_extension.color2_);
		}

		void ColorExtension2::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "color2 : " << color2_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}



		MultiColorExtension::MultiColorExtension()
			:	colors()
		{
		}

		MultiColorExtension::MultiColorExtension(const MultiColorExtension& color_extension)
			: colors(color_extension.colors)
		{
		}

		MultiColorExtension::~MultiColorExtension()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this << " of class " 
									 << RTTI::getName<MultiColorExtension>() << std::endl;
			#endif 
		}

		void MultiColorExtension::clear()
		{
			colors.clear();
		}

		void MultiColorExtension::set(const MultiColorExtension& color_extension)
		{
			colors = color_extension.colors;
		}

		const MultiColorExtension& MultiColorExtension::operator = (const MultiColorExtension& color_extension)
		{
			set(color_extension);
			return *this;
		}

		void MultiColorExtension::swap(MultiColorExtension& color_extension)
		{
			colors.swap(color_extension.colors);
		}

		void MultiColorExtension::setAlphas(Size alpha)
		{
			for (Position p = 0; p < colors.size(); p++)
			{
				colors[p].setAlpha(alpha);
			}
		}

		void MultiColorExtension::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "colors : " << colors.size() << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorExtensions.iC>
#		endif 

	} // namespace VIEW
} // namespace BALL
