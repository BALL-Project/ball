#include "piecewisePolynomial.h"

namespace BALL
{
	
	PiecewisePolynomial::PiecewisePolynomial()
		:	PiecewiseFunction(),
			degree_(0)
	{
	}

	PiecewisePolynomial::PiecewisePolynomial(const PiecewisePolynomial& poly)
		:	PiecewiseFunction(poly.intervals_, poly.coefficients_),
			degree_(poly.degree_)
	{
	}

	PiecewisePolynomial::PiecewisePolynomial(Size degree, 
			const std::vector<Interval>& intervals, 
			const std::vector<Coefficients>& coefficients)
		:	PiecewiseFunction(intervals, coefficients),
			degree_(degree)
	{
		
	}

	PiecewisePolynomial::~PiecewisePolynomial()
	{
	}


	void PiecewisePolynomial::set(Size degree,
			const std::vector<Interval>& intervals,
			const std::vector<Coefficients>& coeffs)
	{
		PiecewiseFunction::set(intervals, coeffs);
		degree_ = degree;
	}


	void PiecewisePolynomial::destroy()
	{
		clear();
	}


	void PiecewisePolynomial::clear()
	{
		degree_ = 0;
		PiecewiseFunction::clear();
	}


	void PiecewisePolynomial::set(const PiecewisePolynomial& poly)
	{
		degree_ = poly.degree_;
		PiecewiseFunction::set(poly);
	}


	PiecewisePolynomial& PiecewisePolynomial::operator =
		(const PiecewisePolynomial& poly)
	{
		set(poly);
		return *this;
	}


	void PiecewisePolynomial::setDegree(Size degree)
	{
		degree_ = degree;
	}

	Size PiecewisePolynomial::getDegree() const
	{
		return degree_;
	}


	double PiecewisePolynomial::operator () (double x) const
	{
		// BAUSTELLE
		if (!isInRange(x))
		{
			Log.error() << "PiecewisePolynomial::operator () (): x is not in range" 
				<< endl;
			return 0;
		}
		else
		{
			Position k = getIntervalIndex(x);
			double val = 0.0;
			double sub = intervals_[k].first;
			for (Size i = 0; i < degree_; ++i)
			{
				val += coefficients_[k][i] * pow((x - sub), (int) i);
			}
			return val;
		}
	}

}
