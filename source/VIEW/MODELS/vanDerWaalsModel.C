// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vanDerWaalsModel.C,v 1.3 2003/09/03 12:34:32 amoll Exp $
//

#include <BALL/VIEW/MODELS/vanDerWaalsModel.h>
#include <BALL/VIEW/MODELS/colorProcessor.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/KERNEL/atom.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		AddVanDerWaalsModel::AddVanDerWaalsModel()
			throw()
			: AtomBondModelBaseProcessor()
		{
 		}

		AddVanDerWaalsModel::AddVanDerWaalsModel(const AddVanDerWaalsModel& model)
			throw()
			: AtomBondModelBaseProcessor(model)
		{
		}

		AddVanDerWaalsModel::~AddVanDerWaalsModel()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
						 				<< " of class " << RTTI::getName<AddVanDerWaalsModel>() << std::endl;
			#endif 
		}

		void AddVanDerWaalsModel::clear()
			throw()
		{
			AtomBondModelBaseProcessor::clear();
		}

		bool AddVanDerWaalsModel::start()
		{
			return AtomBondModelBaseProcessor::start();
		}
				
		bool AddVanDerWaalsModel::finish()
		{
//			buildBondModels_(); // ????? kann man vielleicht weglassen?

			return true;
		}
				
		Processor::Result AddVanDerWaalsModel::operator() (Composite &composite)
		{
			// composite is an atom ?
			if (!RTTI::isKindOf<Atom>(composite))
			{
				return Processor::CONTINUE;
			}

			Atom* atom = RTTI::castTo<Atom>(composite);

			Sphere* sphere_ptr = new Sphere;

			if (sphere_ptr == 0) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Sphere));

			sphere_ptr->setComposite(atom);

			sphere_ptr->setRadius(atom->getElement().getVanDerWaalsRadius());
			sphere_ptr->setPositionAddress(atom->getPosition());

			getColorProcessor()->operator() (atom);
			sphere_ptr->setColor(getColorProcessor()->getColor());
			
			geometric_objects_.push_back(sphere_ptr);
			
			insertAtom_(atom);
			
			return Processor::CONTINUE;
		}

		void AddVanDerWaalsModel::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			AtomBondModelBaseProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

	} // namespace VIEW
} // namespace BALL
