// ----------------------------------------------------
// $Maintainer: Marc Röttig $
// $Authors: Marc Röttig $
// ----------------------------------------------------

#include <BALL/DOCKING/COMMON/receptor.h>

namespace BALL
{
	Receptor::Receptor(Protein* p) : FlexibleMolecule(p)
	{
	}

	Receptor::~Receptor()
	{
	}

	FlexDefinition Receptor::getFlexDefinition(Position i)
	{
		if (i < flexinfo_.size())
			return this->flexinfo_[i];
		else
			throw(Exception::IndexOverflow(__FILE__, __LINE__, i, flexinfo_.size()));
	}

	void Receptor::setFlexDefinition(Position i, FlexDefinition& fd)
	{
		if (i < flexinfo_.size())
			flexinfo_[i] = fd;
		else
			throw(Exception::IndexOverflow(__FILE__, __LINE__, i, flexinfo_.size()));
	}

	vector<FlexDefinition> Receptor::getFlexDefinitions()
	{
		return this->flexinfo_;
	}

	void Receptor::addConformation(Conformation* conf)
	{
		FlexibleMolecule::addConformation(conf);
		flexinfo_.push_back(FlexDefinition::Rigid());
	}

	void Receptor::addFlexConformation(Conformation* conf, FlexDefinition &fd)
	{
		FlexibleMolecule::addConformation(conf);
		flexinfo_.push_back(fd);
	}
}
