// $Id: uhligCavFreeEnergyProcessor.C,v 1.2 2000/09/25 11:17:55 anker Exp $

#include <BALL/SOLVATION/uhligCavFreeEnergyProcessor.h>
#include <BALL/STRUCTURE/numericalSAS.h>

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
	{
		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		options.setDefaultReal(Option::PROBE_RADIUS, Default::PROBE_RADIUS);
		options.setDefaultReal(Option::SURFACE_TENSION, Default::SURFACE_TENSION);
		options.setDefaultReal(Option::CONSTANT, Default::CONSTANT);
	}

	UhligCavFreeEnergyProcessor::~UhligCavFreeEnergyProcessor()
	{
	}

	bool UhligCavFreeEnergyProcessor::finish()
	{

		// first check for user settings

		// for now, there is nothing to report
		//int verbosity = (int) options.getInteger(Option::VERBOSITY);
		// the solvent radius [ A ]
		double solvent_radius = options.getReal(Option::PROBE_RADIUS);
		// the surface tension [ J/mol/A^2 ]
		double gamma = options.getReal(Option::SURFACE_TENSION);
		// an additive constant [ J/mol ]
		double C = options.getReal(Option::CONSTANT);
		
		double A = calculateSASArea(*fragment_, solvent_radius);
		
		energy_ = gamma * A + C;
		return 1;
	}
} // namespace BALL
