// $Id: piecewiseFunction.C,v 1.10 2001/12/30 13:28:47 sturm Exp $

#include <BALL/MATHS/piecewiseFunction.h>

using namespace std;

namespace BALL
{

	PiecewiseFunction::PiecewiseFunction() throw()
		:	intervals_(0),
			coefficients_(0),
			valid_(false),
			range_()
	{
	}


	PiecewiseFunction::PiecewiseFunction(const PiecewiseFunction& function)
		throw() 
		:	intervals_(function.intervals_),
			coefficients_(function.coefficients_),
			valid_(function.valid_),
			range_(function.range_)
	{
	}


	PiecewiseFunction::PiecewiseFunction(const std::vector<Interval>& intervals,
			const std::vector<Coefficients>& coeffs) throw()
		:	intervals_(intervals),
			coefficients_(coeffs)
	{
		calculateRange();
		valid_ = isValid();
	}


	PiecewiseFunction::~PiecewiseFunction() throw()
	{
		clear();

		valid_ = false;
	}


	void PiecewiseFunction::clear() throw()
	{
		intervals_.clear();
		coefficients_.clear();
		calculateRange();
		valid_ = false;
	}


	void PiecewiseFunction::set(const std::vector<Interval>& intervals,
		const std::vector<Coefficients>& coeffs) throw()
	{
		intervals_ = intervals;
		coefficients_ = coeffs;
		calculateRange();
		valid_ = isValid();
	}


	PiecewiseFunction& PiecewiseFunction::operator = 
		(const PiecewiseFunction& function) throw()
	{
		intervals_ = function.intervals_;
		coefficients_ = function.coefficients_;
		valid_ = function.valid_;
		range_ = function.range_;

		return *this;
	}


	void PiecewiseFunction::setIntervals(const std::vector<Interval>& intervals) 
		throw()
	{
		intervals_ = intervals;
		calculateRange();
		valid_ = isValid();
	}


	const std::vector<Interval>& PiecewiseFunction::getIntervals() const
		throw()
	{
		return intervals_;
	}


	const Interval& PiecewiseFunction::getInterval(double x) const 
		throw(Exception::OutOfRange)
	{
		Position index = getIntervalIndex(x);
		// No error checking here, is handled by getIntervalIndex()
		return getInterval(index);
	}


	const Interval& PiecewiseFunction::getInterval(Position index) const
		throw(Exception::IndexOverflow)
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
		throw(Exception::OutOfRange)
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


	const Interval& PiecewiseFunction::getRange() const throw()
	{
		return range_;
	}


	void PiecewiseFunction::setCoefficients
		(const std::vector<Coefficients>& coefficients) throw()
	{
		coefficients_ = coefficients;
		valid_ = isValid();
	}


	const std::vector<Coefficients>& PiecewiseFunction::getCoefficients()
		const throw() 
	{
		if (coefficients_.size() == 0)
		{
			Log.warn() << "PiecewiseFunction::getCoefficients(): "
				<< "No coefficients defined." << endl;
		}
		return coefficients_;
	}


	const Coefficients& PiecewiseFunction::getCoefficients(double x) const
		throw(Exception::OutOfRange)
	{
		Position index = getIntervalIndex(x);
		// No error checking here, is handled by getIntervalIndex()
		return coefficients_[index];
	}


	const Coefficients& PiecewiseFunction::getCoefficients(Position index)
		const throw(Exception::IndexOverflow)
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


	bool PiecewiseFunction::isValid() const throw()
	{

		// ?????: ist das vollständig?
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


	double PiecewiseFunction::operator() (double /* x */) const throw()
	{
		Log.error() << "PiecewiseFunction::operator(): "
			<< "This function shouldn't be called!" << endl;
		return 0.0;
	}


	bool PiecewiseFunction::isInRange(double x) const throw()
	{
		if ((x >= range_.first) && (x < range_.second))
		{
			return true;
		}
		else
		{
			return false;
		}
	}


	bool PiecewiseFunction::operator == (const PiecewiseFunction& function)
		const throw()
	{
		return ((intervals_ == function.intervals_)
			&& (coefficients_ == function.coefficients_));
	}


	void PiecewiseFunction::dump(ostream& stream, Size /* depth */) const
		throw()
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


	void PiecewiseFunction::calculateRange() throw()
	{
		if (intervals_.size() == 0)
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

