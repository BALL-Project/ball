// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modelProcessor.C,v 1.1 2003/08/26 12:38:38 oliver Exp $

#include <BALL/VIEW/GUI/FUNCTOR/modelProcessor.h>
#include <BALL/VIEW/GUI/FUNCTOR/colorProcessor.h>
#include <BALL/VIEW/COMMON/global.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		ModelProcessor::ModelProcessor()
			throw()
			: UnaryProcessor<Composite>(),
				PropertyManager(),
				color_processor_(0),
				drawing_precision_(DRAWING_PRECISION_HIGH)
		{
		}

		ModelProcessor::ModelProcessor(const ModelProcessor& model_processor)
			throw()
			: UnaryProcessor<Composite>(model_processor),
				PropertyManager(model_processor),
				color_processor_(new ColorProcessor(*model_processor.color_processor_)),
				drawing_precision_(model_processor.drawing_precision_)
		{}

		ModelProcessor::~ModelProcessor()
			throw()
		{
			clear();
		}

		void ModelProcessor::clear()
			throw()
		{
			PropertyManager::clear();
			geometric_objects_.clear();
			drawing_precision_ = DRAWING_PRECISION_HIGH;
			if (color_processor_ != 0) delete color_processor_;
			color_processor_ = 0;
		}
		
		bool ModelProcessor::start()
		{
			return true;
		}

		bool ModelProcessor::finish()
		{
			return true;
		}

		Processor::Result ModelProcessor::operator() (Composite& /*composite*/)
		{
			return Processor::CONTINUE;
		}


		void ModelProcessor::setColorProcessor(ColorProcessor* processor)
			throw() 
		{
			if (color_processor_ != 0) delete color_processor_;
			color_processor_ = processor;
		}

	}
}
