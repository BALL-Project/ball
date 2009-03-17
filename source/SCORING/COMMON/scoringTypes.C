// $Id: scoringTypes.C,v 1.1 2006/02/21 16:10:47 anker Exp $
//

#include <BALL/SCORING/COMMON/scoringTypes.h>

namespace BALL
{

	ScoringTypes::ScoringTypes()
		
		:	scoring_component_(0),
			type_map_()
	{
	}

	ScoringTypes::ScoringTypes(const ScoringTypes& st)
		
		:	scoring_component_(st.scoring_component_),
			type_map_(st.type_map_)
	{
	}

	ScoringTypes::ScoringTypes(const ScoringComponent& sc)
		
		:	scoring_component_(&sc),
			type_map_()
	{
		setup();
	}

	ScoringTypes::~ScoringTypes()
		
	{
		clear();
	}

	void ScoringTypes::clear()
		
	{
		scoring_component_ = 0;
		type_map_.clear();
	}

	bool ScoringTypes::setup()
		
	{
		// This is an interface definition and should be abstract. Always
		// return false.
		return(false);
	}

	const HashMap<const Atom*, Size>& ScoringTypes::getTypeMap() const
		
	{
		return(type_map_);
	}


}

