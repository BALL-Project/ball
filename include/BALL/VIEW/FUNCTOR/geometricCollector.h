// $Id: geometricCollector.h,v 1.2 2000/06/15 17:22:09 oliver Exp $

#ifndef BALL_VIEW_FUNCTOR_GEOMETRICCOLLECTOR_H
#define BALL_VIEW_FUNCTOR_GEOMETRICCOLLECTOR_H

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


namespace BALL
{
	
	namespace VIEW
	{

		class GeometricCollector
			: public UnaryProcessor<Composite>
		{
			public:

			// --- CONSTRUCTORS

			GeometricCollector();

			GeometricCollector(const GeometricCollector& collector, bool deep = true);

			virtual ~GeometricCollector();

			virtual void clear();

			virtual void destroy();

			// --- ASSIGNMENT METHODS

			void set(const GeometricCollector& collector, bool deep = true);

			GeometricCollector& operator = (const GeometricCollector& collector);

			void get(GeometricCollector& collector, bool deep = true) const;

			void swap(GeometricCollector& collector);

			// --- ACCESSORS: INSPECTORS and MUTATORS

			List<Composite *>& getCollection();
			
			const List<Composite *>& getCollection() const;

			void collectSelectedGeometricObjects(bool selected);

			bool collectingSelectedGeometricObjects() const;

			virtual bool start();
			
			virtual bool finish();
			
			// --- OPERATORS

			virtual Processor::Result operator()
				(Composite& composite);

			// --- DEBUGGERS and DIAGNOSTICS

			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;

			// --- STORERS

			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;

			
			private:

			bool only_selected_objects_;

			List<Composite*> collection_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/FUNCTOR/geometricCollector.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_FUNCTOR_GEOMETRICCOLLECTOR_H 
