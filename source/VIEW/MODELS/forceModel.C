// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: forceModel.C,v 1.1 2004/07/08 16:49:24 amoll Exp $

#include <BALL/VIEW/MODELS/forceModel.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/VIEW/PRIMITIVES/line.h>

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

//	 Vector3 force = atom->getForce();
	Vector3 force = atom->getVelocity();
	if (force.getSquareLength() == 0) return Processor::CONTINUE;
	force /= 10.0;

	Line* line = new Line();
	line->setVertex1Address(atom->getPosition());
	if (force.getLength() > 10)
	{
		force.normalize();
		force *= 10.0;
	}
	Vector3 pos = atom->getPosition() + force;
	line->setVertex2(pos);
	line->setComposite(atom);
	line->setColor(ColorRGBA(1.0,1.0,1.0,1.0));
	geometric_objects_.push_back(line);

	return Processor::CONTINUE;
}

	} // namespace VIEW
} // namespace BALL
