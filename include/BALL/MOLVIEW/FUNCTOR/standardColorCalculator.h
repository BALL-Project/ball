// $Id: standardColorCalculator.h,v 1.7 2000/10/05 22:32:15 oliver Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_STANDARDCOLORCALCULATOR_H
#define BALL_MOLVIEW_FUNCTOR_STANDARDCOLORCALCULATOR_H

#ifndef BALL_MOLVIEW_FUNCTOR_COLORCALCULATOR_H
#	include <BALL/MOLVIEW/FUNCTOR/colorCalculator.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
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
			//@{

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

			virtual void visit(Composite& composite);
			//@}

			protected:

			ColorRGBA		positive_color_;
			ColorRGBA		neutral_color_;
			ColorRGBA		negative_color_;
		};



		/**
		*/
		class AtomDistanceColorCalculator
			:	public UnaryProcessor<Atom>,
			  public UnaryProcessor<Composite>,
			  public ColorCalculator
		{
			public:
			
			/**	@name	Constructors and Destructors
			*/
			//@{

      AtomDistanceColorCalculator();

 			AtomDistanceColorCalculator(const AtomDistanceColorCalculator& color_calculator);

			virtual void clear();

			virtual void destroy();

			//@}

			/**	@name Accessors
			*/
			//@{
			void setDistance(float distance);
			float getDistance() const;

			void setNullDistanceColor(const ColorRGBA& color);
			const ColorRGBA& getNullDistanceColor() const;

			void setFullDistanceColor(const ColorRGBA& color);
			const ColorRGBA& getFullDistanceColor() const;

			void calculateDistances();

			virtual bool start();
			
			virtual bool finish();

			virtual Processor::Result operator() (Atom& atom);

			virtual Processor::Result operator() (Composite& composite);

			virtual void visit(Atom& atom);

			virtual void visit(Composite& composite);
			//@}

			protected:

			typedef HashMap<void*, float> AtomDistanceHashMap;

			AtomDistanceHashMap atom_2_distance_;

			float distance_;

			ColorRGBA		null_distance_color_;
			ColorRGBA		full_distance_color_;
		};



#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/standardColorCalculator.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_STANDARDCOLORCALCULATOR_H
