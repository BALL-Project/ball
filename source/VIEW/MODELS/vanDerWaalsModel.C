// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vanDerWaalsModel.C,v 1.10 2005/12/23 17:03:35 amoll Exp $
//

#include <BALL/VIEW/MODELS/vanDerWaalsModel.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		AddVanDerWaalsModel::AddVanDerWaalsModel()
			: AtomBondModelBaseProcessor(),
				radius_factor_(1)
		{
 		}

		AddVanDerWaalsModel::AddVanDerWaalsModel(const AddVanDerWaalsModel& model)
			: AtomBondModelBaseProcessor(model),
				radius_factor_(model.radius_factor_)
		{
		}

		AddVanDerWaalsModel::~AddVanDerWaalsModel()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
						 				<< " of class " << RTTI::getName<AddVanDerWaalsModel>() << std::endl;
			#endif 
		}

		Processor::Result AddVanDerWaalsModel::operator() (Composite &composite)
		{
			if (!RTTI::isKindOf<Atom>(composite))
			{
				return Processor::CONTINUE;
			}

			Atom* atom = RTTI::castTo<Atom>(composite);

			Sphere* sphere_ptr = new Sphere;
			if (sphere_ptr == 0) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Sphere));
			sphere_ptr->setComposite(atom);
			if (atom->getElement().getVanDerWaalsRadius() != 0.0)
			{
				sphere_ptr->setRadius(atom->getElement().getVanDerWaalsRadius() * radius_factor_);
			}
			else
			{
				sphere_ptr->setRadius(radius_factor_);
			}
			sphere_ptr->setPositionAddress(atom->getPosition());
			geometric_objects_.push_back(sphere_ptr);
			
			insertAtom_(atom);
			
			return Processor::CONTINUE;
		}

		void AddVanDerWaalsModel::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			AtomBondModelBaseProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

	} // namespace VIEW
} // namespace BALL
