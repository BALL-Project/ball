// $Id: findGeometricObject.h,v 1.3 1999/12/28 18:38:06 oliver Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_FINDGEOMETRICOBJECT_H
#define BALL_MOLVIEW_FUNCTOR_FINDGEOMETRICOBJECT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
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
		class FindGeometricObject
			: public UnaryProcessor<Composite>,
				public ExtendedPropertyManager
		{
			public:

			/**	@name Constructors and Destructors
			*/
			//@{

			FindGeometricObject();

			FindGeometricObject
				(const FindGeometricObject& find_geometric_object, bool deep = true);

			virtual ~FindGeometricObject();

			virtual void clear();

			virtual void destroy();
			//@}

			
			/**	@name	Assignment
			*/
			//@{

			void set
				(const FindGeometricObject& find_geometric_object, bool deep = true);

			FindGeometricObject& operator =
				(const FindGeometricObject& find_geometric_object);

			void get
				(FindGeometricObject& find_geometric_object, bool deep = true) const;

			void swap(FindGeometricObject& find_geometric_object);
			//@}

			/**	@name	Accessors
			*/
			//@{

			VIEW::GeometricObject* getGeometricObject() const;

			//@}
			
			
			/**	@name	Processor Related Methods
			*/
			//@{

			virtual bool start();
			
			virtual bool finish();	

			virtual Processor::Result operator () (Composite& composite);

			//@}
				

			/**	@name	Predicates
			*/
			//@{
			bool geometricObjectFound() const;
			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;
			//@}


			
			private:  

			VIEW::GeometricObject* geometric_object_;
		};

			
#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/findGeometricObject.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_FINDGEOMETRICOBJECT_H
