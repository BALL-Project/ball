// $Id: radialDistributionFunction.C,v 1.5 2000/09/25 11:19:21 anker Exp $

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


	RadialDistributionFunction::RadialDistributionFunction(const PiecewisePolynomial& polynomial)
		:	representation_(polynomial)
	{
		valid_ = representation_.isValid();
	}


	RadialDistributionFunction::~RadialDistributionFunction()
	{
		destroy();
	}


	void RadialDistributionFunction::clear()
	{
		representation_.clear();
		valid_ = false;
	}


	void RadialDistributionFunction::destroy()
	{
		clear();
	}


	void RadialDistributionFunction::set(const RadialDistributionFunction& rdf)
	{
		representation_.set(rdf.representation_);
		valid_ = representation_.isValid();
	}
	

	const RadialDistributionFunction& RadialDistributionFunction::operator =
		(const RadialDistributionFunction& rdf)
	{
		set(rdf);
		return *this;
	}


	void RadialDistributionFunction::setRepresentation(const PiecewisePolynomial& polynomial)
	{
		representation_ = polynomial;
		valid_ = polynomial.isValid();
	}


	PiecewisePolynomial RadialDistributionFunction::getRepresentation() const
	{
		return representation_;
	}


	Interval RadialDistributionFunction::getRange() const
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


	bool RadialDistributionFunction::isValid()
	{
		// BAUSTELLE
		return representation_.isValid();
	}

	
	void RadialDistributionFunction::dump(ostream& stream, Size /* depth */) const
	{
		representation_.dump(stream);
	}

} // namespace BALL
