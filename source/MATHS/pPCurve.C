// $Id: pPCurve.C,v 1.2 2000/07/13 11:56:43 anker Exp $

#include <BALL/MATHS/pPCurve.h>

using namespace std;

namespace BALL
{

	PPCurve::PPCurve()
		:	valid_(false)
	{
	}

	PPCurve::PPCurve(const PPCurve& ppcurve) 
		: degree_(ppcurve.degree_),
			number_of_intervals_(ppcurve.number_of_intervals_),
			intervals_(ppcurve.intervals_),
			coefficients_(ppcurve.coefficients_),
			valid_(ppcurve.valid_)
	{
	}

	/* BAUSTELLE: this constructor doesn't check anything!!! */
	PPCurve::PPCurve(Size degree, Size number_of_intervals, 
			vector<float>& intervals, vector< vector<float> >& coefficients)
		: degree_(degree),
			number_of_intervals_(number_of_intervals),
			intervals_(intervals),
			coefficients_(coefficients),
			valid_(false)
	{
	}

	PPCurve::~PPCurve()
	{
		valid_ = false;
	}

	void PPCurve::clear()
	{
		degree_ = 0;
		number_of_intervals_ = 0;
		intervals_.clear();
		coefficients_.clear();
		valid_ = true;
	}

	void PPCurve::destroy()
	{
		clear();
	}

	void PPCurve::setDegree(Size degree)
	{
		degree_ = degree;
	}

	Size PPCurve::getDegree() const
	{
		return degree_;
	}

	void PPCurve::setIntervals(vector<float>& intervals)
	{
		intervals_ = intervals;
	}

	void PPCurve::setIntervals(const vector<float>& intervals)
	{
		intervals_ = intervals;
	}

	vector<float> PPCurve::getIntervals() const
	{
		return intervals_;
	}

	void PPCurve::setCoefficients(vector< vector<float> >& coefficients)
	{
		coefficients_ = coefficients;
	}

	void PPCurve::setCoefficients(const vector< vector<float> >& coefficients)
	{
		coefficients_ = coefficients;
	}

	vector< vector<float> > PPCurve::getCoefficients() const
	{
		return coefficients_;
	}

	void PPCurve::checkValidity()
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

	float PPCurve::eval(float x)
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

	float PPCurve::sum(float x, Size index)
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
