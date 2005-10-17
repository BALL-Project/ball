// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modelProcessor.C,v 1.12.2.1 2005/10/17 00:43:43 amoll Exp $
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
			throw()
			: UnaryProcessor<Composite>(),
				PropertyManager(),
				drawing_precision_(DRAWING_PRECISION_HIGH),
				surface_drawing_precision_(-1)
		{
		}

		ModelProcessor::ModelProcessor(const ModelProcessor& model_processor)
			throw()
			: UnaryProcessor<Composite>(model_processor),
				PropertyManager(model_processor),
				drawing_precision_(model_processor.drawing_precision_),
				surface_drawing_precision_(model_processor.surface_drawing_precision_)
		{
		}

		ModelProcessor::~ModelProcessor()
			throw()
		{
			clear();
		}

		void ModelProcessor::clear()
			throw()
		{
			PropertyManager::clear();
			
			GeometricObjectList::Iterator it = geometric_objects_.begin();
			for (; it != geometric_objects_.end(); it++)
			{
				delete *it;
			}
			geometric_objects_.clear();
			
			drawing_precision_ = DRAWING_PRECISION_HIGH;
			surface_drawing_precision_ = -1;
			clearComposites();
		}
		
		void ModelProcessor::setDrawingPrecision(Index precision)
			throw() 
		{ 
			drawing_precision_ = precision;
			surface_drawing_precision_ = -1;
		}

		Index ModelProcessor::getDrawingPrecision() const
			throw() 
		{ 
			return drawing_precision_;
		}

		void ModelProcessor::setSurfaceDrawingPrecision(float precision)
			throw() 
		{ 
			surface_drawing_precision_ = precision;
			drawing_precision_ = DRAWING_PRECISION_INVALID;
		}

		float ModelProcessor::getSurfaceDrawingPrecision() const
			throw() 
		{ 
			return surface_drawing_precision_;
		}


	} // namespace VIEW
} // namespace BALL
