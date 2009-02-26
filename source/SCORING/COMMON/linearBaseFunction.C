// $Id: linearBaseFunction.C,v 1.1 2005/11/21 19:27:08 anker Exp $

#include <BALL/common.h>
#include <BALL/SCORING/COMMON/linearBaseFunction.h>

namespace BALL
{

	LinearBaseFunction::LinearBaseFunction()
		
	{
	}

	LinearBaseFunction::LinearBaseFunction(float lower, float upper)
		
	{
		lower_ = lower;
		upper_ = upper;
	}

	float LinearBaseFunction::calculate(float x) const
		
	{

		float return_value;

		// ??? The following might be wrong if someone wants to score negative
		// values of something. We might remove that if.
		if (x < 0.0)
		{
			Log.error() << "LinearBaseFunction::calculate(): "
				<< "negative score, returning 0." << std::endl;
			return(0.0);
		}

		if (lower_ == upper_)
		{
			Log.error() << "LinearBaseFunction::calculate(): "
				<< "lower == upper, division by zero, returning 0" << std::endl;
			return(0.0f);
		}

		// if lower_ < upper return the function that returns 1 for x < lower
		// and 0 for x > upper. Otherwise invert the function.
		if (lower_ < upper_)
		{
			if (x <= lower_)
			{
				return_value = 1.0f;
			}
			else
			{
				if (x <= upper_)
				{
					return_value = 1.0f - ((x - lower_)/(upper_ - lower_));
				}
				else
				{
					return_value = 0.0f;
				}
			}
		}
		else
		{
			// Careful! The meaning of upper_ and lower_ is interchanged in this
			// context
			if (x <= upper_)
			{
				return_value = 0.0f;
			}
			else
			{
				if (x <= lower_)
				{
					return_value = (x - upper_)/(lower_ - upper_);
				}
				else
				{
					return_value = 1.0f;
				}
			}
		}

		return return_value;
	}

	float LinearBaseFunction::calculate(float x, float lower,
			float upper) 
		
	{
		setLower(lower);
		setUpper(upper);
		return(calculate(x));
	}


}
