// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: descriptor.C,v 1.11 2004/05/11 07:28:20 oliver Exp $
//

#include <BALL/QSAR/descriptor.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/expression.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/CONCEPT/timeStamp.h>
#include <BALL/STRUCTURE/numericalSAS.h>

#include <iostream>
#include <cmath>
#include <vector>
#include <queue>

using namespace std;

namespace BALL
{

	Descriptor::Descriptor()
		:	UnaryProcessor<Molecule>(),
			name_("generic descriptor"),
			unit_("")
	{
	}

	Descriptor::Descriptor(const String& name)
		:	UnaryProcessor<Molecule>(),
			name_(name),
			unit_("")
	{
	}

	Descriptor::Descriptor(const String& name, const String& unit)
		:	UnaryProcessor<Molecule>(),
			name_(name),
			unit_(unit)
	{
	}

	Descriptor::~Descriptor()
	{
	}

	Descriptor::Descriptor(const Descriptor& descriptor)
		:	UnaryProcessor<Molecule>(descriptor),
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

	Processor::Result Descriptor::operator () (Molecule& molecule)
	{
		molecule.setProperty(getName(), compute(molecule));
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

	bool Descriptor::isValid(Molecule& /* molecule */)
	{
		return false;
	}

	void Descriptor::calculate(Molecule& /* molecule */)
	{		
	}

	double Descriptor::compute(Molecule& molecule)
	{
		if (!isValid(molecule))
		{
			calculate(molecule);
		}
		return molecule.getProperty(this->getName()).getDouble();
	}

}
