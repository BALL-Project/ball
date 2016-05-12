// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/MATHS/piecewiseFunction.h>

using namespace std;

namespace BALL
{

	PiecewiseFunction::PiecewiseFunction() 
		:	intervals_(0),
			coefficients_(0),
			valid_(false),
			range_()
	{
	}


	PiecewiseFunction::PiecewiseFunction(const PiecewiseFunction& function)
		:	intervals_(function.intervals_),
			coefficients_(function.coefficients_),
			valid_(function.valid_),
			range_(function.range_)
	{
	}


	PiecewiseFunction::PiecewiseFunction(const std::vector<Interval>& intervals,
			const std::vector<Coefficients>& coeffs) 
		:	intervals_(intervals),
			coefficients_(coeffs)
	{
		calculateRange();
		valid_ = isValid();
	}

	PiecewiseFunction::~PiecewiseFunction() 
	{
		clear();

		valid_ = false;
	}


	void PiecewiseFunction::clear() 
	{
		intervals_.clear();
		coefficients_.clear();
		calculateRange();
		valid_ = false;
	}


	void PiecewiseFunction::set(const std::vector<Interval>& intervals,
		const std::vector<Coefficients>& coeffs) 
	{
		intervals_ = intervals;
		coefficients_ = coeffs;
		calculateRange();
		valid_ = isValid();
	}


	PiecewiseFunction& PiecewiseFunction::operator = 
		(const PiecewiseFunction& function) 
	{
		intervals_ = function.intervals_;
		coefficients_ = function.coefficients_;
		valid_ = function.valid_;
		range_ = function.range_;

		return *this;
	}


	void PiecewiseFunction::setIntervals(const std::vector<Interval>& intervals) 
		
	{
		intervals_ = intervals;
		calculateRange();
		valid_ = isValid();
	}


	const std::vector<Interval>& PiecewiseFunction::getIntervals() const
	{
		return intervals_;
	}


	const Interval& PiecewiseFunction::getInterval(double x) const 
	{
		Position index = getIntervalIndex(x);
		// No error checking here, is handled by getIntervalIndex()
		return getInterval(index);
	}


	const Interval& PiecewiseFunction::getInterval(Position index) const
	{
		if (index < intervals_.size())
		{
			return intervals_[index];
		}
		else
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__);
		}
	}


	Position PiecewiseFunction::getIntervalIndex(double x) const
	{
		if (!isInRange(x))
		{
			throw Exception::OutOfRange(__FILE__, __LINE__);
		}

		for (Size i = 0; i < intervals_.size(); i++)
		{
			if ((x >= intervals_[i].first) && (x < intervals_[i].second))
			{
				return i;
			}
		}
		// control should not reach this point if x is in range, i. e. no
		// exception wa thrown
		Log.error() << "PiecewiseFunction::getIntervalIndex(): "
			<< "x cannot be associated" << endl;
		return 0;
	}


	const Interval& PiecewiseFunction::getRange() const 
	{
		return range_;
	}


	void PiecewiseFunction::setCoefficients
		(const std::vector<Coefficients>& coefficients) 
	{
		coefficients_ = coefficients;
		valid_ = isValid();
	}


	const std::vector<Coefficients>& PiecewiseFunction::getCoefficients() const  
	{
		if (coefficients_.empty())
		{
			Log.warn() << "PiecewiseFunction::getCoefficients(): "
				<< "No coefficients defined." << endl;
		}
		return coefficients_;
	}


	const Coefficients& PiecewiseFunction::getCoefficients(double x) const
	{
		Position index = getIntervalIndex(x);
		// No error checking here, is handled by getIntervalIndex()
		return coefficients_[index];
	}


	const Coefficients& PiecewiseFunction::getCoefficients(Position index) const
	{
		if (index < coefficients_.size())
		{
			return coefficients_[index];
		}
		else
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__);
		}
	}


	bool PiecewiseFunction::isValid() const 
	{

		// ?????: ist das vollst�ndig?
		if (coefficients_.empty() || intervals_.empty())
		{
			return false;
		}

		if (intervals_.size() != coefficients_.size())
		{
			return false;
		}

		for (Size k = 0; k < intervals_.size(); ++k)
		{
			if (intervals_[k].first >= intervals_[k].second)
			{
				return false;
			}
		}

		for (Size k = 1; k < intervals_.size(); ++k)
		{
			if (intervals_[k-1].second != intervals_[k].first)
			{
				return false;
			}
		}

		return true;
	}


	double PiecewiseFunction::operator() (double /* x */) const 
	{
		return 0.0;
	}


	bool PiecewiseFunction::isInRange(double x) const 
	{
		return (x >= range_.first) && (x < range_.second);
	}


	bool PiecewiseFunction::operator == (const PiecewiseFunction& function)
		const 
	{
		return ((intervals_ == function.intervals_)
			&& (coefficients_ == function.coefficients_));
	}


	void PiecewiseFunction::dump(ostream& stream, Size /* depth */) const
		
	{
		if (!isValid())
		{
			Log.error() << "PiecewiseFunction::dump(): instance is not valid\n";
			return;
		}
		stream << "[PiecewiseFunction: " << intervals_.size() << " intervals]"
			<< endl;
		vector<Interval>::const_iterator int_it = intervals_.begin();
		vector<Coefficients>::const_iterator coef_it = coefficients_.begin();
		Coefficients::const_iterator c_it;
		for(; int_it != intervals_.end(); ++int_it, ++coef_it)
		{
			stream << "[" << int_it->first << "," << int_it->second << "): ";
			for(c_it = coef_it->begin(); c_it != coef_it->end(); ++c_it)
			{
				stream << *c_it << " ";
			}
			stream << endl;
		}
	}


	void PiecewiseFunction::calculateRange() 
	{
		if (intervals_.empty())
		{
			range_.first = range_.second = 0.0;
		}
		else
		{
			range_.first = intervals_[0].first;
			range_.second = intervals_[intervals_.size()-1].second;
		}
	}

} // namespace BALL

