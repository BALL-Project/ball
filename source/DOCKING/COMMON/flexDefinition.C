// ----------------------------------------------------
// $Maintainer: Marc Röttig $
// $Authors: Marc Röttig $
// ----------------------------------------------------

#include <BALL/DOCKING/COMMON/flexDefinition.h>

namespace BALL
{
	FlexDefinition FlexDefinition::Rigid()
	{
		return FlexDefinition();
	}

	FlexDefinition::FlexDefinition()
	{
	}

	FlexDefinition::~FlexDefinition()
	{
	}

	void FlexDefinition::addFullyFlexibleResidue(Position idx)
	{
		fully_flexible_.push_back(idx);
	}

	vector<Position> FlexDefinition::getFullyFlexibleResidues() const
	{
		return fully_flexible_;
	}

	void FlexDefinition::addRotamerFlexibleResidue(Position idx)
	{
		rotamer_flexible_.push_back(idx);
	}

	vector<Position> FlexDefinition::getRotamerFlexibleResidues() const
	{
		return rotamer_flexible_;
	}
}
