// $Id: baseModel.h,v 1.9 2001/05/13 14:55:21 hekl Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_BASEMODEL_H
#define BALL_MOLVIEW_FUNCTOR_BASEMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
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

#ifndef BALL_MOLVIEW_FUNCTOR_MOLECULARINFORMATION_H
#	include <BALL/MOLVIEW/FUNCTOR/molecularInformation.h>
#endif

#ifndef BALL_MOLVIEW_KERNEL_EXTENDEDPROPERTY_H
#	include <BALL/MOLVIEW/KERNEL/extendedProperty.h>
#endif

using namespace BALL::VIEW;

namespace BALL
{

	namespace MOLVIEW
	{


		/** BaseModelProcessor class.
				{\bf Framework:} BALL/MOLVIEW/FUNCTOR\\
				{\bf Definition:} \URL{BALL/MOLVIEW/FUNCTOR/baseModel.h}\\
				{\bf Concept:} processor design pattern\\
				{\bf Concept:} property design pattern\\ \\
				The class BaseModelProcessor is derived from \Ref{UnaryProcessor} and
				\Ref{ExtendedPropertyManager}. It contains therefore the processor interface
				and the property concept.
				The class BaseModelProcessor is base class for all model processors. 
				A model can be created for static mode or dynamic mode therefore there are methods
				to switch and check the actual mode of {\em *this} baseModelProcessor.
				See \Ref{GeometricObject} for information about static and dynamic object creation.
				Every model must have a \Ref{ColorCalculator} and a \Ref{BaseModelConnector} object
				so interface methods are available too.
				Some protected helper methods are included that are used by some model processors.
				@memo    BaseModelProcessor class (BALL MOLVIEW functor framework)
				@author  $Author: hekl $
				@version $Revision: 1.9 $
				@date    $Date: 2001/05/13 14:55:21 $
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
					Initialize the \Ref{ColorCalculator} and the \Ref{BaseModelConnector} of
					{\em *this} baseModelConnector to the default colorCalculator and default baseModelConnector
					and clears the \Ref{ExtendedPropertyManager} and the \Ref{FindGeometricObjects}
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
					Copies the properties, the \Ref{ColorCalculator} and the \Ref{BaseModelConnector}
					of {\em base_model_processor} to {\em this} baseModelProcessor.
					@param       base_model_processor the baseModelProcessor to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em base_model_processor}
					@return      BaseModelProcessor new constructed baseModelProcessor copied from {\em base_model_processor}
					@see         ExtendedPropertyManager
					@see         ColorCalculator
					@see         BaseModelConnector
					@see         FindGeometricObjects
			*/
			BaseModelProcessor
				(const BaseModelProcessor& base_model_processor, bool deep = true)
			  throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} baseModelProcessor.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~BaseModelProcessor()
				throw();

			/** Explicit default initialization.
					Reset the state of {\em *this} baseModelProcessor to:
					\begin{itemize}
					  \item  clear the properties
					  \item  clear the \Ref{FindGeometricObjects}
					  \item  set the \Ref{ColorCalculator} to the default colorCalculator
					  \item  set the \Ref{BaseModelConnector} to the default baseModelConnector
					\end{itemize}
					Calls \Ref{ExtendedPropertyManager::clear}.
					Calls \Ref{FindGeometricObjects::clear}.
					Calls \Ref{clear_}
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
					Initializes the \Ref{ExtendedPropertyManager}, \Ref{BaseModelConnector}, 
					\Ref{FindGeometricObject} and the \Ref{ColorCalculator} of {\em *this} baseModelProcessor
					to those of {\em base_model_processor}.
					Calls \Ref{clear_}.
					Calls \Ref{ExtendedPropertyManager::set}.
					Calls \Ref{FindGeometricObject::set}.
					@param       base_model_processor the baseModelProcessor to be copied 
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em base_model_processor}
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
					Initializes the \Ref{ExtendedPropertyManager}, \Ref{BaseModelConnector}, 
					\Ref{FindGeometricObject} and the \Ref{ColorCalculator} of {\em *this} baseModelProcessor
					to those of {\em base_model_processor}.
					Calls \Ref{set}.
					The assignment will be deep.
					@param       base_model_processor the baseModelProcessor to be copied
					@return      BaseModelProcessor& constant reference of {\em *this} baseModelProcessor
					@see         set
			*/
			const BaseModelProcessor& operator =
				(const BaseModelProcessor& base_model_processor)
				throw();

			/** Copying.
					Copy {\em *this} baseModelProcessor to the baseModelProcessor {\em connector}.
					Initializes the \Ref{ExtendedPropertyManager}, \Ref{BaseModelConnector}, 
					\Ref{FindGeometricObject} and the \Ref{ColorCalculator} of {\em *this} baseModelProcessor
					to those of {\em base_model_processor}.
					Calls \Ref{set}.
					@param       base_model_processor the baseModelProcessor to be assigned to
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em base_model_processor}
					@see         set
			*/
			void get
				(BaseModelProcessor& base_model_processor, bool deep = true) const
				throw();

			/** Swapping of baseModelProcessor's.
					Swap the \Ref{ExtendedPropertyManager}, \Ref{BaseModelConnector}, 
					\Ref{FindGeometricObject} and the \Ref{ColorCalculator} of {\em *this} baseModelProcessor
					with those of {\em base_model_processor}.
					Calls \Ref{ExtendedPropertyManager::swap}
					Calls \Ref{FindGeometricObject::swap}
					@param       connector the baseModelConnector being swapped with {\em *this} baseModelProcessor 
					@see         BaseModelConnector
					@see         ExtendedPropertyManager
					@see         FindGeometricObject
					@see         ColorCalculator
			*/
			void swap
				(BaseModelProcessor& base_model_processor)
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			
			/** Enable creation of static models.
					If this method is called {\em *this} baseModelProcessor will create models with
					the property {\tt PROPERTY__OBJECT_STATIC}.
					See \Ref{GeometricObject} for information about static models.
					The method \Ref{disableDynamicModel} will be do exactly the same as this method.
					@see  disableStaticModel
					@see  disableDynamicModel
					@see  isStaticModelEnabled
					@see  GeometricObject
			*/
			void enableStaticModel()
				throw();

			/** Disable creation of static models.
					If this method is called {\em *this} baseModelProcessor will create models with
					the property {\tt PROPERTY__OBJECT_DYNAMIC}.
					See \Ref{GeometricObject} for information about dynamic models.
					The method \Ref{enableDynamicModel} will be do exactly the same as this method.
					@see  enableStaticModel
					@see  enableDynamicModel
					@see  isStaticModelDisabled
					@see  GeometricObject
			*/
			void disableStaticModel()
				throw();

			/** Enable creation of dynamic models.
					If this method is called {\em *this} baseModelProcessor will create models with
					the property {\tt PROPERTY__OBJECT_DYNAMIC}.
					See \Ref{GeometricObject} for information about dynamic models.
					The method \Ref{disableStaticModel} will be do exactly the same as this method.
					@see  disableDynamicModel
					@see  disableStaticModel
					@see  isDynamicModelEnabled
					@see  GeometricObject
			*/
			void enableDynamicModel()
				throw();

			/** Disable creation of dynamic models.
					If this method is called {\em *this} baseModelProcessor will create models with
					the property {\tt PROPERTY__OBJECT_STATIC}.
					See \Ref{GeometricObject} for information about static models.
					The method \Ref{enableStaticModel} will be do exactly the same as this method.
					@see  enableDynamicModel
					@see  enableStaticModel
					@see  isDynamicModelDisabled
					@see  GeometricObject
			*/
			void disableDynamicModel()
				throw();

			/** Register a colorCalculator.
					Change the \Ref{ColorCalculator} of {\em *this} baseModelProcessor.
					@param   color_calculator the new \Ref{ColorCalculator} for {\em *this} baseModelProcessor
					@see     unregisterColorCalculator
					@see     ColorCalculator
			*/
			void registerColorCalculator
				(const ColorCalculator& color_calculator)
				throw();

			/** Unregister the colorCalculator.
					Change the \Ref{ColorCalculator} of {\em *this} baseModelProcessor to the
					default colorCalculator.
					@see     registerColorCalculator
					@see     ColorCalculator
			*/
			void unregisterColorCalculator()
				throw();

			/** Mutable inspection of the colorCalculator.
					Access the mutable pointer to the \Ref{ColorCalculator} of {\em *this} baseModelProcessor.
					@return  ColorCalculator* a pointer to the colorCalculator of {\em *this} baseModelProcessor
					@see     registerColorCalculator
					@see     unregisterColorCalculator
					@see     ColorCalculator
			*/
			ColorCalculator* getColorCalculator() const
				throw();

			/** Register a baseModelConnector.
					Change the \Ref{BaseModelConnector} of {\em *this} baseModelProcessor.
					@param    model_connector the new \Ref{BaseModelConnector} for {\em *this} baseModelProcessor
					@see     unregisterBaseModelConnector
					@see     BaseModelConnector
			*/
			void registerModelConnector
				(const BaseModelConnector& model_connector)
				throw();

			/** Unregister the baseModelConnector.
					Change the \Ref{BaseModelConnector} of {\em *this} baseModelProcessor to the
					default baseModelConnector.
					@see     registerBaseModelConnector
					@see     BaseModelConnector
			*/
			void unregisterModelConnector()
				throw();

			/** Mutable inspection of the baseModelConnector.
					Access the mutable pointer to the \Ref{BaseModelConnector} of {\em *this} baseModelProcessor.
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
					Set the properties of \Ref{FindGeometricObjects} search processor and the properties
					of \Ref{BaseModelConnector} to the properties of {\em *this} baseModelProcessor
					and registers the \Ref{ColorCalculator} of {\em *this} baseModelProcessor to
					\Ref{BaseModelConnector}.
					@return bool {\tt true} if the start of {\em *this} baseModelProcessor was successful, {\tt false} otherwise
					@see    operator()
					@see    BaseModelConnector
					@see    FindGeometricObjects
			*/
			virtual bool start()
				throw();
			
			/** Finish method.
					Empty for further purpose. Override this method for finalizing
					the models.
					@return bool {\tt true} if the finish of {\em *this} baseModelProcessor was successful, {\tt false} otherwise
					@see    operator()
			*/
			virtual bool finish()
				throw();

			/**	Operator method.
					This method will be overridden to implement the creation of the different models.
					@param  composite the \Ref{Composite} object that will be processed
					@return Processor::Result the result of {\em *this} baseModel
					@see    start
					@see    finish
					@see    Composite
			*/
			virtual Processor::Result operator() (Composite& composite)
				throw();

			//@}
				
			/**	@name	Predicates
			*/
			//@{

			/** Test if creation of static models is enabled.
					Test if the property {\tt PROPERTY__OBJECT_STATIC} is set for {\em *this} baseModelProcessor.
					See \Ref{GeometricObject} for information about this property.
					@return  bool {\tt true} if static models of {\em *this} baseModelProcessor should be created, {\tt false} otherwise
					@see     enableStaticModel
					@see     disableStaticModel
					@see     GeometricObject
					@see     ExtendedPropertyManager
			*/
			bool isStaticModelEnabled()
				throw();

			/** Test if creation of static models is disabled.
					Test if the property {\tt PROPERTY__OBJECT_STATIC} is not set for {\em *this} baseModelProcessor.
					See \Ref{GeometricObject} for information about this property.
					@return  bool {\tt true} if static models of {\em *this} baseModelProcessor should not be created, {\tt false} otherwise
					@see     enableStaticModel
					@see     disableStaticModel
					@see     GeometricObject
					@see     ExtendedPropertyManager
			*/
			bool isStaticModelDisabled()
				throw();

			/** Test if creation of dynamic models is enabled.
					Test if the property {\tt PROPERTY__OBJECT_DYNAMIC} is set for {\em *this} baseModelProcessor.
					See \Ref{GeometricObject} for information about this property.
					@return  bool {\tt true} if dynamic models of {\em *this} baseModelProcessor should be created, {\tt false} otherwise
					@see     enableDynamicModel
					@see     disableDynamicModel
					@see     GeometricObject
					@see     ExtendedPropertyManager
			*/
			bool isDynamicModelEnabled()
				throw();

			/** Test if creation of dynamic models is disabled.
					Test if the property {\tt PROPERTY__OBJECT_DYNAMIC} is not set for {\em *this} baseModelProcessor.
					See \Ref{GeometricObject} for information about this property.
					@return  bool {\tt true} if dynamic models of {\em *this} baseModelProcessor should not be created, {\tt false} otherwise
					@see     enableDynamicModel
					@see     disableDynamicModel
					@see     GeometricObject
					@see     ExtendedPropertyManager
			*/
			bool isDynamicModelDisabled()
				throw();

			/** Test if the default colorCalculator is registered.
					Test if the default \Ref{ColorCalculator} is registered for {\em *this} 
					atomBondModelConnector.
					@return  bool {\tt true} if the default \Ref{ColorCalculator} is registered, {\tt false} otherwise
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
					(self-validated) and consistent {\tt true} is returned, {\tt false} otherwise. 
					{\em *this} atomBondModelConnector is valid if:
					\Ref{ExentedPropertyManager}, \Ref{BaseModelConnector} and \Ref{FindGeometricObjects}
					is valid.
					Calls \Ref{ExentedPropertyManager::isValid}.
					Calls \Ref{BaseModelConnector::isValid}.
					Calls \Ref{FindGeometricObjects::isValid}.
					@return			bool {\tt true} if the internal state of {\em *this}atomBondModelConnector is correct (self-validated) and consistent, {\tt false} otherwise
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
			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			
			protected:

			/** @name Protected members
			*/
			//@{
			/** Mutable inspection of the search processor.
					Access the mutable reference to \Ref{FindGeometricObjects} of {\em *this}
					atomBondModelConnector.
					This object is used to search for specific \Ref{GeometricObject} objects that
					have certain properties.
					@return  FindGeometricObjects& a mutable reference to the \Ref{FindGeometricObjects} of {\em *this} atomBondModelConnector
					@see     FindGeometricObjects
			*/
			FindGeometricObjects& getSearcher_()
				throw();

			/** Test if a geometricObject is a model.
					Test if \Ref{GeometricObject} {\em geometric_object} has a model property.
					Return {\tt true} if a model property is set in {\em geometric_object}.
					This method is called from the method \Ref{removeGeometricObjects_}.
					The following models are recognized:
					\begin{itemize}
					  \item {\em PROPERTY__MODEL_BALL_AND_STICK}
					  \item {\em PROPERTY__MODEL_DOTS}
					  \item {\em PROPERTY__MODEL_LINES}
					  \item {\em PROPERTY__MODEL_VDW}
					\end{itemize}
					@param  geometric_object the \Ref{GeometricObject} to be tested if a model property is present
					@return bool {\tt true} if a model property is set, {\tt false} otherwise
					@see    removeGeometricObjects_
					@see    GeometricObject
			*/
			bool isModel_(GeometricObject& geometric_object)
				throw();

			/** Remove geometric objects.
					Remove \Ref{GeometricObject} objects appended to \Ref{Composite} {\em composite}.
					If {\em only_models} is set to {\tt true} (Default) only \Ref{GeometricObject}
					objects are removed that have a model property (determined with the method \Ref{isModel_}).
					All found \Ref{GeometricObject} objects are deleted.
					@param  composite the \Ref{Composite} object that should be search for \Ref{GeometricObject} objects
					@param  only_models if set to {\tt true} only \Ref{GeometricObject} objects are searched that have a model property
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
					  \item {\tt PROPERTY__OBJECT_STATIC}
					  \item {\tt PROPERTY__OBJECT_OPAQUE}
					  \item {\tt PROPERTY__OBJECT_VISIBLE}
					  \item {\tt PROPERTY__OBJECT_CLOSED}
					  \item {\tt PROPERTY__DRAWING_MODE_SOLID}
					  \item {\tt PROPERTY__DRAWING_PRECISION_HIGH}
					\end{itemize}
					Those properties are defined in the class \Ref{GeometricObject}.
					@see   GeometricObject
			*/
			void clear_()
				throw();
			//@}

			
			private:

			void dump_
				(std::ostream& s, Size depth) const;


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

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_BASEMODEL_H 
