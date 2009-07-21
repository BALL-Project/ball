// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modelProcessor.h,v 1.19.20.1 2007/03/28 13:51:47 amoll Exp $
//

#ifndef BALL_VIEW_MODELS_MODELPROCESSOR_H
#define BALL_VIEW_MODELS_MODELPROCESSOR_H

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_CONCEPT_PROPERTY_H
# include <BALL/CONCEPT/property.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
# include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** Base class for all model processors.
		 		Every model processor has a list with all GeometricObject 's it has
				created. It also stores the drawing precision, it will use for creating new models.
				You can apply model processors like every other UnaryProcessor for Composite 's: <br>
				\verbatim				
					Molecule m;
					ModelProcessor model_processor;
					m.apply(model_processor);
				\endverbatim
				\ingroup  ViewModels
		*/
		class BALL_VIEW_EXPORT ModelProcessor
			: public UnaryProcessor<Composite>,
				public PropertyManager
		{
			public:

				BALL_CREATE(ModelProcessor)
				
				///
				ModelProcessor();

				///
				ModelProcessor(const ModelProcessor& model_processor);

				/** Destructor, calls clear
				*/
				virtual ~ModelProcessor();

				/// Clear all datastructures, but dont destroy the geometric objects.
				virtual void clear();

				///
				virtual bool isValid() const
					{return true;};

 				/** Needed in derived classes, which store references to the Composite instances they worked on.
				 		Here only needed for a common interface and therefore empty.
				*/
				virtual void clearComposites()
					{};

				///
				virtual void dump(std::ostream& /*s = std::cout*/, Size /*depth = 0*/) const
					{};

				/// Return the list with the created geometric objects
				GeometricObjectList& getGeometricObjects() 
					{ return geometric_objects_;}
				
				/// Return the list with the created geometric objects
				const GeometricObjectList& getGeometricObjects() const
					{ return geometric_objects_;}
				
				///
				void setDrawingPrecision(Index precision);

				///
				Index getDrawingPrecision() const;

				///
				void setSurfaceDrawingPrecision(float precision);

				///
				float getSurfaceDrawingPrecision() const;

				/** Method to create geometric objects.
				 		This method is called in Representation::update() after
						all operator() was called for all Composites.
				 		To be overloaded in derived classes
				*/
				virtual bool createGeometricObjects() 
					{ return true;}
				
				protected:
				
				//_
				GeometricObjectList geometric_objects_;

				//_
				Index drawing_precision_;

				//_
				float surface_drawing_precision_;
	};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_MODELS_MODELPROCESSOR_H
