// $Id: standardColorCalculator.h,v 1.4 2000/04/04 15:18:33 oliver Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_STANDARDCOLORCALCULATOR_H
#define BALL_MOLVIEW_FUNCTOR_STANDARDCOLORCALCULATOR_H

#ifndef BALL_MOLVIEW_FUNCTOR_COLORCALCULATOR_H
#	include <BALL/MOLVIEW/FUNCTOR/colorCalculator.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif


  
namespace BALL
{

	namespace MOLVIEW
	{
			
		/**
		*/
		typedef ColorCalculator CustomColorCalculator;
		
		/**
		*/
		class ElementColorCalculator
			: public ColorCalculator
		{
			public:
			
			/**	@name	Constructors and Destructors
			*/
			//@{

			ElementColorCalculator();

			//@}
		};

		/**
		*/
		class ResidueNameColorCalculator
			: public ColorCalculator
		{
			public:
			
			/**	@name	Constructors and Destructors
			*/
			//@{

			ResidueNameColorCalculator();

			//@}

			/**	@name Accessors
			*/

			/**
			*/
			virtual const String& calculateKey(const Atom& atom) const;
			//@}
			
		};

			
		/**
		*/
		class AtomChargeColorCalculator
			: public ColorCalculator
		{
			public:
			
			/**	@name	Constructors and Destructors
			*/
			//@{

      AtomChargeColorCalculator();

 			AtomChargeColorCalculator(const AtomChargeColorCalculator& color_calculator);

			//@}

			/**	@name Accessors
			*/
			//@{
			void setPositiveColor(const ColorRGBA& color);
			const ColorRGBA& getPositiveColor() const;

			void setNeutralColor(const ColorRGBA& color);
			const ColorRGBA& getNeutralColor() const;

			void setNegativeColor(const ColorRGBA& color);
			const ColorRGBA& getNegativeColor() const;

			virtual void visit(Atom& atom);

			virtual void visit(Composite& composite) 
			{
			};
			//@}

			protected:
			ColorRGBA		positive_color_;
			ColorRGBA		neutral_color_;
			ColorRGBA		negative_color_;
		};

			
#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/standardColorCalculator.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_STANDARDCOLORCALCULATOR_H
