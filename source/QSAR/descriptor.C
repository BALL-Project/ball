// $Id: descriptor.C,v 1.7 2002/01/03 02:31:02 oliver Exp $

#include <BALL/QSAR/descriptor.h>

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

	Descriptor::~Descriptor()
	{
	}

	Descriptor::Descriptor(const Descriptor& descriptor)
		:	UnaryProcessor<Molecule>(descriptor),
			name_(descriptor.name_)
	{
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

	double Descriptor::compute(const Molecule& /* molecule */)
	{
		return 0.0;
	}
}
