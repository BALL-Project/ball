// $Id: baseFunction.C,v 1.2 2005/11/25 14:03:55 anker Exp $

#include <BALL/SCORING/COMMON/baseFunction.h>

namespace BALL
{

	ScoringBaseFunction::ScoringBaseFunction()
		
		:	lower_(0.0f),
			upper_(0.0f)
	{
	}


	ScoringBaseFunction::ScoringBaseFunction(float lower, float upper)
		
		:	lower_(lower),
			upper_(upper)
	{
	}


	ScoringBaseFunction::~ScoringBaseFunction()
		
	{
	}


	void ScoringBaseFunction::setLower(float lower)
		
	{
		lower_ = lower;
	}


	void ScoringBaseFunction::setUpper(float upper)
		
	{
		upper_ = upper;
	}


	float ScoringBaseFunction::calculate(float /* x */) const 
		
	{
		return(0.0);
	}


	float ScoringBaseFunction::calculate(float /* x */, float /* lower */,
			float /* upper */) 
		
	{
		return(0.0);
	}



}

