// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HBondModel.C,v 1.12 2005/12/23 17:03:33 amoll Exp $
//

#include <BALL/VIEW/MODELS/HBondModel.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		HBondModelProcessor::HBondModelProcessor()
			: AtomBondModelBaseProcessor(),
				radius_((float)0.3)
		{
		}

		HBondModelProcessor::HBondModelProcessor(const HBondModelProcessor& model)
			: AtomBondModelBaseProcessor(model),
				radius_(model.radius_)
		{
		}

		HBondModelProcessor::~HBondModelProcessor()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class HBondModelProcessor" << endl;
			#endif 
		}

		void HBondModelProcessor::clear()
		{
			AtomBondModelBaseProcessor::clear();
			radius_ = (float) 0.3;
		}

		void HBondModelProcessor::set(const HBondModelProcessor& model)
		{
			AtomBondModelBaseProcessor::set(model);
			radius_ = model.radius_;
		}

		const HBondModelProcessor &HBondModelProcessor::operator = (const HBondModelProcessor& model)
		{
			set(model);
			return *this;
		}
				
		Processor::Result HBondModelProcessor::operator() (Composite& composite)
		{
			if (!RTTI::isKindOf<Atom>(composite))
			{
				return Processor::CONTINUE;
			}

			Atom *atom = RTTI::castTo<Atom>(composite); 

			Atom::BondIterator bit = atom->beginBond();
			for (; +bit; ++bit)
			{
				// Consider each bond just once (first atom!) and only those which are real H bonds.
				if ((bit->getFirstAtom() == atom) && (bit->getType() == Bond::TYPE__HYDROGEN))
				{
					const Atom* partner = bit->getPartner(*atom);

					// generate tubes
					const Vector3 v = partner->getPosition() - atom->getPosition();
					Vector3 last = atom->getPosition() + v / 4.5;
					for (Position p = 0; p < 3; p++)
					{
						Tube *tube = new Tube;
						if (tube == 0) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Tube));
											
						tube->setRadius(radius_);
						tube->setComposite(atom);
						tube->setVertex1(last);
						tube->setVertex2(last + (v / 8.));
						geometric_objects_.push_back(tube);

						Disc* disc = new Disc(Circle3(last, -v, radius_));
						if (!disc) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Disc));
						disc->setComposite(atom);
						geometric_objects_.push_back(disc);

						disc = new Disc(Circle3(last + (v / 8), v, radius_));
						if (!disc) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Disc));
						disc->setComposite(atom);
						geometric_objects_.push_back(disc);

						last += (v /4.);
					}
				}
			}

			return Processor::CONTINUE;
		}

	} // namespace VIEW

} // namespace BALL
