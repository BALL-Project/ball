// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorExtension1.C,v 1.9 2002/12/16 12:23:11 sturm Exp $

#include <BALL/VIEW/KERNEL/colorExtension1.h>
#include <BALL/COMMON/rtti.h>

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

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/colorExtension1.iC>
#		endif 
		
	} // namespace VIEW

} // namespace BALL
