// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: baseModelConnector.h,v 1.2 2003/08/26 18:35:33 amoll Exp $
//

#ifndef BALL_VIEW_MODELS_BASEMODELCONNECTOR_H
#define BALL_VIEW_MODELS_BASEMODELCONNECTOR_H

#ifndef BALL_CONCEPT_VISITOR_H
#	include <BALL/CONCEPT/visitor.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_CONCEPT_PROPERTY_H
#	include <BALL/CONCEPT/property.h>
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
		class ColorProcessor;
	}

	namespace VIEW
	{
		using VIEW::GeometricObject;
		using VIEW::ColorProcessor;

		/** BaseModelConnector class.
				The class BaseModelConnector is derived from Visitor and
				PropertyManager. It contains therefore the visitor interface
				and the property concept.
				This class is a base class for all model connector classes (e.g. AtomBondModelConnector).
				A model connector class is used by the model classes to create a connection
				between different types of models.
				If a new model is introduced the model connector class must be extended to deal with this
				new model and its connections to the other models.
				\ingroup  MolviewModels
		*/
		class BaseModelConnector
			: public Visitor<Composite>,
			  public PropertyManager
		{
			public:
			
			/**	@name	Constructors and Destructor
			*/	
			//@{

			/** Default Constructor.
			*/
			BaseModelConnector()
				throw();

			/** Copy constructor.
			*/
			BaseModelConnector(const BaseModelConnector& connector)
				throw();

			/** Destructor.
			*/
			virtual ~BaseModelConnector()
				throw();

			/** Explicit default initialization.
					Calls PropertyManager::clear.
			*/
			virtual void clear()
				throw() {PropertyManager::clear();}

			//@} 
			/**	@name	Assignment 
			*/ 
			//@{

			/** Assignment.
					Calls PropertyManager::set.
			*/
			void set(const BaseModelConnector& connector)
				throw() {PropertyManager::set(connector);}

			/** Assignment operator.
					Calls set().
			*/
			const BaseModelConnector& operator = (const BaseModelConnector& connector)
				throw();

			/** Swapping of baseModelConnector.
					Calls PropertyManager::swap
			*/
			void swap(BaseModelConnector& connector)
				throw() {PropertyManager::swap(connector);}

			//@} 
			/**	@name	Accessors: inspectors and mutators 
			*/ 
			//@{

			///
			void setGeometricObjectsList(List<GeometricObject*>* list_ptr)
				throw() { geometric_objects_ = list_ptr;}
			
			///
			void setColorProcessor(ColorProcessor* processor)
				throw() { color_processor_ = processor;}

			///
			ColorProcessor* getColorProcessor()
				throw() { return color_processor_;}

			/** Change the properties.
					Change the properties of this instance by copying the properties from a PropertyManager.
					Calls PropertyManager::set.
					This method is provided for convenience.
					\param  manager the PropertyManager to be copied 
			*/
			void setProperties(PropertyManager &manager)
				throw() {PropertyManager::set(manager);}

			/** Visit method.
					This method will be overridden from the derived classes to implement the
					creation of the connector models between the different models.
			*/
			virtual void visit(Composite& /*composite*/) {}

			//@} 
			/**	@name	debuggers and diagnostics 
			*/ 
			//@{

			/** Dump the current state of to the output ostream with a given dumping depth.
					\param   s output stream where to output the state 
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			//@}

			protected:

			List<GeometricObject*>* geometric_objects_;

			ColorProcessor* 				color_processor_;

		};

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_MODELS_BASEMODELCONNECTOR_H
