#include <BALL/STRUCTURE/BONDORDERS/bondOrderAssignmentStrategy.h>
#include <BALL/KERNEL/atomHandle.h>      // v2.2 H3d.D: D-H3.8 opt-in

namespace BALL
{
	BondOrderAssignmentStrategy::BondOrderAssignmentStrategy(AssignBondOrderProcessor* parent)
		: abop(parent)
	{
	}

	bool BondOrderAssignmentStrategy::readOptions(const Options& /*options*/)
	{
		return true;
	}

	void BondOrderAssignmentStrategy::setDefaultOptions()
	{
	}

	void BondOrderAssignmentStrategy::clear()
	{
	}
}
