// $Id: elementColor.h,v 1.4 2000/03/28 19:23:30 oliver Exp $

#ifndef BALL_VIEW_MOLVIEW_COLORS_ELEMENTCOLOR_H
#define BALL_VIEW_MOLVIEW_COLORS_ELEMENTCOLOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORTABLE_H
#	include <BALL/VIEW/DATATYPE/colorTable.h>
#endif

#include <iostream>

namespace BALL
{

	namespace MOLVIEW
	{

		using VIEW::ColorTable;
		using VIEW::ColorRGBA;
		
		/**
		*/
		class ElementColor	
			: public VIEW::ColorTable
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			ElementColor();

			ElementColor
				(const ElementColor& element_color, bool deep = true);

			ElementColor
				(Size size, const ColorRGBA& color);

			ElementColor
				(const ElementColor& element_color, Index from, Index to);

			ElementColor
				(const ColorRGBA* array, Size array_size);

			virtual ~ElementColor();
			//@}



			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name Storers
			*/
			//@{

			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;
			//@}


				
			private:

			void initializeColors_();

			/* element color array */ 
			// BAUSTELLE: Konstante fuer Anzahl Elemente
			// aus PTE uebernehmen
			static const unsigned char color_values_[111][3];
		};


#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/COLORS/elementColor.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL
			

#endif // BALL_MOLVIEW_COLORS_ELEMENTCOLOR_H
