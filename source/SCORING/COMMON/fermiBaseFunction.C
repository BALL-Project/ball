// $Id: fermiBaseFunction.C,v 1.1 2005/11/21 19:27:08 anker Exp $

#include <BALL/common.h>
#include <BALL/SCORING/COMMON/fermiBaseFunction.h>

namespace BALL 
{
	FermiBaseFunction::FermiBaseFunction()
		
		:	a_(0.0),
			b_(0.0)
	{
	}

	FermiBaseFunction::FermiBaseFunction(float lower,
			float upper)
		
	{
		// ????? see constructor of BaseFunctionLinear
		// BaseFunction::BaseFunction(lower, upper);
		lower_ = lower;
		upper_ = upper;
		computeFermiParameters_();
	}

	void FermiBaseFunction::setLower(float lower)
		
	{
		ScoringBaseFunction::setLower(lower);
		// computeFermiParameters_();
	}

	void FermiBaseFunction::setUpper(float upper)
		
	{
		ScoringBaseFunction::setUpper(upper);
		// computeFermiParameters_();
	}

	float FermiBaseFunction::calculate(float x) const
		
	{
		return(1.0 / (1.0 + exp(-a_ * x + b_)));
	}

	float FermiBaseFunction::calculate(float x, float lower,
			float upper)
		
	{
		setLower(lower);
		setUpper(upper);
		computeFermiParameters_();
		return(calculate(x));
	}


	void FermiBaseFunction::computeFermiParameters_()
		
	{
		a_ = 4.0 / (lower_ - upper_);
		b_ = a_ * (lower_ + 0.5 * (upper_ - lower_));
	}


}
