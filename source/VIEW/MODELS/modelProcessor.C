// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modelProcessor.C,v 1.7 2003/12/15 01:24:19 amoll Exp $

#include <BALL/VIEW/MODELS/modelProcessor.h>
#include <BALL/VIEW/KERNEL/common.h>

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
		surface_drawing_precision_(-1)
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
	surface_drawing_precision_ = -1;
}

} }
