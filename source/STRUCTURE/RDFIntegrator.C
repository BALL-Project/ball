// $Id: RDFIntegrator.C,v 1.3 2000/09/01 06:09:46 oliver Exp $

#include <BALL/STRUCTURE/RDFIntegrator.h>

using namespace std;

namespace BALL
{
	RDFIntegrator::RDFIntegrator()
		:	RadialDistributionFunction()
	{
	}


	RDFIntegrator::RDFIntegrator(const RDFIntegrator& integrator)
		:	RadialDistributionFunction(integrator)
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
		RadialDistributionFunction::clear();
	}


	void RDFIntegrator::set(const RDFIntegrator& integrator)
	{
		RadialDistributionFunction::set(integrator);
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
