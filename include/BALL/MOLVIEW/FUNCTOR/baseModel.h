// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: baseModel.h,v 1.17 2003/03/03 14:18:03 anhi Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_BASEMODEL_H
#define BALL_MOLVIEW_FUNCTOR_BASEMODEL_H

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_BASEMODELCONNECTOR_H
#	include <BALL/MOLVIEW/FUNCTOR/baseModelConnector.h>
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
    /**  \addtogroup  MolviewFunctorsBase
     *  @{
     */
		/** BaseModelProcessor class.
				The class BaseModelProcessor is derived from  \link UnaryProcessor UnaryProcessor \endlink  and
				 \link ExtendedPropertyManager ExtendedPropertyManager \endlink . It contains therefore the processor interface
				and the property concept.
				The class BaseModelProcessor is base class for all model processors. 
				A model can be created for static mode or dynamic mode therefore there are methods
				to switch and check the actual mode of {\em *this} baseModelProcessor.
				See  \link GeometricObject GeometricObject \endlink  for information about static and dynamic object creation.
				Every model must have a  \link ColorCalculator ColorCalculator \endlink  and a  \link BaseModelConnector BaseModelConnector \endlink  object
				so interface methods are available too.
				Some protected helper methods are included that are used by some model processors.  \par
				<b>Definition:</b> BALL/MOLVIEW/FUNCTOR/baseModel.h
		*/
		class BaseModelProcessor
			: public UnaryProcessor<Composite>,
			  public ExtendedPropertyManager
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new baseModelProcessor.
					Initialize the  \link ColorCalculator ColorCalculator \endlink  and the  \link BaseModelConnector BaseModelConnector \endlink  of
					{\em *this} baseModelConnector to the default colorCalculator and default baseModelConnector
					and clears the  \link ExtendedPropertyManager ExtendedPropertyManager \endlink  and the  \link FindGeometricObjects FindGeometricObjects \endlink 
					search processor.
					@return      BaseModelProcessor new constructed baseModelProcessor
					@see         ExtendedPropertyManager
					@see         ColorCalculator
					@see         BaseModelConnector
					@see         FindGeometricObjects
			*/
			BaseModelProcessor()
				throw();

			/** Copy constructor.
					Construct new baseModelProcessor by copying the baseModelProcessor
					{\em base_model_processor}.
					Copies the properties, the  \link ColorCalculator ColorCalculator \endlink  and the  \link BaseModelConnector BaseModelConnector \endlink 
					of {\em base_model_processor} to {\em this} baseModelProcessor.
					@param       base_model_processor the baseModelProcessor to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em base_model_processor}
					@return      BaseModelProcessor new constructed baseModelProcessor copied from {\em base_model_processor}
					@see         ExtendedPropertyManager
					@see         ColorCalculator
					@see         BaseModelConnector
					@see         FindGeometricObjects
			*/
			BaseModelProcessor(const BaseModelProcessor& base_model_processor, bool deep = true)
			  throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} baseModelProcessor.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~BaseModelProcessor()
				throw();

			/** Explicit default initialization.
					Reset the state of {\em *this} baseModelProcessor to:
					\begin{itemize}
					  \item  clear the properties
					  \item  clear the  \link FindGeometricObjects FindGeometricObjects \endlink 
					  \item  set the  \link ColorCalculator ColorCalculator \endlink  to the default colorCalculator
					  \item  set the  \link BaseModelConnector BaseModelConnector \endlink  to the default baseModelConnector
					\end{itemize}
					Calls  \link ExtendedPropertyManager::clear ExtendedPropertyManager::clear \endlink .
					Calls  \link FindGeometricObjects::clear FindGeometricObjects::clear \endlink .
					Calls  \link clear_ clear_ \endlink 
					@see  BaseModelConnector
					@see  FindGeometricObjects
					@see  ColorCalculator
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
					Assign the baseModelProcessor {\em base_model_processor} to {\em *this} baseModelProcessor.
					Initializes the  \link ExtendedPropertyManager ExtendedPropertyManager \endlink ,  \link BaseModelConnector BaseModelConnector \endlink , 
					 \link FindGeometricObject FindGeometricObject \endlink  and the  \link ColorCalculator ColorCalculator \endlink  of {\em *this} baseModelProcessor
					to those of {\em base_model_processor}.
					Calls  \link clear_ clear_ \endlink .
					Calls  \link ExtendedPropertyManager::set ExtendedPropertyManager::set \endlink .
					Calls  \link FindGeometricObject::set FindGeometricObject::set \endlink .
					@param       base_model_processor the baseModelProcessor to be copied 
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em base_model_processor}
					@see         BaseModelConnector
					@see         ExtendedPropertyManager
					@see         FindGeometricObject
					@see         ColorCalculator
			*/
			void set
				(const BaseModelProcessor& base_model_processor, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the baseModelProcessor {\em base_model_processor} to {\em *this} baseModelProcessor.
					Initializes the  \link ExtendedPropertyManager ExtendedPropertyManager \endlink ,  \link BaseModelConnector BaseModelConnector \endlink , 
					 \link FindGeometricObject FindGeometricObject \endlink  and the  \link ColorCalculator ColorCalculator \endlink  of {\em *this} baseModelProcessor
					to those of {\em base_model_processor}.
					Calls  \link set set \endlink .
					The assignment will be deep.
					@param       base_model_processor the baseModelProcessor to be copied
					@return      BaseModelProcessor& constant reference of {\em *this} baseModelProcessor
					@see         set
			*/
			const BaseModelProcessor& operator = (const BaseModelProcessor& base_model_processor)
				throw();

			/** Copying.
					Copy {\em *this} baseModelProcessor to the baseModelProcessor {\em connector}.
					Initializes the  \link ExtendedPropertyManager ExtendedPropertyManager \endlink ,  \link BaseModelConnector BaseModelConnector \endlink , 
					 \link FindGeometricObject FindGeometricObject \endlink  and the  \link ColorCalculator ColorCalculator \endlink  of {\em *this} baseModelProcessor
					to those of {\em base_model_processor}.
					Calls  \link set set \endlink .
					@param       base_model_processor the baseModelProcessor to be assigned to
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em base_model_processor}
					@see         set
			*/
			void get(BaseModelProcessor& base_model_processor, bool deep = true) const
				throw();

			/** Swapping of baseModelProcessor's.
					Swap the  \link ExtendedPropertyManager ExtendedPropertyManager \endlink ,  \link BaseModelConnector BaseModelConnector \endlink , 
					 \link FindGeometricObject FindGeometricObject \endlink  and the  \link ColorCalculator ColorCalculator \endlink  of {\em *this} baseModelProcessor
					with those of {\em base_model_processor}.
					Calls  \link ExtendedPropertyManager::swap ExtendedPropertyManager::swap \endlink 
					Calls  \link FindGeometricObject::swap FindGeometricObject::swap \endlink 
					@param       connector the baseModelConnector being swapped with {\em *this} baseModelProcessor 
					@see         BaseModelConnector
					@see         ExtendedPropertyManager
					@see         FindGeometricObject
					@see         ColorCalculator
			*/
			void swap(BaseModelProcessor& base_model_processor)
				throw();

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			
			/** Enable creation of static models.
					If this method is called {\em *this} baseModelProcessor will create models with
					the property <tt>PROPERTY__OBJECT_STATIC</tt>.
					See  \link GeometricObject GeometricObject \endlink  for information about static models.
					The method  \link disableDynamicModel disableDynamicModel \endlink  will be do exactly the same as this method.
					@see  disableStaticModel
					@see  disableDynamicModel
					@see  isStaticModelEnabled
					@see  GeometricObject
			*/
			void enableStaticModel()
				throw();

			/** Disable creation of static models.
					If this method is called {\em *this} baseModelProcessor will create models with
					the property <tt>PROPERTY__OBJECT_DYNAMIC</tt>.
					See  \link GeometricObject GeometricObject \endlink  for information about dynamic models.
					The method  \link enableDynamicModel enableDynamicModel \endlink  will be do exactly the same as this method.
					@see  enableStaticModel
					@see  enableDynamicModel
					@see  isStaticModelDisabled
					@see  GeometricObject
			*/
			void disableStaticModel()
				throw();

			/** Enable creation of dynamic models.
					If this method is called {\em *this} baseModelProcessor will create models with
					the property <tt>PROPERTY__OBJECT_DYNAMIC</tt>.
					See  \link GeometricObject GeometricObject \endlink  for information about dynamic models.
					The method  \link disableStaticModel disableStaticModel \endlink  will be do exactly the same as this method.
					@see  disableDynamicModel
					@see  disableStaticModel
					@see  isDynamicModelEnabled
					@see  GeometricObject
			*/
			void enableDynamicModel()
				throw();

			/** Disable creation of dynamic models.
					If this method is called {\em *this} baseModelProcessor will create models with
					the property <tt>PROPERTY__OBJECT_STATIC</tt>.
					See  \link GeometricObject GeometricObject \endlink  for information about static models.
					The method  \link enableStaticModel enableStaticModel \endlink  will be do exactly the same as this method.
					@see  enableDynamicModel
					@see  enableStaticModel
					@see  isDynamicModelDisabled
					@see  GeometricObject
			*/
			void disableDynamicModel()
				throw();

			/** Register a colorCalculator.
					Change the  \link ColorCalculator ColorCalculator \endlink  of {\em *this} baseModelProcessor.
					@param   color_calculator the new  \link ColorCalculator ColorCalculator \endlink  for {\em *this} baseModelProcessor
					@see     unregisterColorCalculator
					@see     ColorCalculator
			*/
			void registerColorCalculator(const ColorCalculator& color_calculator)
				throw();

			/** Unregister the colorCalculator.
					Change the  \link ColorCalculator ColorCalculator \endlink  of {\em *this} baseModelProcessor to the
					default colorCalculator.
					@see     registerColorCalculator
					@see     ColorCalculator
			*/
			void unregisterColorCalculator()
				throw();

			/** Mutable inspection of the colorCalculator.
					Access the mutable pointer to the  \link ColorCalculator ColorCalculator \endlink  of {\em *this} baseModelProcessor.
					@return  ColorCalculator* a pointer to the colorCalculator of {\em *this} baseModelProcessor
					@see     registerColorCalculator
					@see     unregisterColorCalculator
					@see     ColorCalculator
			*/
			ColorCalculator* getColorCalculator() const
				throw();

			/** Register a baseModelConnector.
					Change the  \link BaseModelConnector BaseModelConnector \endlink  of {\em *this} baseModelProcessor.
					@param    model_connector the new  \link BaseModelConnector BaseModelConnector \endlink  for {\em *this} baseModelProcessor
					@see     unregisterBaseModelConnector
					@see     BaseModelConnector
			*/
			void registerModelConnector(const BaseModelConnector& model_connector)
				throw();

			/** Unregister the baseModelConnector.
					Change the  \link BaseModelConnector BaseModelConnector \endlink  of {\em *this} baseModelProcessor to the
					default baseModelConnector.
					@see     registerBaseModelConnector
					@see     BaseModelConnector
			*/
			void unregisterModelConnector()
				throw();

			/** Mutable inspection of the baseModelConnector.
					Access the mutable pointer to the  \link BaseModelConnector BaseModelConnector \endlink  of {\em *this} baseModelProcessor.
					@return  BaseModelConnector* a pointer to the colorCalculator of {\em *this} baseModelProcessor
					@see     registerBaseModelConnector
					@see     unregisterBaseModelConnector
					@see     BaseModelConnector
			*/
			BaseModelConnector* getModelConnector()
				throw();

			//@}
			/**	@name Processor specific methods
			*/
			//@{
			
			/** Start method.
					Set the properties of  \link FindGeometricObjects FindGeometricObjects \endlink  search processor and the properties
					of  \link BaseModelConnector BaseModelConnector \endlink  to the properties of {\em *this} baseModelProcessor
					and registers the  \link ColorCalculator ColorCalculator \endlink  of {\em *this} baseModelProcessor to
					 \link BaseModelConnector BaseModelConnector \endlink .
					@return bool <tt>true</tt> if the start of {\em *this} baseModelProcessor was successful, <tt>false</tt> otherwise
					@see    operator()
					@see    BaseModelConnector
					@see    FindGeometricObjects
			*/
			virtual bool start();
			
			/** Finish method.
					Empty for further purpose. Override this method for finalizing
					the models.
					@return bool <tt>true</tt> if the finish of {\em *this} baseModelProcessor was successful, <tt>false</tt> otherwise
					@see    operator()
			*/
			virtual bool finish();

			/**	Operator method.
					This method will be overridden to implement the creation of the different models.
					@param  composite the  \link Composite Composite \endlink  object that will be processed
					@return Processor::Result the result of {\em *this} baseModel
					@see    start
					@see    finish
					@see    Composite
			*/
			virtual Processor::Result operator() (Composite& composite);

			//@}
			/**	@name	Predicates
			*/
			//@{

			/** Test if creation of static models is enabled.
					Test if the property <tt>PROPERTY__OBJECT_STATIC</tt> is set for {\em *this} baseModelProcessor.
					See  \link GeometricObject GeometricObject \endlink  for information about this property.
					@return  bool <tt>true</tt> if static models of {\em *this} baseModelProcessor should be created, <tt>false</tt> otherwise
					@see     enableStaticModel
					@see     disableStaticModel
					@see     GeometricObject
					@see     ExtendedPropertyManager
			*/
			bool isStaticModelEnabled()
				throw();

			/** Test if creation of static models is disabled.
					Test if the property <tt>PROPERTY__OBJECT_STATIC</tt> is not set for {\em *this} baseModelProcessor.
					See  \link GeometricObject GeometricObject \endlink  for information about this property.
					@return  bool <tt>true</tt> if static models of {\em *this} baseModelProcessor should not be created, <tt>false</tt> otherwise
					@see     enableStaticModel
					@see     disableStaticModel
					@see     GeometricObject
					@see     ExtendedPropertyManager
			*/
			bool isStaticModelDisabled()
				throw();

			/** Test if creation of dynamic models is enabled.
					Test if the property <tt>PROPERTY__OBJECT_DYNAMIC</tt> is set for {\em *this} baseModelProcessor.
					See  \link GeometricObject GeometricObject \endlink  for information about this property.
					@return  bool <tt>true</tt> if dynamic models of {\em *this} baseModelProcessor should be created
					@see     enableDynamicModel
					@see     disableDynamicModel
					@see     GeometricObject
					@see     ExtendedPropertyManager
			*/
			bool isDynamicModelEnabled()
				throw();

			/** Test if creation of dynamic models is disabled.
					Test if the property <tt>PROPERTY__OBJECT_DYNAMIC</tt> is not set for {\em *this} baseModelProcessor.
					See  \link GeometricObject GeometricObject \endlink  for information about this property.
					@return  bool <tt>true</tt> if dynamic models of {\em *this} baseModelProcessor should not be created
					@see     enableDynamicModel
					@see     disableDynamicModel
					@see     GeometricObject
					@see     ExtendedPropertyManager
			*/
			bool isDynamicModelDisabled()
				throw();

			/** Test if the default colorCalculator is registered.
					Test if the default  \link ColorCalculator ColorCalculator \endlink  is registered for {\em *this} 
					atomBondModelConnector.
					@return  bool <tt>true</tt> if the default  \link ColorCalculator ColorCalculator \endlink  is registered, <tt>false</tt> otherwise
					@see     registerColorCalculator
					@see     unregisterColorCalculator
					@see     getColorCalculator
					@see     ColorCalculator
			*/
			bool isDefaultColorCalculatorRegistered()
				throw();

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} atomBondModelConnector.
					If the internal state of {\em *this} atomBondModelConnector is correct 
					(self-validated) and consistent <tt>true</tt> is returned, <tt>false</tt> otherwise. 
					{\em *this} atomBondModelConnector is valid if:
					 \link ExentedPropertyManager ExentedPropertyManager \endlink ,  \link BaseModelConnector BaseModelConnector \endlink  and  \link FindGeometricObjects FindGeometricObjects \endlink 
					is valid.
					Calls  \link ExentedPropertyManager::isValid ExentedPropertyManager::isValid \endlink .
					Calls  \link BaseModelConnector::isValid BaseModelConnector::isValid \endlink .
					Calls  \link FindGeometricObjects::isValid FindGeometricObjects::isValid \endlink .
					@return			bool <tt>true</tt> if the internal state of {\em *this}atomBondModelConnector is correct
					@see       ExentedPropertyManager
					@see       BaseModelConnector
					@see       FindGeometricObjects
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of {\em *this} atomBondModelConnector to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the state of {\em *this} atomBondModelConnector
					@param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			protected:

			//@}
			/** @name Protected members
			*/
			//@{
			
			/** Mutable inspection of the search processor.
					Access the mutable reference to  \link FindGeometricObjects FindGeometricObjects \endlink  of {\em *this}
					atomBondModelConnector.
					This object is used to search for specific  \link GeometricObject GeometricObject \endlink  objects that
					have certain properties.
					@return  FindGeometricObjects& a mutable reference to the  \link FindGeometricObjects FindGeometricObjects \endlink  
					@see     FindGeometricObjects
			*/
			FindGeometricObjects& getSearcher_()
				throw();

			/** Test if a geometricObject is a model.
					Test if  \link GeometricObject GeometricObject \endlink  {\em geometric_object} has a model property.
					Return <tt>true</tt> if a model property is set in {\em geometric_object}.
					This method is called from the method  \link removeGeometricObjects_ removeGeometricObjects_ \endlink .
					The following models are recognized:
					\begin{itemize}
					  \item {\em PROPERTY__MODEL_BALL_AND_STICK}
					  \item {\em PROPERTY__MODEL_DOTS}
					  \item {\em PROPERTY__MODEL_LINES}
					  \item {\em PROPERTY__MODEL_VDW}
					\end{itemize}
					@param  geometric_object the  \link GeometricObject GeometricObject \endlink  to be tested if a model property is present
					@return bool <tt>true</tt> if a model property is set, <tt>false</tt> otherwise
					@see    removeGeometricObjects_
					@see    GeometricObject
			*/
			bool isModel_(GeometricObject& geometric_object)
				throw();

			/** Remove geometric objects.
					Remove  \link GeometricObject GeometricObject \endlink  objects appended to  \link Composite Composite \endlink  {\em composite}.
					If {\em only_models} is set to <tt>true</tt> (Default) only  \link GeometricObject GeometricObject \endlink 
					objects are removed that have a model property (determined with the method  \link isModel_ isModel_ \endlink ).
					All found  \link GeometricObject GeometricObject \endlink  objects are deleted.
					@param  composite the  \link Composite Composite \endlink  object that should be search for  \link GeometricObject GeometricObject \endlink  objects
					@param  only_models if set to <tt>true</tt>  \link GeometricObject GeometricObject \endlink  objects are searched that have a model property
					@see    isModel_
					@see    GeometricObject
					@see    Composite
			*/
			void removeGeometricObjects_(Composite& composite, bool only_models = false)
				throw();

			/** Resets the properties.
					Resets the properties of {\em *this} baseModelProcessor.
					Set the properties to:
					\begin{itemize}
					  \item <tt>PROPERTY__OBJECT_STATIC</tt>
					  \item <tt>PROPERTY__OBJECT_OPAQUE</tt>
					  \item <tt>PROPERTY__OBJECT_VISIBLE</tt>
					  \item <tt>PROPERTY__OBJECT_CLOSED</tt>
					  \item <tt>PROPERTY__DRAWING_MODE_SOLID</tt>
					  \item <tt>PROPERTY__DRAWING_PRECISION_HIGH</tt>
					\end{itemize}
					Those properties are defined in the class  \link GeometricObject GeometricObject \endlink .
					@see   GeometricObject
			*/
			void clear_()
				throw();
							
			//@}
			
			private:

			void dump_(std::ostream& s, Size depth) const;


			/* search processor */ 
			FindGeometricObjects find_geometric_object_;

			/* pointer to userdefinable table of colors */
			ColorCalculator* color_calculator_;

			/* pointer to a visitor used for connecting the different models */
			BaseModelConnector* model_connector_;
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/baseModel.iC>
#			endif
  /** @} */
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_BASEMODEL_H 
