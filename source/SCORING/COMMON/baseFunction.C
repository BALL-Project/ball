// $Id: baseFunction.C,v 1.1 2005/11/21 19:27:07 anker Exp $

#include <BALL/SCORING/COMMON/baseFunction.h>

namespace BALL
{

	ScoringBaseFunction::ScoringBaseFunction()
		throw()
		:	lower_(0.0f),
			upper_(0.0f)
	{
	}


	ScoringBaseFunction::ScoringBaseFunction(float lower, float upper)
		throw()
		:	lower_(lower),
			upper_(upper)
	{
	}


	void ScoringBaseFunction::setLower(float lower)
		throw()
	{
		lower_ = lower;
	}


	void ScoringBaseFunction::setUpper(float upper)
		throw()
	{
		upper_ = upper;
	}


	float ScoringBaseFunction::calculate(float /* x */) const 
		throw()
	{
		return(0.0);
	}


	float ScoringBaseFunction::calculate(float /* x */, float /* lower */,
			float /* upper */) 
		throw()
	{
		return(0.0);
	}



}

