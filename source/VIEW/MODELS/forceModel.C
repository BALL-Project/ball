// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: forceModel.C,v 1.10 2005/02/23 13:01:49 amoll Exp $
//

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
				Log.error() << "Destructing object " << this << " of class ForceModel" << std::endl;
			#endif 
		}

		Processor::Result ForceModel::operator() (Composite &composite)
		{
			Atom* atom = dynamic_cast<Atom*>(&composite);
			if (atom == 0) return Processor::CONTINUE;

			Vector3 force = atom->getForce() * pow((float)10.0, 12);
			if (Maths::isZero(force.getSquareLength())) return Processor::CONTINUE;
			float forcev = log(force.getLength()) * scaling_; 

			if (forcev < 0) return Processor::CONTINUE;

			if (forcev > max_length_) forcev = max_length_;

			// prevent problems in normalize
			force *= 10000000000.0;
			force.normalize();
			force *= forcev;

			Line* line = new Line();
			line->setVertex1Address(atom->getPosition());
			line->setVertex2(atom->getPosition() + force);
			line->setComposite(atom);
			geometric_objects_.push_back(line);

			return Processor::CONTINUE;
		}

	} // namespace VIEW
} // namespace BALL
