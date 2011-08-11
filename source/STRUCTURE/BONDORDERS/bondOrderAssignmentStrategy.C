#include <BALL/STRUCTURE/BONDORDERS/bondOrderAssignmentStrategy.h>

namespace BALL
{
	BondOrderAssignmentStrategy::BondOrderAssignmentStrategy(AssignBondOrderProcessor* parent)
		: abop(parent)
	{
	}

	void BondOrderAssignmentStrategy::readOptions(const Options& /*options*/)
	{
	}

	void BondOrderAssignmentStrategy::setDefaultOptions()
	{
	}

	void BondOrderAssignmentStrategy::clear()
	{
	}
}
