// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: forceModel.C,v 1.6 2004/09/01 14:14:04 amoll Exp $

#include <BALL/VIEW/MODELS/forceModel.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/VIEW/PRIMITIVES/line.h>
#include <cmath>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		ForceModel::ForceModel()
			throw()
			: ModelProcessor(),
				scaling_(11),
				max_length_(10)
		{
		}

		ForceModel::ForceModel(const ForceModel& model)
			throw()
			: ModelProcessor(model),
				scaling_(model.scaling_),
				max_length_(model.max_length_)
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
			float forcev = log(force.getLength() * pow((float)10.0, scaling_)); 

			if (forcev < 0) return Processor::CONTINUE;
			if (forcev > max_length_)
			{
				forcev = max_length_;
			}	

			// prevent problems in normalize
			force *= 10000000000.0;
			force.normalize();
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
