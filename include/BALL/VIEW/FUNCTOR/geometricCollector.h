// $Id: geometricCollector.h,v 1.3 2001/05/13 13:35:15 hekl Exp $

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

		/** GeometricCollector class.
				{\bf Framework:} BALL/VIEW/FUNCTOR\\
				{\bf Definition:} \URL{BALL/VIEW/FUNCTOR/geometricCollector.h}\\
				{\bf Concept:} processor design pattern\\ \\
				The class GeometricCollector is a \Ref{Processor} that collects
				\Ref{GeometricObject} objects that can either be selected or
				not. It is also a base class for collector objects that can collect
				\Ref{Composite} objects. Override the	methods \Ref{operator},
				\Ref{start} and \Ref{finish} to	change the collection filter criteria. 
				This class is used by the class \Ref{Scene} to collect picked objects.
				@see     GeometricObject
				@see     Scene
				@memo    GeometricCollector class (BALL VIEW functor framework)
				@author  $Author: hekl $
				@version $Revision: 1.3 $
				@date    $Date: 2001/05/13 13:35:15 $
		*/
		class GeometricCollector: public UnaryProcessor<Composite>
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new geometricCollector.
					@return      GeometricCollector new constructed geometricCollector
					Calls \Ref{clear}.
					@see         clear
			*/
			GeometricCollector()
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} geometricCollector.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~GeometricCollector()
				throw();

			/** Explicit default initialization.
					Reset the state of {\em *this} geometricCollector to:
					\begin{itemize}
					  \item  collect all \Ref{GeometricObject} objects (independend of the selection property)
					  \item  the collection list is empty
					\end{itemize}
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
					@see  ~GeometricCollector
			*/
			virtual void destroy()
				throw();

			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Mutable inspection of the collection list.
					Access the mutable reference of the collected \Ref{GeometricObject}
					objects as a list of \Ref{Composite} objects from {\em *this}
					geometricCollector.
					@return  List<Composite*> a mutable reference to the collected \Ref{GeometricObject} objects returned as a list of \Ref{Composite} objects
					@see     collectSelectedGeometricObjects
					@see     collectingSelectedGeometricObjects
					@see     Composite
					@see     GeometricObject
			*/
			List<Composite *>& getCollection()
				throw();
			
			/** Non-mutable inspection of the collection list.
					Access the constant reference of the collected \Ref{GeometricObject}
					objects as a list of \Ref{Composite} objects from {\em *this}
					geometricCollector.
					@return  List<Composite*> a constant reference to the collected \Ref{GeometricObject} objects returned as a list of \Ref{Composite} objects
					@see     collectSelectedGeometricObjects
					@see     collectingSelectedGeometricObjects
					@see     Composite
					@see     GeometricObject
			*/
			const List<Composite *>& getCollection() const
				throw();

			/** Change the collection mode.
					Change the collection mode of {\em *this} geometricCollection.
					@param       selected {\tt true} if {\em only selected} \Ref{GeometricObject} objects should be collected, {\tt false} if {\em all} \Ref{GeometricObject} object should be selected
					@see         getCollection
					@see         GeometricObject
			*/
			void collectSelectedGeometricObjects(bool selected)
				throw();

			/** Inspection of the collection mode.
					Check the collection mode of {\em *this} geometricCollector.
					If {\tt true} is returned than {\em only selected} \Ref{GeometricObject}
					objects are collected. If {\tt false} is returned than {\em all} 
					\Ref{GeometricObject} objects are collected.
					@return      bool {\tt true} if {\em only selected} \Ref{GeometricObject} objects are collected, {\tt false} otherwise.
					@see         getCollection
					@see         GeometricObject
			*/
			bool collectingSelectedGeometricObjects() const
				throw();
			//@}

			/**	@name Processor specific methods
			*/
			//@{
			/** Start method.
					Empty for further purpose. Override this method for initializing
					the geometricCollector.
					@return bool {\tt true} if the start of {\em *this} geometricCollection was successful, {\tt false} otherwise
			*/
			virtual bool start()
				throw();
			
			/** Finish method.
					Empty for further purpose. Override this method for finalizing
					the geometricCollector.
					@return bool {\tt true} if the finish of {\em *this} geometricCollection was successful, {\tt false} otherwise
			*/
			virtual bool finish()
				throw();
			
			/**	Operator ().
					Collect all \Ref{GeometricObject} objects that are
					available in the given \Ref{Composite} object {\em composite}. The collected
					\Ref{GeometricObject} objects can either be selected or not.
					Use the method \Ref{collectSelectedGeometricObjects} to switch
					the filter criteria.
					@param  composite the \Ref{Composite} to be filtered for \Ref{GeometricObject} objects
					@return Processor::Result the result of {\em *this} geometricCollection
					@see    collectSelectedGeometricObjects
					@see    GeometricObject
					@see    Processor
					@see    Composite
			*/
			virtual Processor::Result operator() (Composite& composite)
				throw();
			//@}

			/**	@name	debuggers and diagnostics
			*/
			//@{
			
			/** Internal value dump.
					Dump the current state of {\em *this} geometricCollector to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the state of {\em *this} geometricCollector
					@param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			
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
