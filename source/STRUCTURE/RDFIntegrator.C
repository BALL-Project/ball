// $Id: RDFIntegrator.C,v 1.4 2000/09/02 16:18:02 anker Exp $

#include <BALL/STRUCTURE/RDFIntegrator.h>

using namespace std;

namespace BALL
{
	RDFIntegrator::RDFIntegrator()
		:	rdf_()
	{
	}


	RDFIntegrator::RDFIntegrator(const RDFIntegrator& integrator)
		: rdf_(integrator.rdf_)
	{
	}
	

	RDFIntegrator::RDFIntegrator(const RadialDistributionFunction& rdf)
		: rdf_(rdf)
	{
	}


	RDFIntegrator::~RDFIntegrator()
	{
		destroy();
	}


	void RDFIntegrator::destroy()
	{
		clear();
	}


	void RDFIntegrator::clear()
	{
		rdf_.clear();
	}


	void RDFIntegrator::set(const RDFIntegrator& integrator)
	{
		rdf_ = integrator.rdf_;
	}


	void RDFIntegrator::setRDF(const RadialDistributionFunction& rdf)
	{
		rdf_ = rdf;
	}


	RadialDistributionFunction& RDFIntegrator::getRDF() 
	{
		return rdf_;
	}


	const RDFIntegrator& RDFIntegrator::operator = 
		(const RDFIntegrator& integrator)
	{
		set(integrator);
		return *this;
	}


	double RDFIntegrator::operator () (double /* x */) const
	{
		Log.error() << "RDFIntegrator::operator () (double): "
			<< "This method should not be called!" << endl;
		return 0.0;
	}
}
