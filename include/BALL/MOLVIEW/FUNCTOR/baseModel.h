// $Id: baseModel.h,v 1.3 1999/12/28 18:38:06 oliver Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_BASEMODEL_H
#define BALL_MOLVIEW_FUNCTOR_BASEMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_COLORCALCULATOR_H
#	include <BALL/MOLVIEW/FUNCTOR/colorCalculator.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_FINDGEOMETRICOBJECT_H
#	include <BALL/MOLVIEW/FUNCTOR/findGeometricObject.h>
#endif

#ifndef BALL_MOLVIEW_KERNEL_EXTENDEDPROPERTY_H
#	include <BALL/MOLVIEW/KERNEL/extendedProperty.h>
#endif


namespace BALL
{

	namespace MOLVIEW
	{


		/**
		*/
		class BaseModelProcessor
			: public UnaryProcessor<Atom>,
				public ExtendedPropertyManager
		{
			public:

			/**	@name	 Constructors and Destructors
			*/
			//@{

			BaseModelProcessor();

			BaseModelProcessor
				(const BaseModelProcessor& base_model_processor, bool deep = true);

			virtual ~BaseModelProcessor();

			virtual void clear();

			virtual void destroy();
			//@}

			
			/**	@name	Assignment
			*/
			//@{

			void set
				(const BaseModelProcessor& base_model_processor,bool deep = true);

			BaseModelProcessor& operator =
				(const BaseModelProcessor& base_model_processor);

			void get
				(BaseModelProcessor& base_model_processor, bool deep = true) const;

			void swap
				(BaseModelProcessor& base_model_processor);
			//@}

			/**	@name	Accessors
			*/
			//@{

			void enableStaticModel();

			void disableStaticModel();

			void enableDynamicModel();

			void disableDynamicModel();

			void setColorCalculator
				(const ColorCalculator& color_calculator);

			void resetColorCalculator();

			ColorCalculator* getColorCalculator() const;
			//@}

				
			/**	@name	Processor Related Methods
			*/
			//@{
			virtual bool start();
			
			virtual bool finish();

			virtual Processor::Result operator() (Atom& atom);

			//@}
				
			/**	@name	Predicates
			*/
			//@{

			bool isStaticModelEnabled();

			bool isStaticModelDisabled();

			bool isDynamicModelEnabled();

			bool isDynamicModelDisabled();

			bool isDefaultColorCalculator();

			bool isOtherColorCalculator();
			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{
			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name Storers
			*/
			//@{

			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;
			//@}


			
			protected:

			FindGeometricObject& getSearcher();

			
			private:

			void dump_
				(std::ostream& s, Size depth) const;

			void clear_();

			/* search processor */ 
			FindGeometricObject find_geometric_object_;

			/* pointer to userdefinable table of colors */
			ColorCalculator* color_calculator_;
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/baseModel.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_BASEMODEL_H 
