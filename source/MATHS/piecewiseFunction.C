// $Id: piecewiseFunction.C,v 1.2 2000/08/31 12:32:30 anker Exp $

#include <BALL/MATHS/piecewiseFunction.h>

namespace BALL
{

	PiecewiseFunction::PiecewiseFunction()
		:	intervals_(0),
			coefficients_(0)
	{
	}


	PiecewiseFunction::PiecewiseFunction(const PiecewiseFunction& function)
		:	intervals_(function.intervals_),
			coefficients_(function.coefficients_)
	{
	}


	PiecewiseFunction::PiecewiseFunction(const std::vector<Interval>& intervals,
			const std::vector<Coefficients>& coeffs)
		:	intervals_(intervals),
			coefficients_(coeffs)
	{
	}


	PiecewiseFunction::~PiecewiseFunction()
	{
		destroy();
	}


	void PiecewiseFunction::clear()
	{
		intervals_.clear();
		coefficients_.clear();
	}


	void PiecewiseFunction::destroy()
	{
		clear();
	}


	void PiecewiseFunction::set(const PiecewiseFunction& function)
	{
		intervals_ = function.intervals_;
		coefficients_ = function.coefficients_;
		valid_ = function.valid_;
	}


	void PiecewiseFunction::set(const std::vector<Interval>& intervals,
		const std::vector<Coefficients>& coeffs)
	{
		intervals_ = intervals;
		coefficients_ = coeffs;
		valid_ = isValid();
	}


	PiecewiseFunction& PiecewiseFunction::operator = 
		(const PiecewiseFunction& function)
	{
		set(function);
		return *this;
	}


	void PiecewiseFunction::setIntervals(const std::vector<Interval>& intervals)
	{
		intervals_ = intervals;
	}


	const std::vector<Interval>& PiecewiseFunction::getIntervals() const
	{
		return intervals_;
	}


	const Interval& PiecewiseFunction::getInterval(double x) const
	{
		Position index = getIntervalIndex(x);
		if (index == INVALID_POSITION)
		{
			return INVALID_INTERVAL;
		}
		else
		{
			return getInterval(index);
		}
	}


	const Interval& PiecewiseFunction::getInterval(Position index) const
	{
		if (intervals_.size() == 0)
		{
			Log.warn() << "PiecewiseFunction::getInterval(): "
				<< "No intervals defined." << endl;
			return INVALID_INTERVAL;
		}
		if (index < intervals_.size())
		{
			return intervals_[index];
		}
		else
		{
			Log.error() << "PiecewiseFunction::getInterval(Position): "
				<< "index is too large." << endl;
			return INVALID_INTERVAL;
		}
	}

	Position PiecewiseFunction::getIntervalIndex(double x) const
	{
		if (!isInRange(x))
		{
			Log.error() << "PiecewiseFunction::getIntervalIndex(): "
				<< "x is out of range" << endl;
			return INVALID_POSITION;
		}

		for (Size i = 0; i < intervals_.size(); i++)
		{
			if ((x >= intervals_[i].first) && (x < intervals_[i].second))
			{
				return i;
			}
		}
		// control should not reach this point
		Log.error() << "PiecewiseFunction::getIntervalIndex(): "
			<< "x cannot be associated" << endl;
		return false;
	}

	Interval PiecewiseFunction::getRange() const
	{
		// as this is a local variable, the return value is not a const ref
		Interval tmp;
		if (intervals_.size() == 0)
		{
			Log.error() << "PiecewiseFunction::getRange(): "
				<< "No intervals defined." << endl;
			tmp = INVALID_INTERVAL;
		}
		else
		{
			tmp.first = intervals_[0].first;
			tmp.second = intervals_[intervals_.size()-1].second;
		}
		return tmp;
	}

	void PiecewiseFunction::setCoefficients(const std::vector<Coefficients>& coefficients)
	{
		coefficients_ = coefficients;
	}

	const std::vector<Coefficients>& PiecewiseFunction::getCoefficients() const
	{
		if (coefficients_.size() == 0)
		{
			Log.warn() << "PiecewiseFunction::getCoefficients(): "
				<< "No coefficients defined." << endl;
		}
		return coefficients_;
	}


	const Coefficients& PiecewiseFunction::getCoefficients(double x) const
	{
		if (coefficients_.size() == 0)
		{
			Log.warn() << "PiecewiseFunction::getCoefficients(): "
				<< "No coefficients defined." << endl;
			return INVALID_COEFFICIENTS;
		}
		Position index = getIntervalIndex(x);
		if (index != INVALID_POSITION)
		{
			return coefficients_[index];
		}
		else
		{
			return INVALID_COEFFICIENTS;
		}
	}

	const Coefficients& PiecewiseFunction::getCoefficients(Position index) const
	{
		if (coefficients_.size() == 0)
		{
			Log.warn() << "PiecewiseFunction::getCoefficients(): "
				<< "No coefficients defined." << endl;
			return INVALID_COEFFICIENTS;
		}
		if (index < coefficients_.size())
		{
			return coefficients_[index];
		}
		else
		{
			Log.error() << "PiecewiseFunction::getCoefficients(): index too large" 
				<< endl;
			return INVALID_COEFFICIENTS;
		}
	}

	bool PiecewiseFunction::isValid() const
	{

		// BAUSTELLE: ist das vollständig?
		if ((coefficients_.size() == 0) || (intervals_.size() == 0))
		{
			Log.error() << "PiecewiseFunction::isValid(): "
				<< "No coefficients and/or intervals defined." << endl;
			return false;
		}

		if (intervals_.size() != coefficients_.size())
		{
			Log.error() << "PiecewiseFunction::isValid(): "
				<< "Number of intervals and coefficients do not match" << endl;
			return false;
		}

		for (Size k = 0; k < intervals_.size(); ++k)
		{
			if (intervals_[k].first >= intervals_[k].second)
			{
				Log.error() << "PiecewiseFunction::isValid(): "
					<< "First limit of an interval has to be smaller than second limit" 
					<< endl;
				return false;
			}
		}

		for (Size k = 1; k < intervals_.size(); ++k)
		{
			if (intervals_[k-1].second != intervals_[k].first)
			{
				Log.error() << "PiecewiseFunction::isValid(): "
					<< "Intervals are not continuous" << endl;
				return false;
			}
		}

		return true;
	}

	double PiecewiseFunction::operator() (double /* x */) const
	{
		Log.error() << "PiecewiseFunction::operator(): "
			<< "This function shouldn't be called!" << endl;
		return 0.0;
	}

	bool PiecewiseFunction::isInRange(double x) const
	{
		Interval range = getRange();
		if ((x >= range.first) && (x < range.second))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

} // namespace BALL

