// $Id: colorTable.h,v 1.1 1999/08/26 07:53:14 oliver Exp $

#ifndef BALL_VIEW_DATATYPE_COLORTABLE_H
#define BALL_VIEW_DATATYPE_COLORTABLE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#include <iostream.h>
#include <vector>

namespace BALL 
{

	namespace VIEW 
	{

		/**
		*/
		class ColorTable
			: public vector<ColorRGBA>
		{
			public:

			/**	@name	Constructors and Destructors 
			*/	
			//@{

			/**	Default constructor.
					Create an empty instance of ColorTable.
			*/
			ColorTable();

			/**	Copy constructor.
					Create a copy of a ColorTable object.
					@param	color_table the color table to be copied
					@param	deep ignored
			*/
			ColorTable(const ColorTable& color_table, bool deep = true);

			/**
			*/
			ColorTable(Size size, const ColorRGBA& color);

			/**
			*/
			ColorTable(const ColorTable& color_table, Index from, Index to);

			/**
			*/
			ColorTable(const ColorRGBA* color_array, Size array_size);

			/**	Destructor.
			*/
			virtual ~ColorTable();
			//@}

			/**	@name	Debugging and Diagnostics
			*/	
			//@{

			/**	
			*/
			virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(istream& s);

			virtual void write(ostream& s) const;
			//@}

		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorTable.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_DATATYPE_COLORTABLE_H
