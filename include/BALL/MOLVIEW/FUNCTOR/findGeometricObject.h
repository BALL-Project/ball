// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: findGeometricObject.h,v 1.14 2003/03/14 11:49:34 sturm Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_FINDGEOMETRICOBJECTS_H
#define BALL_MOLVIEW_FUNCTOR_FINDGEOMETRICOBJECTS_H

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_MOLVIEW_KERNEL_EXTENDEDPROPERTY_H
#	include <BALL/MOLVIEW/KERNEL/extendedProperty.h>
#endif

#ifndef BALL_MOLVIEW_COMMONC_COMMON_H
# include <BALL/MOLVIEW/COMMON/common.h>
#endif

using BALL::VIEW::GeometricObject;

namespace BALL
{
	namespace MOLVIEW
	{
    /**  \addtogroup  MolviewFunctorsHelper
     *  @{
     */		
		/** FindGeometricObjects class.
				The class FindGeometricObjects is derived from  \link UnaryProcessor UnaryProcessor \endlink  and
				 \link ExtendedPropertyManager ExtendedPropertyManager \endlink . It contains therefore the processor interface
				and the property concept. This class is used to find  \link GeometricObject GeometricObject \endlink  objects that
				are available in a  \link Composite Composite \endlink  object 
				that have the same properties as {\em *this} findGeometricObjects. The found objects
				can be accessed by the method  \link getGeometricObjects getGeometricObjects \endlink .
				This class is used by some model processors (e.g.  \link AddBallAndStickModelProcessor AddBallAndStickModelProcessor \endlink )
				to find  \link GeometricObject GeometricObject \endlink  objects they will change or delete.  \par
				
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
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em find_geometric_objects}
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
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~FindGeometricObjects()
				throw();

			/** Explicit default initialization.
					Reset the state of {\em *this} findGeometricObjects to:

					  -  clears the properties
					  -  the list of  \link GeometricObject GeometricObject \endlink  objects is cleared
					
					Calls  \link ExtendedPropertyManager::clear ExtendedPropertyManager::clear \endlink .
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
					Calls  \link clear clear \endlink .
					Calls  \link ExtendedPropertyManager::set ExtendedPropertyManager::set \endlink .
					@param       find_geometric_objects the findGeometricObjects to be copied 
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em find_geometric_objects}
					@see         ExtendedPropertyManager
					@see         clear
			*/
			void set
				(const FindGeometricObjects& find_geometric_objects, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the findGeometricObjects {\em find_geometric_objects} to {\em *this}
					findGeometricObjects.
					Calls  \link set set \endlink .
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
					Calls  \link set set \endlink .
					The properties of the findGeometricObjects {\em find_geometric_objects} are
					initialized to the properties of {\em *this} findGeometricObjects.
					@param       find_geometric_objects the findGeometricObjects to be assigned to
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em find_geometric_objects}
					@see         set
			*/
			void get
				(FindGeometricObjects& find_geometric_objects, bool deep = true) const
				throw();

			/** Swapping of findGeometricObjects.
					Swap the properties of {\em *this} findGeometricObjects with the findGeometricObjects
					{\em find_geometric_objects}.
					Calls  \link ExtendedPropertyManager::swap ExtendedPropertyManager::swap \endlink 
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
					Access the mutable reference of the found  \link GeometricObject GeometricObject \endlink  objects
					as a list of  \link GeometricObject GeometricObject \endlink  objects from {\em *this}
					findGeometricObjects.
					@return  List<GeometricObject*> a mutable reference to the collected  \link GeometricObject GeometricObject \endlink  objects that have the specified properties of {\em *this} findGeometricObjects
					@see     start
					@see     finish
					@see     geometricObjectsFound
					@see     GeometricObject
			*/
			List<GeometricObject*>& getGeometricObjects()
				throw();

			/** Non-mutable inspection of the collection list.
					Access the constant reference of the found  \link GeometricObject GeometricObject \endlink  objects
					as a list of  \link GeometricObject GeometricObject \endlink  objects from {\em *this}
					findGeometricObjects.
					@return  List<GeometricObject*> a constant reference to the collected  \link GeometricObject GeometricObject \endlink  objects that have the specified properties of {\em *this} findGeometricObjects
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
					Clears the list of found  \link GeometricObject GeometricObject \endlink  objects.
					@return bool <tt>true</tt> if the start of {\em *this} findGeometricObjects was successful, <tt>false</tt> otherwise
					@see    operator()
					@see    getGeometricObjects
			*/
			virtual bool start();
			
			/** Finish method.
					Empty for further purpose. Override this method for finalizing
					the findGeometricObjects.
					@return bool <tt>true</tt> if the finish of {\em *this} findGeometricObjects was successful, <tt>false</tt> otherwise
					@see    operator()
			*/
			virtual bool finish();	

			/**	Operator method.
					Collect all  \link GeometricObject GeometricObject \endlink  objects that are
					available in the given  \link Composite Composite \endlink  object {\em composite} and have the same
					properties as {\em *this} findGeometricObjects.
					The found objects are stored in a list that can be accessed by the method
					 \link getGeometricObjects getGeometricObjects \endlink .
					@param  composite the  \link Composite Composite \endlink  to be searched for  \link GeometricObject GeometricObject \endlink  objects
					@return Processor::Result the result of {\em *this} findGeometricObjects
					@see    getGeometricObjects
					@see    ExtendedPropertyManager
					@see    GeometricObject
					@see    Processor
					@see    Composite
			*/
			virtual Processor::Result operator () (Composite& composite);

			//@}
				

			/**	@name	Predicates
			*/
			//@{
			/** Test if objects are found.
					Test if  \link GeometricObject GeometricObject \endlink  objects are found that have the specified properties
					of {\em *this} findGeometricObjects.
					@return  bool <tt>true</tt> if  \link GeometricObject GeometricObject \endlink  objects are found, <tt>false</tt> otherwise
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
					(self-validated) and consistent <tt>true</tt> is returned, <tt>false</tt> otherwise. 
					{\em *this} findGeometricObjects is valid if:
					 \link ExentedPropertyManager ExentedPropertyManager \endlink  is valid.
					Calls  \link ExentedPropertyManager::isValid ExentedPropertyManager::isValid \endlink .
					@return			bool <tt>true</tt> if the internal state of {\em *this} findGeometricObjects is correct (self-validated) and consistent,
					 						<tt>false</tt> otherwise
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
  /** @} */
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_FINDGEOMETRICOBJECTS_H
