// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modelProcessor.C,v 1.5 2003/10/17 16:17:37 amoll Exp $

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
		drawing_precision_(DRAWING_PRECISION_HIGH)
{
}

ModelProcessor::ModelProcessor(const ModelProcessor& model_processor)
	throw()
	: UnaryProcessor<Composite>(model_processor),
		PropertyManager(model_processor),
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

} }
