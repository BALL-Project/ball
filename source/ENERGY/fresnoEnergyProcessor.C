// $Id: fresnoEnergyProcessor.C,v 1.1 2002/01/28 10:43:29 anker Exp $

#include <BALL/ENERGY/fresnoEnergyProcessor.h>

using namespace std;

namespace BALL
{

	const char* FresnoEnergyProcessor::Option::VERBOSITY = "verbosity";


	const Size FresnoEnergyProcessor::Default::VERBOSITY = 1;


	FresnoEnergyProcessor::FresnoEnergyProcessor()
		throw()
		:	EnergyProcessor(),
			coefficients_()
	{
		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
	}


	FresnoEnergyProcessor::FresnoEnergyProcessor
		(const FresnoEnergyProcessor& proc)
		throw()
		:	EnergyProcessor(proc),
			coefficients_(proc.coefficients_)
	{
		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
	}


	FresnoEnergyProcessor::~FresnoEnergyProcessor()
		throw()
	{
	}


	void FresnoEnergyProcessor::setCoefficients(const ::vector<double>& coefficients)
		throw()
	{
		if (coefficients.size() != 6)
		{
			throw Exception::InvalidSize(__FILE__, __LINE__, 6);
		}
		else
		{
			coefficients_ = coefficients;
		}
	}


	const ::vector<double>& FresnoEnergyProcessor::getCoefficients() const
		throw()
	{
		return coefficients_;
	}


	const FresnoEnergyProcessor& FresnoEnergyProcessor::operator = 
		(const FresnoEnergyProcessor& proc)
		throw()
	{
		EnergyProcessor::operator = (proc);
		coefficients_ = proc.coefficients_;

		return *this;
	}


	bool FresnoEnergyProcessor::operator == (const FresnoEnergyProcessor& proc)
		throw()
	{
		return (EnergyProcessor::operator == (proc);
				&& (coefficients_ == proc.coefficients_))
	}


	void FresnoEnergyProcessor::clear()
		throw()
	{
		EnergyProcessor::clear();
		coefficients_.clear();
	}


	bool FresnoEnergyProcessor::finish()
		throw()
	{
		// TODO
		// Do I need a global view? Does a local view suffice?

		energy_ = coefficients_[0];

		return true;
	}

	double FresnoEnergyProcessor::calculateHelperFunction_
		(double x, double l, double u) const
		throw()
	{
		if (x <= l)
		{
			return 1.0;
		}
		else
		{
			if (x <= u)
			{
				return 1.0 - ((x - l)/(u - l));
			}
			else
			{
				return 0.0;
			}
		}
	}


	double FresnoEnergyProcessor::calculateHB_() const
		throw()
	{
	}




}
