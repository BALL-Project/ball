// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HBondModel.C,v 1.2 2003/09/03 15:10:39 amoll Exp $

#include <BALL/VIEW/MODELS/HBondModel.h>
#include <BALL/VIEW/MODELS/colorProcessor.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

HBondModelProcessor::HBondModelProcessor()
	throw()
	: AtomBondModelBaseProcessor()
{
}

HBondModelProcessor::HBondModelProcessor(const HBondModelProcessor& model)
	throw()
	: AtomBondModelBaseProcessor(model)
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

		
bool HBondModelProcessor::finish()
{
	return true;
}
		
Processor::Result HBondModelProcessor::operator() (Composite& composite)
{
	if (!RTTI::isKindOf<Atom>(composite) ||
			!((Atom*)&composite)->hasProperty("HBOND_DONOR"))
	{
		return Processor::CONTINUE;
	}

	Atom *atom = RTTI::castTo<Atom>(composite);
	Atom* partner = (Atom*) atom->getProperty("HBOND_DONOR").getObject();

	if (!partner) return Processor::CONTINUE;
	// generate single colored tube
	Tube *tube = new Tube;
	if (tube == 0) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Tube));
						
	tube->setRadius(0.3);
	tube->setComposite(atom);
	tube->setVertex1Address(atom->getPosition());
	tube->setVertex2Address(partner->getPosition());
	tube->setColor(ColorRGBA(0.0,0,0.5));
	geometric_objects_.push_back(tube);
	
	return Processor::CONTINUE;
}

} } // namespaces
