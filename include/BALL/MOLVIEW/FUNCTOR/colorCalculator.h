// $Id: colorCalculator.h,v 1.2 1999/12/19 17:18:49 oliver Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_COLORCALCULATOR_H
#define BALL_MOLVIEW_FUNCTOR_COLORCALCULATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_VISITOR_H
#	include <BALL/CONCEPT/visitor.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_PSE_H
#	include <BALL/KERNEL/PSE.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORTABLE_H
#	include <BALL/VIEW/DATATYPE/colorTable.h>
#endif

#ifndef BALL_MOLVIEW_COLORS_ELEMENTCOLOR_H
#	include <BALL/MOLVIEW/COLORS/elementColor.h>
#endif

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
#endif

  
namespace BALL
{

	namespace MOLVIEW
	{

		/**
		*/
		class ColorCalculator
			: public Visitor<Atom>,
				public Visitor<Composite>
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			ColorCalculator();

			ColorCalculator
				(const ColorCalculator& color_calculator, bool deep = true);

			virtual ~ColorCalculator();

			virtual void clear();

			virtual void destroy();
			//@}

			/**	@name	Asignment
			*/
			//@{

			void set
				(const ColorCalculator& color_calculator, bool deep = true);

			ColorCalculator&  operator =
				(const ColorCalculator& color_calculator);

			void get
				(ColorCalculator&  color_calculator, bool deep = true) const;

			void swap
				(ColorCalculator&  color_calculator);
			//@}

			
			/**	@name	Accessors
			*/
			//@{

			void setColorTable(const ColorTable& color_table);

			void resetColorTable();

			const ColorTable* getColorTable() const;

			const ColorRGBA& getColor() const;

			virtual void visit(Atom& atom);

			virtual void visit(Composite& composite);
			//@}
			

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(ostream& s = cout, unsigned long depth = 0) const;
			//@}


				
			private:  

			ColorRGBA color_;

			ColorTable* color_table_;
		};

			
#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/colorCalculator.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_COLORCALCULATOR_H
