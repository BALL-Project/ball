// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modelProcessor.C,v 1.13 2005/12/23 17:03:34 amoll Exp $
//

#include <BALL/VIEW/MODELS/modelProcessor.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/geometricObject.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		ModelProcessor::ModelProcessor()
			: UnaryProcessor<Composite>(),
				PropertyManager(),
				drawing_precision_(DRAWING_PRECISION_HIGH),
				surface_drawing_precision_(-1)
		{
		}

		ModelProcessor::ModelProcessor(const ModelProcessor& model_processor)
			: UnaryProcessor<Composite>(model_processor),
				PropertyManager(model_processor),
				drawing_precision_(model_processor.drawing_precision_),
				surface_drawing_precision_(model_processor.surface_drawing_precision_)
		{
		}

		ModelProcessor::~ModelProcessor()
		{
			clear();
		}

		void ModelProcessor::clear()
		{
			PropertyManager::clear();
			for(std::list<GeometricObject*>::iterator it = geometric_objects_.begin(); it != geometric_objects_.end(); ++it)
			{
				delete *it;
			}
			geometric_objects_.clear();
			
			drawing_precision_ = DRAWING_PRECISION_HIGH;
			surface_drawing_precision_ = -1;
			clearComposites();
		}
		
		void ModelProcessor::setDrawingPrecision(Index precision)
		{ 
			drawing_precision_ = precision;
		}

		Index ModelProcessor::getDrawingPrecision() const
		{ 
			return drawing_precision_;
		}

		void ModelProcessor::setSurfaceDrawingPrecision(float precision)
		{ 
			surface_drawing_precision_ = precision;
		}

		float ModelProcessor::getSurfaceDrawingPrecision() const
		{ 
			return surface_drawing_precision_;
		}


	} // namespace VIEW
} // namespace BALL
