// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: forceModel.C,v 1.2 2004/07/09 16:14:26 amoll Exp $

#include <BALL/VIEW/MODELS/forceModel.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/VIEW/PRIMITIVES/line.h>
#include <math.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

ForceModel::ForceModel()
	throw()
	: ModelProcessor()
{
}

ForceModel::ForceModel(const ForceModel& model)
	throw()
	: ModelProcessor(model)
{
}

ForceModel::~ForceModel()
	throw()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing object " << (void *)this 
							  << " of class " << RTTI::getName<ForceModel>() << std::endl;
	#endif 
}

Processor::Result ForceModel::operator() (Composite &composite)
{
	if (!RTTI::isKindOf<Atom>(composite))
	{
		return Processor::CONTINUE;
	}
	Atom* atom = (Atom*) &composite;

	Vector3 force = atom->getForce();
	if (force.getSquareLength() == 0) return Processor::CONTINUE;
	force.normalize();
	force *= 10000000000.0;

	float forcev = std::log(force.getLength());
	if (forcev < 0) return Processor::CONTINUE;
	if (forcev > 10)
	{
		forcev = 10;
	}	
	force *= forcev;

	Line* line = new Line();
	line->setVertex1Address(atom->getPosition());
	line->setVertex2(atom->getPosition() + force);
	line->setComposite(atom);
	line->setColor(ColorRGBA(1.0,1.0,1.0,1.0));
	geometric_objects_.push_back(line);

	return Processor::CONTINUE;
}

	} // namespace VIEW
} // namespace BALL
