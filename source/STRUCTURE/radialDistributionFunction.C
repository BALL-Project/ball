// $Id: radialDistributionFunction.C,v 1.7 2000/10/18 13:55:53 anker Exp $

#include <BALL/STRUCTURE/radialDistributionFunction.h>

using namespace std;

namespace BALL
{

	RadialDistributionFunction::RadialDistributionFunction() throw()
		:	representation_(),
			valid_(false)
	{
	}


	RadialDistributionFunction::RadialDistributionFunction 
		(const RadialDistributionFunction& rdf) throw()
		:	representation_(rdf.representation_),
			valid_(rdf.valid_)
	{
	}


	RadialDistributionFunction::RadialDistributionFunction
		(const PiecewisePolynomial& polynomial) throw()
		:	representation_(polynomial)
	{
		valid_ = representation_.isValid();
	}


	RadialDistributionFunction::~RadialDistributionFunction() throw()
	{
		clear();

		valid_ = false;
	}


	void RadialDistributionFunction::clear() throw()
	{
		representation_.clear();

		valid_ = false;
	}

	const RadialDistributionFunction& RadialDistributionFunction::operator =
		(const RadialDistributionFunction& rdf) throw()
	{
		representation_ = rdf.representation_;
		valid_ = rdf.valid_;

		return *this;
	}


	void RadialDistributionFunction::setRepresentation
		(const PiecewisePolynomial& polynomial) throw()
	{
		representation_ = polynomial;
		valid_ = polynomial.isValid();
	}


	const PiecewisePolynomial& RadialDistributionFunction::getRepresentation() 
		const throw()
	{
		return representation_;
	}


	const Interval& RadialDistributionFunction::getRange() const throw()
	{
		return representation_.getRange();
	}


	double RadialDistributionFunction::operator () (double x) const throw()
	{
		return representation_.operator () (x);
	}


	bool RadialDistributionFunction::isInRange(double x) const throw()
	{
		return representation_.isInRange(x);
	}


	bool RadialDistributionFunction::operator == 
		(const RadialDistributionFunction& rdf) const throw()
	{
		return (representation_ == rdf.representation_);
	}


	bool RadialDistributionFunction::isValid() const throw()
	{
		return valid_;
	}

	
	void RadialDistributionFunction::dump(ostream& stream, Size /* depth */)
		const throw()
	{
		representation_.dump(stream);
	}

} // namespace BALL
