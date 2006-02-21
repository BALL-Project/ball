// $Id: scoringTypes.C,v 1.1 2006/02/21 16:10:47 anker Exp $
//

#include <BALL/SCORING/COMMON/scoringTypes.h>

namespace BALL
{

	ScoringTypes::ScoringTypes()
		throw()
		:	scoring_component_(0),
			type_map_()
	{
	}

	ScoringTypes::ScoringTypes(const ScoringTypes& st)
		throw()
		:	scoring_component_(st.scoring_component_),
			type_map_(st.type_map_)
	{
	}

	ScoringTypes::ScoringTypes(const ScoringComponent& sc)
		throw()
		:	scoring_component_(&sc),
			type_map_()
	{
		setup();
	}

	ScoringTypes::~ScoringTypes()
		throw()
	{
		clear();
	}

	void ScoringTypes::clear()
		throw()
	{
		scoring_component_ = 0;
		type_map_.clear();
	}

	bool ScoringTypes::setup()
		throw()
	{
		// This is an interface definition and should be abstract. Always
		// return false.
		return(false);
	}

	const HashMap<const Atom*, Size>& ScoringTypes::getTypeMap() const
		throw()
	{
		return(type_map_);
	}


}

