// $Id: scoringComponent.C,v 1.1 2005/11/21 19:27:08 anker Exp $

#include <BALL/SCORING/COMMON/scoringComponent.h>

namespace BALL
{

	ScoringComponent::ScoringComponent()
		
		:	scoring_function_(0),
			score_(0.0),
			name_("")
	{
		// ???
	}


	ScoringComponent::ScoringComponent(const ScoringComponent& sc)
		
		:	scoring_function_(sc.scoring_function_),
			score_(sc.score_),
			name_(sc.name_)
	{
		// ???
	}


	ScoringComponent::ScoringComponent(ScoringFunction& sf)
		
		:	scoring_function_(&sf),
			score_(0.0),
			name_("")
	{
		// ???
	}


	ScoringComponent::~ScoringComponent()
		
	{
		clear();
	}


	void ScoringComponent::clear()
		
	{
		scoring_function_ = 0;
		score_ = 0.0;
		name_ = "";
	}


	bool ScoringComponent::setup()
		
	{
		return(true);
	}


	ScoringFunction* ScoringComponent::getScoringFunction() const
		
	{
		return(scoring_function_);
	}


	void ScoringComponent::setScoringFunction(ScoringFunction& sf)
		
	{
		scoring_function_ = &sf;
	}


	String ScoringComponent::getName() const
		
	{
		return(name_);
	}


	void ScoringComponent::setName(const String& name)
		
	{
		name_ = name;
	}


	double ScoringComponent::calculateScore()
		
	{
		return(score_);
	}


	double ScoringComponent::getScore() const
		
	{
		return(score_);
	}

}
