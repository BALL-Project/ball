// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geometricCollector.h,v 1.11 2003/03/03 14:18:26 anhi Exp $

#ifndef BALL_VIEW_FUNCTOR_GEOMETRICCOLLECTOR_H
#define BALL_VIEW_FUNCTOR_GEOMETRICCOLLECTOR_H

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
    /**  \addtogroup  ViewFunctors
     *  @{
     */		
		/** GeometricCollector class.
				The class GeometricCollector is a  \link Processor Processor \endlink  that collects
				 \link GeometricObject GeometricObject \endlink  objects that can either be selected or
				not. It is also a base class for collector objects that can collect
				 \link Composite Composite \endlink  objects. Overrides the	methods  \link operator operator \endlink ,
				 \link start start \endlink  and  \link finish finish \endlink  to	change the collection filter criteria. 
				This class is used by the class  \link Scene Scene \endlink  to collect picked objects.  \par
				<b>Definition:</b> BALL/VIEW/FUNCTOR/geometricCollector.h
				@see     GeometricObject
				@see     Scene
		*/
		class GeometricCollector: public UnaryProcessor<Composite>
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Constructs new geometricCollector.
					Calls  \link clear clear \endlink .
					@return      GeometricCollector new constructed geometricCollector
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
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~GeometricCollector()
				throw();

			/** Explicit default initialization.
					Resets the state of {\em *this} geometricCollector to:
					\begin{itemize}
					  \item  collect all  \link GeometricObject GeometricObject \endlink  objects (independent of the selection property)
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
					Accesses the mutable reference of the collected  \link GeometricObject GeometricObject \endlink 
					objects as a list of  \link Composite Composite \endlink  objects from {\em *this}
					geometricCollector.
					@return  List<Composite*> a mutable reference to the collected  \link GeometricObject GeometricObject \endlink  objects 
									 returned as a list of  \link Composite Composite \endlink  objects
					@see     collectSelectedGeometricObjects
					@see     collectingSelectedGeometricObjects
					@see     Composite
					@see     GeometricObject
			*/
			List<Composite *>& getCollection()
				throw();
			
			/** Non-mutable inspection of the collection list.
			    For further information see {\em List<Composite *>& getCollection()}.
			*/
			const List<Composite *>& getCollection() const
				throw();

			/** Changes the collection mode.
					Changes the collection mode of {\em *this} geometricCollection.
					@param  selected <tt>true</tt> if {\em only selected}  \link GeometricObject GeometricObject \endlink  objects should be collected, 
									<tt>false</tt> if {\em all}  \link GeometricObject GeometricObject \endlink  object should be selected
					@see    getCollection
					@see    GeometricObject
			*/
			void collectSelectedGeometricObjects(bool selected)
				throw();

			/** Inspection of the collection mode.
					Checks the collection mode of {\em *this} geometricCollector.
					If <tt>true</tt> is returned than {\em only selected}  \link GeometricObject GeometricObject \endlink 
					objects are collected. If <tt>false</tt> is returned than {\em all} 
					 \link GeometricObject GeometricObject \endlink  objects are collected.
					@return      bool <tt>true</tt> if {\em only selected}  \link GeometricObject GeometricObject \endlink  objects are collected
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
					Empty for further purpose. Override this method for initializing the geometricCollector.
					@return bool <tt>true</tt> if the start of {\em *this} geometricCollection was successful, <tt>false</tt> otherwise
			*/
			virtual bool start()
				throw();
			
			/** Finish method.
					Empty for further purpose. Override this method for finalizing the geometricCollector.
					@return bool <tt>true</tt> if the finish of {\em *this} geometricCollection was successful, <tt>false</tt> otherwise
			*/
			virtual bool finish()
				throw();
			
			/**	Operator ().
					Collects all  \link GeometricObject GeometricObject \endlink  objects that are
					available in the given  \link Composite Composite \endlink  object {\em composite}. The collected
					 \link GeometricObject GeometricObject \endlink  objects can either be selected or not.
					Uses the method  \link collectSelectedGeometricObjects collectSelectedGeometricObjects \endlink  to switch the filter criteria.
					@param  composite the  \link Composite Composite \endlink  to be filtered for  \link GeometricObject GeometricObject \endlink  objects
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
					Dumps the current state of {\em *this} geometricCollector to 
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
  /** @} */
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_FUNCTOR_GEOMETRICCOLLECTOR_H 
