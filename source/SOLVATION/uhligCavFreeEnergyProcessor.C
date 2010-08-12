// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: uhligCavFreeEnergyProcessor.C,v 1.11 2002/12/16 12:30:37 amoll Exp $

#include <BALL/SOLVATION/uhligCavFreeEnergyProcessor.h>
#include <BALL/STRUCTURE/numericalSAS.h>

using std::endl;

namespace BALL
{

	const char* UhligCavFreeEnergyProcessor::Option::VERBOSITY = "verbosity";
	const char* UhligCavFreeEnergyProcessor::Option::PROBE_RADIUS 
		= "probe_radius";
	const char* UhligCavFreeEnergyProcessor::Option::SURFACE_TENSION 
		= "surface_tension";
	const char* UhligCavFreeEnergyProcessor::Option::CONSTANT = "constant";

	const int UhligCavFreeEnergyProcessor::Default::VERBOSITY = 0;
	const float UhligCavFreeEnergyProcessor::Default::PROBE_RADIUS = 1.385;
	const float UhligCavFreeEnergyProcessor::Default::SURFACE_TENSION 
		= 22.63544;
	const float UhligCavFreeEnergyProcessor::Default::CONSTANT 
		= 3854.606;


	UhligCavFreeEnergyProcessor::UhligCavFreeEnergyProcessor()
		:	EnergyProcessor()
	{
		setDefaultOptions();

		valid_ = true;
	}


	UhligCavFreeEnergyProcessor::UhligCavFreeEnergyProcessor
	(const UhligCavFreeEnergyProcessor& proc)
		: EnergyProcessor(proc)
	{
	}


	UhligCavFreeEnergyProcessor::~UhligCavFreeEnergyProcessor()
	{
		clear();

		valid_ = false;
	}


	void UhligCavFreeEnergyProcessor::clear()
	{
		EnergyProcessor::clear();
		setDefaultOptions();

		valid_ = true;
	}

	const UhligCavFreeEnergyProcessor& UhligCavFreeEnergyProcessor::operator = (const UhligCavFreeEnergyProcessor& proc) 
	{
		valid_=proc.valid_;
		energy_=proc.energy_;
		fragment_=proc.fragment_;
		return *this;
	}

	bool UhligCavFreeEnergyProcessor::operator == (const UhligCavFreeEnergyProcessor& proc) const
	{
		bool result;
		if ((fragment_ == 0) && (proc.fragment_ == 0))
		{
			result = ((energy_ == proc.energy_) && (valid_ == proc.valid_));
		}
		else
		{
			if ((fragment_ == 0) || (proc.fragment_ == 0))
			{
				result = false;
			}
			else
			{
				result = ((*fragment_ == *proc.fragment_) && (energy_ 	 == proc.energy_) && (valid_ 	 == proc.valid_));
			}
		}
		return result;
	}

	bool UhligCavFreeEnergyProcessor::finish()
	{

		// first check for user settings

		// for now, there is nothing to report
		int verbosity = (int) options.getInteger(Option::VERBOSITY);
		// the solvent radius [ A ]
		double solvent_radius = options.getReal(Option::PROBE_RADIUS);
		// the surface tension [ J/mol/A^2 ]
		double gamma = options.getReal(Option::SURFACE_TENSION);
		// an additive constant [ J/mol ]
		double C = options.getReal(Option::CONSTANT);
		
		NumericalSAS sas_computer;
		sas_computer.options[NumericalSAS::Option::PROBE_RADIUS  ] = solvent_radius;
		sas_computer.options[NumericalSAS::Option::COMPUTE_VOLUME] = false;

		sas_computer(*fragment_);
		double A = sas_computer.getTotalArea();
		
		if (verbosity > 0)
		{
			Log.info() << "Uhlig parameters and calculated values:" << endl
			<< "solvent radius:  " << solvent_radius << endl
			<< "surface tension: " << gamma << endl
			<< "constant:        " << C << endl
			<< "SAS area:        " << A << endl;
		}

		// return energy in units of kJ/mol
		energy_ = (gamma * A + C)/1000; 
		return 1;
	}


	void UhligCavFreeEnergyProcessor::setDefaultOptions()
	{
		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		options.setDefaultReal(Option::PROBE_RADIUS, Default::PROBE_RADIUS);
		options.setDefaultReal(Option::SURFACE_TENSION, Default::SURFACE_TENSION);
		options.setDefaultReal(Option::CONSTANT, Default::CONSTANT);
	}

} // namespace BALL
