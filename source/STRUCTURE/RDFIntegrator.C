// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: RDFIntegrator.C,v 1.7.2.1 2003/01/07 13:21:56 anker Exp $

#include <BALL/STRUCTURE/RDFIntegrator.h>

using namespace std;

namespace BALL
{
	RDFIntegrator::RDFIntegrator()
		throw()
		:	rdf_(),
			valid_(false)
	{
	}


	RDFIntegrator::RDFIntegrator(const RDFIntegrator& integrator)
		throw()
		: rdf_(integrator.rdf_),
			valid_(false)
	{
	}
	

	RDFIntegrator::RDFIntegrator(const RadialDistributionFunction& rdf)
		throw()
		: rdf_(rdf),
			valid_(false)
	{
	}


	RDFIntegrator::~RDFIntegrator()
		throw()
	{
		clear();

		valid_ = false;
	}


	void RDFIntegrator::clear()
		throw()
	{
		rdf_.clear();
	}


	void RDFIntegrator::setRDF(const RadialDistributionFunction& rdf)
		throw()
	{
		rdf_ = rdf;
	}


	const RadialDistributionFunction& RDFIntegrator::getRDF() const 
		throw()
	{
		return rdf_;
	}


	const RDFIntegrator& RDFIntegrator::operator = (const RDFIntegrator& integrator)
		throw()
	{
		rdf_ = integrator.rdf_;
		return *this;
	}


	bool RDFIntegrator::operator == (const RDFIntegrator& integrator) const
		throw()
	{
		return (rdf_ == integrator.rdf_);
	}


	bool RDFIntegrator::isValid() const
		throw()
	{
		return valid_;
	}


	double RDFIntegrator::operator () (double /* x */) const
		throw()
	{
		Log.error() << "RDFIntegrator::operator () (double): "
								<< "This method should not be called!" << endl;
		return 0.0;
	}
}
