// $Id: scoringComponent.C,v 1.1 2005/11/21 19:27:08 anker Exp $

#include <BALL/SCORING/COMMON/scoringComponent.h>

namespace BALL
{

	ScoringComponent::ScoringComponent()
		throw()
		:	scoring_function_(0),
			score_(0.0),
			name_("")
	{
		// ???
	}


	ScoringComponent::ScoringComponent(const ScoringComponent& sc)
		throw()
		:	scoring_function_(sc.scoring_function_),
			score_(sc.score_),
			name_(sc.name_)
	{
		// ???
	}


	ScoringComponent::ScoringComponent(ScoringFunction& sf)
		throw()
		:	scoring_function_(&sf),
			score_(0.0),
			name_("")
	{
		// ???
	}


	ScoringComponent::~ScoringComponent()
		throw()
	{
		clear();
	}


	void ScoringComponent::clear()
		throw()
	{
		scoring_function_ = 0;
		score_ = 0.0;
		name_ = "";
	}


	bool ScoringComponent::setup()
		throw()
	{
		return(true);
	}


	ScoringFunction* ScoringComponent::getScoringFunction() const
		throw()
	{
		return(scoring_function_);
	}


	void ScoringComponent::setScoringFunction(ScoringFunction& sf)
		throw()
	{
		scoring_function_ = &sf;
	}


	String ScoringComponent::getName() const
		throw()
	{
		return(name_);
	}


	void ScoringComponent::setName(const String& name)
		throw()
	{
		name_ = name;
	}


	double ScoringComponent::calculateScore()
		throw()
	{
		return(score_);
	}


	double ScoringComponent::getScore() const
		throw()
	{
		return(score_);
	}

}
