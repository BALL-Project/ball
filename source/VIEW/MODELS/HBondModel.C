// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HBondModel.C,v 1.7 2004/06/07 10:16:58 amoll Exp $

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
	throw()
	: AtomBondModelBaseProcessor(),
		radius_(0.3)
{
}

HBondModelProcessor::HBondModelProcessor(const HBondModelProcessor& model)
	throw()
	: AtomBondModelBaseProcessor(model),
		radius_(model.radius_)
{
}

HBondModelProcessor::~HBondModelProcessor()
	throw()
{
	#ifdef BALL_VIEW_DEBUG
		Log.info() << "Destructing object " << (void *)this 
							 << " of class " << RTTI::getName<HBondModelProcessor>() << std::endl;
	#endif 
}

void HBondModelProcessor::clear()
	throw()
{
	AtomBondModelBaseProcessor::clear();
	radius_ = 0.3;
}

void HBondModelProcessor::set(const HBondModelProcessor& model)
	throw()
{
	AtomBondModelBaseProcessor::set(model);
}

const HBondModelProcessor &HBondModelProcessor::operator = (const HBondModelProcessor& model)
	throw()
{
	set(model);
	return *this;
}

void HBondModelProcessor::swap(HBondModelProcessor& model)
	throw()
{
	AtomBondModelBaseProcessor::swap(model);
}

		
Processor::Result HBondModelProcessor::operator() (Composite& composite)
{
	if (!RTTI::isKindOf<Atom>(composite))
	{
		return Processor::CONTINUE;
	}

	Atom *atom = RTTI::castTo<Atom>(composite); 
	Bond* bond = 0;

	for (Position p = 0; p < atom->countBonds(); p++)
	{
		if (atom->getBond(p)->getType() == Bond::TYPE__HYDROGEN)
		{
			bond = atom->getBond(p);
			break;
		}
	}

	if (bond == 0) return Processor::CONTINUE;

	Atom* partner = bond->getPartner(*atom);

	// only one visualisation for a bond!
	if (partner == 0  ||
			partner < atom )
	{
		return Processor::CONTINUE;
	}

	// generate tubes
	Vector3 v = partner->getPosition() - atom->getPosition();
	Vector3 last = atom->getPosition() + v / 4.5;
	for (Position p = 0; p < 3; p++)
	{
		Tube *tube = new Tube;
		if (tube == 0) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Tube));
							
		tube->setRadius(radius_);
		tube->setComposite(atom);
		tube->setVertex1(last);
		tube->setVertex2(last + (v / 8));
		geometric_objects_.push_back(tube);

		Disc* disc = new Disc(Circle3(last, v, radius_));
		if (!disc) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Disc));
		disc->setComposite(atom);
		geometric_objects_.push_back(disc);

		disc = new Disc(Circle3(last + (v / 8), v, radius_));
		if (!disc) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Disc));
		disc->setComposite(atom);
		geometric_objects_.push_back(disc);

		last += (v /4);
	}

	return Processor::CONTINUE;
}

} } // namespaces
