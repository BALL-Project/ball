// $Id: baseModelConnector.h,v 1.2 2000/12/12 16:16:52 oliver Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_BASEMODELCONNECTOR_H
#define BALL_MOLVIEW_FUNCTOR_BASEMODELCONNECTOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_VISITOR_H
#	include <BALL/CONCEPT/visitor.h>
#endif

#ifndef BALL_CONCEPT_PROPERTY_H
#	include <BALL/CONCEPT/property.h>
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

 
namespace BALL
{

	namespace MOLVIEW
	{
		
		/**
		*/
		class BaseModelConnector
			: public Visitor<Composite>,
			  public PropertyManager
		{
			public:
			
			/**	@name	Type definitions
			*/
			//@{
			//@}

			/**	@name	Constructors and Destructors
			*/
			//@{

			BaseModelConnector();

			BaseModelConnector
				(const BaseModelConnector& connector, bool deep = true);

			virtual ~BaseModelConnector()
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
				(const BaseModelConnector& connector, bool deep = true);

			BaseModelConnector&  operator =
				(const BaseModelConnector& connector);

			void get
				(BaseModelConnector& connector, bool deep = true) const;

			void swap
				(BaseModelConnector& connector);
			//@}

			
			/**	@name	Accessors
			*/
			//@{

			void registerColorCalculator
				(const ColorCalculator& color_calculator);

			void unregisterColorCalculator();

			ColorCalculator* getColorCalculator() const;

			void setProperties(PropertyManager &manager);

			FindGeometricObjects& getGeometricObjectSearcher();

			virtual void visit(Composite& composite);
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
			
  		private:

			FindGeometricObjects searcher_;

			ColorCalculator* color_calculator_;
		};

			
#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/baseModelConnector.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_BASEMODELCONNECTOR_H
