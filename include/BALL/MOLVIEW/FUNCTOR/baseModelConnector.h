// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: baseModelConnector.h,v 1.10 2003/03/03 14:18:03 anhi Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_BASEMODELCONNECTOR_H
#define BALL_MOLVIEW_FUNCTOR_BASEMODELCONNECTOR_H

#ifndef BALL_CONCEPT_VISITOR_H
#	include <BALL/CONCEPT/visitor.h>
#endif

#ifndef BALL_CONCEPT_PROPERTY_H
#	include <BALL/CONCEPT/property.h>
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
    /**  \addtogroup  MolviewFunctorsBase
     *  @{
     */		
		/** BaseModelConnector class.
				The class BaseModelConnector is derived from  \link Visitor Visitor \endlink  and
				 \link PropertyManager PropertyManager \endlink . It contains therefore the visitor interface
				and the property concept.
				This class is a base class for all model connector classes (e.g.  \link AtomBondModelConnector AtomBondModelConnector \endlink ).
				A model connector class is used by the model classes to create a connection
				between different types of models.
				If one  \link Atom Atom \endlink  object has the model {\em Ball And Stick} and its neighbor  \link Atom Atom \endlink 
				object has the model {\em Van Der Waals} than the model connector class determines
				the model of the  \link Bond Bond \endlink  object between these two atoms (in this case a 
				{\em Ball And Stick} representation would be chosen). If a new model is introduced
				the model connector class must be extended to deal with this new model and
				its connections to the other models.
				This class has access to the class  \link FindGeometricObjects FindGeometricObjects \endlink  and the class 
				 \link ColorCalculator ColorCalculator \endlink .  \par
				<b>Definition:</b> BALL/MOLVIEW/FUNCTOR/baseModelConnector.h
		*/
		class BaseModelConnector
			: public Visitor<Composite>,
			  public PropertyManager
		{
			public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new baseModelConnector.
					Initialize the  \link ColorCalculator ColorCalculator \endlink  of {\em *this} baseModelConnector to the
					default colorCalculator and clears the  \link PropertyManager PropertyManager \endlink  and the
					 \link FindGeometricObjects FindGeometricObjects \endlink  search processor.
					@return      BaseModelConnector new constructed baseModelConnector
					@see         Visitor
					@see         PropertyManager
			*/
			BaseModelConnector()
				throw();

			/** Copy constructor.
					Construct new baseModelConnector by copying the baseModelConnector {\em connector}.
					Copies the properties and the  \link ColorCalculator ColorCalculator \endlink  of {\em connector} to 
					{\em this} baseModelConnector.
					@param       connector the baseModelConnector to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em connector}
					@return      BaseModelConnector new constructed baseModelConnector copied from {\em connector}
					@see         PropertyManager
			*/
			BaseModelConnector
				(const BaseModelConnector& connector, bool deep = true)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} baseModelConnector.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~BaseModelConnector()
				throw();

			/** Explicit default initialization.
					Reset the state of {\em *this} baseModelConnector to:
					\begin{itemize}
					  \item  clear the properties
					  \item  clear the  \link FindGeometricObjects FindGeometricObjects \endlink 
					  \item  set the  \link ColorCalculator ColorCalculator \endlink  to the default colorCalculator
					\end{itemize}
					Calls  \link PropertyManager::clear PropertyManager::clear \endlink .
					Calls  \link FindGeometricObjects::clear FindGeometricObjects::clear \endlink .
					@see  FindGeometricObjects
					@see  ColorCalculator
					@see  PropertyManager
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
					Assign the baseModelConnector {\em connector} to {\em *this} baseModelConnector.
					The properties and the  \link ColorCalculator ColorCalculator \endlink  of {\em *this} are initialized to 
					the properties and the colorCalculator of the baseModelConnector {\em connector}.
					Calls  \link PropertyManager::set PropertyManager::set \endlink .
					@param       connector the baseModelConnector to be copied 
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em connector}
					@see         PropertyManager
					@see         ColorCalculator
			*/
			void set
				(const BaseModelConnector& connector, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the baseModelConnector {\em connector} to {\em *this} baseModelConnector.
					Calls  \link set set \endlink .
					The properties and the  \link ColorCalculator ColorCalculator \endlink  of {\em *this} are initialized to 
					the properties and the colorCalculator of the baseModelConnector {\em connector}.
					The assignment will be deep.
					@param       connector the baseModelConnector to be copied
					@return      BaseModelConnector& constant reference of {\em *this} baseModelConnector
					@see         set
			*/
			const BaseModelConnector& operator =
				(const BaseModelConnector& connector)
				throw();

			/** Copying.
					Copy {\em *this} baseModelConnector to the baseModelConnector {\em connector}.
					Calls  \link set set \endlink .
					The properties and the  \link ColorCalculator ColorCalculator \endlink  of {\em *this} are initialized to 
					the properties and the colorCalculator of the baseModelConnector {\em connector}.
					@param       connector the baseModelConnector to be assigned to
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em connector}
					@see         set
			*/
			void get
				(BaseModelConnector& connector, bool deep = true) const
				throw();

			/** Swapping of baseModelConnector.
					Swap the properties and the  \link ColorCalculator ColorCalculator \endlink  of {\em *this} baseModelConnector
					with those of the baseModelConnector {\em connector}.
					Calls  \link PropertyManager::swap PropertyManager::swap \endlink 
					Calls  \link ColorCalculator::swap ColorCalculator::swap \endlink 
					@param       connector the baseModelConnector being swapped with {\em *this} baseModelConnector 
					@see         PropertyManager
					@see         ColorCalculator
			*/
			void swap
				(BaseModelConnector& connector)
				throw();
			//@}

			
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Register a colorCalculator.
					Change the  \link ColorCalculator ColorCalculator \endlink  of {\em *this} baseModelConnector.
					@param   color_calculator the new  \link ColorCalculator ColorCalculator \endlink  for {\em *this} baseModelConnector
					@see     unregisterColorCalculator
					@see     ColorCalculator
			*/
			void registerColorCalculator
				(const ColorCalculator& color_calculator)
				throw();

			/** Unregister the colorCalculator.
					Change the  \link ColorCalculator ColorCalculator \endlink  of {\em *this} baseModelConnector to the
					default colorCalculator.
					@see     registerColorCalculator
					@see     ColorCalculator
			*/
			void unregisterColorCalculator()
				throw();

			/** Mutable inspection of the colorCalculator.
					Access the mutable pointer to the  \link ColorCalculator ColorCalculator \endlink  of {\em *this} baseModelConnector.
					@return  ColorCalculator* a pointer to the colorCalculator of {\em *this} baseModelConnector
					@see     registerColorCalculator
					@see     unregisterColorCalculator
					@see     ColorCalculator
			*/
			ColorCalculator* getColorCalculator() const
				throw();

			/** Change the properties.
					Change the properties of {\em *this} baseModelConnector by copying the properties
					of {\em manager} to {\em *this} baseModelConnector.
					Calls  \link PropertyManager::set PropertyManager::set \endlink .
					This method is provided for convenience.
					@param  manager the  \link PropertyManager PropertyManager \endlink  to be copied to {\em *this} baseModelConnector
					@see    PropertyManager
			*/
			void setProperties(PropertyManager &manager)
				throw();

			/** Mutable inspection of the search processor.
					Access the mutable reference to  \link FindGeometricObjects FindGeometricObjects \endlink  of {\em *this}
					baseModelConnector.
					This object is used to search for specific  \link GeometricObject GeometricObject \endlink  objects that
					have certain properties.
					@return  FindGeometricObjects& a mutable reference to the  \link FindGeometricObjects FindGeometricObjects \endlink  of {\em *this} baseModelConnector
					@see     FindGeometricObjects
			*/
			FindGeometricObjects& getGeometricObjectSearcher()
				throw();

			/** Visit method.
					This method will be overridden from the derived classes to implement the
					creation of the connector models between the different models.
			*/
			virtual void visit(Composite& composite);
			//@}
			

			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal value dump.
					Dump the current state of {\em *this} baseModelConnector to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls  \link ColorCalculator::dump ColorCalculator::dump \endlink .
					@param   s output stream where to output the state of {\em *this} baseModelConnector
					@param   depth the dumping depth
			*/
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
  /** @} */
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_BASEMODELCONNECTOR_H
