// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modelProcessor.h,v 1.2 2003/08/26 18:35:34 amoll Exp $

#ifndef BALL_VIEW_GUI_MODELS_ModelProcessor_H
#define BALL_VIEW_GUI_MODELS_ModelProcessor_H

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
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
		class ColorProcessor;
		
		class ModelProcessor
			: public UnaryProcessor<Composite>,
				public PropertyManager
		{
			public:
				
				///
				typedef List<GeometricObject*> GeometricObjectList;

				///
				ModelProcessor()
					throw();

				///
				ModelProcessor(const ModelProcessor& model_processor)
					throw();

				///
				~ModelProcessor()
					throw();

				///
				virtual void clear()
					throw();

				///
				virtual bool start();

				///
				virtual bool finish();

				///
				virtual Processor::Result operator() (Composite& composite);

				///
				virtual bool isValid() const
					throw() {return true;};

				///
				virtual void dump(std::ostream& /*s = std::cout*/, Size /*depth = 0*/) const
					throw(){};

				///
				void setColorProcessor(ColorProcessor* processor)
					throw();

				///
				ColorProcessor* getColorProcessor()
					throw() { return color_processor_;}
				
				///
				GeometricObjectList& getGeometricObjects() 
					throw() { return geometric_objects_;}
				
				///
				const GeometricObjectList& getGeometricObjects() const
					throw() { return geometric_objects_;}
				
				///
				void setDrawingPrecision(Index precision)
					throw() { drawing_precision_ = precision;}

				///
				Index getDrawingPrecision() const
					throw() { return drawing_precision_;}
				
				protected:
				
				//_
				GeometricObjectList geometric_objects_;

				//_
				ColorProcessor* color_processor_;

				//_
				Index drawing_precision_;
	};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_GUI_MODELS_BASEMODEL_H 
