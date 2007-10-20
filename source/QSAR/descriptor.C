// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: descriptor.C,v 1.12.20.3 2007/08/07 16:16:16 bertsch Exp $
//

#include <BALL/QSAR/descriptor.h>

using namespace std;

namespace BALL
{

	Descriptor::Descriptor()
		:	UnaryProcessor<AtomContainer>(),
			name_("generic descriptor"),
			unit_("")
	{
	}

	Descriptor::Descriptor(const String& name)
		:	UnaryProcessor<AtomContainer>(),
			name_(name),
			unit_("")
	{
	}

	Descriptor::Descriptor(const String& name, const String& unit)
		:	UnaryProcessor<AtomContainer>(),
			name_(name),
			unit_(unit)
	{
	}

	Descriptor::~Descriptor()
	{
	}

	Descriptor::Descriptor(const Descriptor& descriptor)
		:	UnaryProcessor<AtomContainer>(descriptor),
			name_(descriptor.name_),
			unit_(descriptor.unit_)
	{
	}

	Descriptor& Descriptor::operator = (const Descriptor& descriptor)
	{
		this->setUnit(descriptor.getUnit());
		this->setName(descriptor.getName());
		return *this;
	}

	const String& Descriptor::getName() const
	{
		return name_;
	}

	void Descriptor::setName(const String& name)
	{
		name_ = name;
	}

	Processor::Result Descriptor::operator () (AtomContainer& ac)
	{
		ac.setProperty(getName(), compute(ac));
		return Processor::CONTINUE;
	}

	const String& Descriptor::getUnit() const 
	{
		return unit_;
	}

	void Descriptor::setUnit(const String& unit)
	{
		unit_ = unit;
	}

	bool Descriptor::isValid_(AtomContainer& /* ac */)
	{
		return false;
	}

	void Descriptor::calculate_(AtomContainer& /* ac */)
	{		
	}

	double Descriptor::compute(AtomContainer& ac)
	{
		if (!isValid_(ac))
		{
			calculate_(ac);
		}
		return ac.getProperty(this->getName()).getDouble();
	}

}

