// $Id: piecewisePolynomial.C,v 1.6 2001/12/30 13:28:47 sturm Exp $

#include <BALL/MATHS/piecewisePolynomial.h>

using namespace std;

namespace BALL
{
	
	PiecewisePolynomial::PiecewisePolynomial() throw()
		:	PiecewiseFunction(),
			degree_(0)
	{
	}

	PiecewisePolynomial::PiecewisePolynomial(const PiecewisePolynomial& poly)
		throw()
		:	PiecewiseFunction(poly),
			degree_(poly.degree_)
	{
	}

	PiecewisePolynomial::PiecewisePolynomial(Size degree, 
			const std::vector<Interval>& intervals, 
			const std::vector<Coefficients>& coefficients) throw()
		:	PiecewiseFunction(intervals, coefficients),
			degree_(degree)
	{
	}


	PiecewisePolynomial::~PiecewisePolynomial() throw()
	{
		clear();

		valid_ = false;
	}


	void PiecewisePolynomial::set(Size degree,
			const std::vector<Interval>& intervals,
			const std::vector<Coefficients>& coeffs) throw()
	{
		PiecewiseFunction::set(intervals, coeffs);
		degree_ = degree;
	}


	void PiecewisePolynomial::clear() throw()
	{
		degree_ = 0;

		PiecewiseFunction::clear();
	}


	const PiecewisePolynomial& PiecewisePolynomial::operator =
		(const PiecewisePolynomial& poly) throw()
	{
		PiecewiseFunction::operator = (poly);
		degree_ = poly.degree_;

		return *this;
	}


	void PiecewisePolynomial::setDegree(Size degree) throw()
	{
		degree_ = degree;
	}


	Size PiecewisePolynomial::getDegree() const throw()
	{
		return degree_;
	}


	double PiecewisePolynomial::operator () (double x) const throw()
	{
		// ?????
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


	bool PiecewisePolynomial::operator == (const PiecewisePolynomial& poly)
		const throw()
	{
		return (PiecewiseFunction::operator == (poly)
			&& (degree_ == poly.degree_));
	}


	void PiecewisePolynomial::dump(ostream& stream, Size /* depth */) const
		throw() 
	{
		stream << "[PiecewisePolynomial: degree " << degree_ << "]" << endl;
		PiecewiseFunction::dump();
	}

}
