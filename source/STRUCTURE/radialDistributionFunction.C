// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/radialDistributionFunction.h>

using namespace std;

namespace BALL
{

	RadialDistributionFunction::RadialDistributionFunction() 
		:	representation_(),
			valid_(false)
	{
	}


	RadialDistributionFunction::RadialDistributionFunction 
		(const RadialDistributionFunction& rdf) 
		:	representation_(rdf.representation_),
			valid_(rdf.valid_)
	{
	}


	RadialDistributionFunction::RadialDistributionFunction
		(const PiecewisePolynomial& polynomial) 
		:	representation_(polynomial)
	{
		valid_ = representation_.isValid();
	}


	RadialDistributionFunction::~RadialDistributionFunction() 
	{
		clear();

		valid_ = false;
	}


	void RadialDistributionFunction::clear() 
	{
		representation_.clear();

		valid_ = false;
	}

	const RadialDistributionFunction& RadialDistributionFunction::operator =
		(const RadialDistributionFunction& rdf) 
	{
		representation_ = rdf.representation_;
		valid_ = rdf.valid_;

		return *this;
	}


	void RadialDistributionFunction::setRepresentation
		(const PiecewisePolynomial& polynomial) 
	{
		representation_ = polynomial;
		valid_ = polynomial.isValid();
	}


	const PiecewisePolynomial& RadialDistributionFunction::getRepresentation() 
		const 
	{
		return representation_;
	}


	const Interval& RadialDistributionFunction::getRange() const 
	{
		return representation_.getRange();
	}


	double RadialDistributionFunction::operator () (double x) const 
	{
		return representation_.operator () (x);
	}


	bool RadialDistributionFunction::isInRange(double x) const 
	{
		return representation_.isInRange(x);
	}


	bool RadialDistributionFunction::operator == 
		(const RadialDistributionFunction& rdf) const 
	{
		return (representation_ == rdf.representation_);
	}


	bool RadialDistributionFunction::isValid() const 
	{
		return valid_;
	}

	
	void RadialDistributionFunction::dump(ostream& stream, Size /* depth */)
		const 
	{
		representation_.dump(stream);
	}

} // namespace BALL
