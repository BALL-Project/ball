// $Id: pPCurve.C,v 1.1 2000/07/13 11:48:43 anker Exp $

#include "spline.h"

using namespace std;

namespace BALL
{

	Spline::Spline()
		:	valid_(false)
	{
	}

	Spline::Spline(const Spline& spline) 
		: degree_(spline.degree_),
			number_of_intervals_(spline.number_of_intervals_),
			intervals_(spline.intervals_),
			coefficients_(spline.coefficients_),
			valid_(spline.valid_)
	{
	}

	/* BAUSTELLE: this constructor doesn't check anything!!! */
	Spline::Spline(Size degree, Size number_of_intervals, 
			vector<float>& intervals, vector< vector<float> >& coefficients)
		: degree_(degree),
			number_of_intervals_(number_of_intervals),
			intervals_(intervals),
			coefficients_(coefficients),
			valid_(false)
	{
	}

	Spline::~Spline()
	{
		valid_ = false;
	}

	void Spline::clear()
	{
		degree_ = 0;
		number_of_intervals_ = 0;
		intervals_.clear();
		coefficients_.clear();
		valid_ = true;
	}

	void Spline::destroy()
	{
		clear();
	}

	void Spline::setDegree(Size degree)
	{
		degree_ = degree;
	}

	Size Spline::getDegree() const
	{
		return degree_;
	}

	void Spline::setIntervals(vector<float>& intervals)
	{
		intervals_ = intervals;
	}

	void Spline::setIntervals(const vector<float>& intervals)
	{
		intervals_ = intervals;
	}

	vector<float> Spline::getIntervals() const
	{
		return intervals_;
	}

	void Spline::setCoefficients(vector< vector<float> >& coefficients)
	{
		coefficients_ = coefficients;
	}

	void Spline::setCoefficients(const vector< vector<float> >& coefficients)
	{
		coefficients_ = coefficients;
	}

	vector< vector<float> > Spline::getCoefficients() const
	{
		return coefficients_;
	}

	void Spline::checkValidity()
	{
		if (coefficients_.size() == number_of_intervals_ + 2) 
		{
			vector< vector<float> >::iterator it = coefficients_.begin();
			for (; it != coefficients_.end(); ++it)
			{
				if (it->size() != degree_)
				{
					valid_ = false;
					return;
				}
			}
			if (number_of_intervals_ == intervals_.size() - 1)
			{
				valid_ = true;
				return;
			}
		}
		valid_ = false;
	}

	float Spline::eval(float x)
	{
		for (Size k = 0; k < intervals_.size(); k++)
		{
			if (x < intervals_[k])
			{
				return sum(x, k);
			}
		}
		return sum(x, intervals_.size());
	}

	float Spline::sum(float x, Size index)
	{
		float val = 0.0;
		vector<float> coef;
		coef = coefficients_[index];
		for (Size i = 0; i < degree_; ++i)
		{
			val += coef[i] * pow(x, i);
		}
		return val;
	}

}
