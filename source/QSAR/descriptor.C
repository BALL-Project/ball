// $Id: descriptor.C,v 1.3 2001/12/17 02:50:03 oliver Exp $

#include <BALL/QSAR/descriptor.C>

namespace BALL
{

	Descriptor::Descriptor()
		:	UnaryProcessor<Molecule>(),
			name_("generic descriptor")
	{
	}

	Descriptor::Descriptor(const String& name)
		:	UnaryProcessor<Molecule>(),
			name_(name)
	{
	}

	const String& Descriptor::getName() const
	{
		return name_;
	}

	void Descriptor::setName(const String& name) const
	{
		name_ = name;
	}

	Processor::Result Descriptor::operator () (Molecule& molecule)
	{
		molecule.setProperty(getName(), compute(molecule));
		return Processor::CONTINUE;
	}

	double Descriptor::compute(const Molecule& molecule)
	{
		return 0.0;
	}
}
