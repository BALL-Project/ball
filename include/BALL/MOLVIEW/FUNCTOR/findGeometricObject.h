// $Id: findGeometricObject.h,v 1.5 2000/12/12 16:16:52 oliver Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_FINDGEOMETRICOBJECTS_H
#define BALL_MOLVIEW_FUNCTOR_FINDGEOMETRICOBJECTS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
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
		class FindGeometricObjects
			: public UnaryProcessor<Composite>,
				public ExtendedPropertyManager
		{
			public:

			/**	@name Constructors and Destructors
			*/
			//@{

			FindGeometricObjects();

			FindGeometricObjects
				(const FindGeometricObjects& find_geometric_objects, bool deep = true);

			virtual ~FindGeometricObjects()
				throw();

			virtual void clear()
				throw();

			virtual void destroy()
				throw();
			//@}

			
			/**	@name	Assignment
			*/
			//@{

			void set
				(const FindGeometricObjects& find_geometric_objects, bool deep = true);

			FindGeometricObjects& operator =
				(const FindGeometricObjects& find_geometric_objects);

			void get
				(FindGeometricObjects& find_geometric_objects, bool deep = true) const;

			void swap(FindGeometricObjects& find_geometric_objects);
			//@}

			/**	@name	Accessors
			*/
			//@{

			List<VIEW::GeometricObject*>& getGeometricObjects();

			const List<VIEW::GeometricObject*>& getGeometricObjects() const;

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
			bool geometricObjectsFound() const;
			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}


			
			private:  

			List<VIEW::GeometricObject*> geometric_objects_;
		};

			
#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/findGeometricObject.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_FINDGEOMETRICOBJECTS_H
