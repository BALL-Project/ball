// $Id: findGeometricObject.h,v 1.6 2001/05/13 14:55:22 hekl Exp $

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
		
		/** FindGeometricObjects class.
				{\bf Framework:} BALL/MOLVIEW/FUNCTOR\\
				{\bf Definition:} \URL{BALL/MOLVIEW/FUNCTOR/findGeometricObject.h}\\
				{\bf Concept:} processor design pattern\\
				{\bf Concept:} property design pattern\\ \\
				The class FindGeometricObjects is derived from \Ref{UnaryProcessor} and
				\Ref{ExtendedPropertyManager}. It contains therefore the processor interface
				and the property concept. This class is used to find \Ref{GeometricObject} objects that
				are available in a \Ref{Composite} object 
				that have the same properties as {\em *this} findGeometricObjects. The found objects
				can be accessed by the method \Ref{getGeometricObjects}.
				This class is used by some model processors (e.g. \Ref{AddBallAndStickModelProcessor})
				to find \Ref{GeometricObject} objects they will change or delete.
				@memo    FindGeometricObjects class (BALL MOLVIEW functor framework)
				@author  $Author: hekl $
				@version $Revision: 1.6 $
				@date    $Date: 2001/05/13 14:55:22 $
		*/
		class FindGeometricObjects
			: public UnaryProcessor<Composite>,
				public ExtendedPropertyManager
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new findGeometricObjects.
					@return      FindGeometricObjects new constructed findGeometricObjects
					@see         UnaryProcessor
					@see         ExtendedPropertyManager
			*/
			FindGeometricObjects()
				throw();

			/** Copy constructor.
					Construct new findGeometricObjects by copying the findGeometricObjects {\em find_geometric_objects}.
					@param       find_geometric_objects the findGeometricObjects to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em find_geometric_objects}
					@return      FindGeometricObjects new constructed findGeometricObjects copied from {\em find_geometric_objects}
					@see         ExtendedPropertyManager
			*/
			FindGeometricObjects
				(const FindGeometricObjects& find_geometric_objects, bool deep = true)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} findGeometricObjects.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~FindGeometricObjects()
				throw();

			/** Explicit default initialization.
					Reset the state of {\em *this} findGeometricObjects to:
					\begin{itemize}
					  \item  clears the properties
					  \item  the list of \Ref{GeometricObject} objects is cleared
					\end{itemize}
					Calls \Ref{ExtendedPropertyManager::clear}.
					@see  ExtendedPropertyManager
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();
			//@}

			
			/**	@name	Assignment
			*/
			//@{

			/** Assignment.
					Assign the findGeometricObjects {\em find_geometric_objects} to {\em *this}
					findGeometricObjects.
					The properties of {\em *this} are initialized to the properties of the
					findGeometricObjects {\em find_geometric_objects}.
					Calls \Ref{clear}.
					Calls \Ref{ExtendedPropertyManager::set}.
					@param       find_geometric_objects the findGeometricObjects to be copied 
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em find_geometric_objects}
					@see         ExtendedPropertyManager
					@see         clear
			*/
			void set
				(const FindGeometricObjects& find_geometric_objects, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the findGeometricObjects {\em find_geometric_objects} to {\em *this}
					findGeometricObjects.
					Calls \Ref{set}.
					The properties of {\em *this} are initialized to the properties of the 
					findGeometricObjects {\em find_geometric_objects}. The assignment will be deep.
					@param       find_geometric_objects the findGeometricObjects to be copied
					@return      FindGeometricObjects& constant reference of {\em *this} findGeometricObjects
					@see         set
			*/
			const FindGeometricObjects& operator =
				(const FindGeometricObjects& find_geometric_objects)
				throw();

			/** Copying.
					Copy {\em *this} findGeometricObjects to the findGeometricObjects 
					{\em find_geometric_objects}.
					Calls \Ref{set}.
					The properties of the findGeometricObjects {\em find_geometric_objects} are
					initialized to the properties of {\em *this} findGeometricObjects.
					@param       find_geometric_objects the findGeometricObjects to be assigned to
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em find_geometric_objects}
					@see         set
			*/
			void get
				(FindGeometricObjects& find_geometric_objects, bool deep = true) const
				throw();

			/** Swapping of findGeometricObjects.
					Swap the properties of {\em *this} findGeometricObjects with the findGeometricObjects
					{\em find_geometric_objects}.
					Calls \Ref{ExtendedPropertyManager::swap}
					@param       find_geometric_objects the findGeometricObjects being swapped with {\em *this} findGeometricObjects 
					@see         ExtendedPropertyManager
			*/
			void swap(FindGeometricObjects& find_geometric_objects)
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Mutable inspection of the list of found objects.
					Access the mutable reference of the found \Ref{GeometricObject} objects
					as a list of \Ref{GeometricObject} objects from {\em *this}
					findGeometricObjects.
					@return  List<GeometricObject*> a mutable reference to the collected \Ref{GeometricObject} objects that have the specified properties of {\em *this} findGeometricObjects
					@see     start
					@see     finish
					@see     geometricObjectsFound
					@see     GeometricObject
			*/
			List<GeometricObject*>& getGeometricObjects()
				throw();

			/** Non-mutable inspection of the collection list.
					Access the constant reference of the found \Ref{GeometricObject} objects
					as a list of \Ref{GeometricObject} objects from {\em *this}
					findGeometricObjects.
					@return  List<GeometricObject*> a constant reference to the collected \Ref{GeometricObject} objects that have the specified properties of {\em *this} findGeometricObjects
					@see     start
					@see     finish
					@see     geometricObjectsFound
					@see     GeometricObject
			*/
			const List<GeometricObject*>& getGeometricObjects() const
				throw();

			//@}
			
			
			/**	@name Processor specific methods
			*/
			//@{

			/** Start method.
					Clears the list of found \Ref{GeometricObject} objects.
					@return bool {\tt true} if the start of {\em *this} findGeometricObjects was successful, {\tt false} otherwise
					@see    operator()
					@see    getGeometricObjects
			*/
			virtual bool start()
				throw();
			
			/** Finish method.
					Empty for further purpose. Override this method for finalizing
					the findGeometricObjects.
					@return bool {\tt true} if the finish of {\em *this} findGeometricObjects was successful, {\tt false} otherwise
					@see    operator()
			*/
			virtual bool finish()
				throw();	

			/**	Operator method.
					Collect all \Ref{GeometricObject} objects that are
					available in the given \Ref{Composite} object {\em composite} and have the same
					properties as {\em *this} findGeometricObjects.
					The found objects are stored in a list that can be accessed by the method
					\Ref{getGeometricObjects}.
					@param  composite the \Ref{Composite} to be searched for \Ref{GeometricObject} objects
					@return Processor::Result the result of {\em *this} findGeometricObjects
					@see    getGeometricObjects
					@see    ExtendedPropertyManager
					@see    GeometricObject
					@see    Processor
					@see    Composite
			*/
			virtual Processor::Result operator () (Composite& composite)
				throw();

			//@}
				

			/**	@name	Predicates
			*/
			//@{
			/** Test if objects are found.
					Test if \Ref{GeometricObject} objects are found that have the specified properties
					of {\em *this} findGeometricObjects.
					@return  bool {\tt true} if \Ref{GeometricObject} objects are found, {\tt false} otherwise
					@see     start
					@see     finish
			*/
			bool geometricObjectsFound() const
				throw();
			//@}

			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} findGeometricObjects.
					If the internal state of {\em *this} findGeometricObjects is correct 
					(self-validated) and consistent {\tt true} is returned, {\tt false} otherwise. 
					{\em *this} findGeometricObjects is valid if:
					\Ref{ExentedPropertyManager} is valid.
					Calls \Ref{ExentedPropertyManager::isValid}.
					@return			bool {\tt true} if the internal state of {\em *this} findGeometricObjects is correct (self-validated) and consistent,
					 						{\tt false} otherwise
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of {\em *this} findGeometricObjects to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the state of {\em *this} findGeometricObjects
					@param   depth the dumping depth
			*/
			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}


			
			private:  

			List<GeometricObject*> geometric_objects_;
		};

			
#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/findGeometricObject.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_FINDGEOMETRICOBJECTS_H
