// $Id: colorCalculator.h,v 1.6 2000/12/12 16:16:52 oliver Exp $

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

#ifndef BALL_KERNEL_PTE_H
#	include <BALL/KERNEL/PTE.h>
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
			
			/**	@name	Type definitions
			*/
			//@{
				
			/**	ColorMap type
			*/
			typedef StringHashMap<ColorRGBA>	ColorMap;
			//@}

			/**	@name	Constructors and Destructors
			*/
			//@{

			ColorCalculator();

			ColorCalculator
				(const ColorCalculator& color_calculator, bool deep = true);

			virtual ~ColorCalculator()
				throw();

			virtual void clear()
				throw();

			virtual void destroy()
				throw();
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

			void setDefaultColor(const ColorRGBA& color);

			const ColorRGBA& getDefaultColor() const;

			const ColorRGBA& getColor() const;

			void setColorMap(const ColorMap& color_map);

			const ColorMap& getColorMap() const;				

			virtual void visit(Atom& atom);

			virtual void visit(Composite& composite);

			virtual const String& calculateKey(const Atom& atom) const;
			//@}
			

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			protected:
			
			ColorMap		color_map_;
			ColorRGBA		default_color_;
			ColorRGBA		color_;
		};

			
#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/colorCalculator.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_COLORCALCULATOR_H
