// $Id: reissCavFreeEnergyProcessor.C,v 1.6 2000/10/23 10:24:52 anker Exp $

#include <BALL/SOLVATION/reissCavFreeEnergyProcessor.h>
#include <BALL/STRUCTURE/numericalSAS.h>

using namespace std;

namespace BALL
{

	const char* ReissCavFreeEnergyProcessor::Option::VERBOSITY = "verbosity";
	const char* ReissCavFreeEnergyProcessor::Option::SOLVENT_NUMBER_DENSITY 
		= "solvent_number_density";
	const char* ReissCavFreeEnergyProcessor::Option::PRESSURE = "pressure";
	const char* ReissCavFreeEnergyProcessor::Option::ABSOLUTE_TEMPERATURE 
		= "absolute_temperature";
	const char* ReissCavFreeEnergyProcessor::Option::PROBE_RADIUS 
		= "probe_radius";
	
	const int ReissCavFreeEnergyProcessor::Default::VERBOSITY = 0;
	const float ReissCavFreeEnergyProcessor::Default::SOLVENT_NUMBER_DENSITY 
		= 3.33253e-2;
	const float ReissCavFreeEnergyProcessor::Default::PRESSURE = 1.01325e5;
	const float ReissCavFreeEnergyProcessor::Default::ABSOLUTE_TEMPERATURE 
		= 298.0;
	const float ReissCavFreeEnergyProcessor::Default::PROBE_RADIUS = 1.385;


	ReissCavFreeEnergyProcessor::ReissCavFreeEnergyProcessor() throw()
		: EnergyProcessor()
	{
		setDefaultOptions();

		valid_ = true;
	}


	ReissCavFreeEnergyProcessor::ReissCavFreeEnergyProcessor
		(const ReissCavFreeEnergyProcessor& proc) throw()
		: EnergyProcessor(proc)
	{
	}


	ReissCavFreeEnergyProcessor::~ReissCavFreeEnergyProcessor() throw()
	{
		clear();

		valid_ = false;
	}


	void ReissCavFreeEnergyProcessor::clear() throw()
	{
		EnergyProcessor::clear();
		setDefaultOptions();

		valid_ = true;
	}


	bool ReissCavFreeEnergyProcessor::finish() throw()
	{

		// first check for user settings

		int verbosity = (int) options.getInteger(Option::VERBOSITY);
		// rho is the number density of the solvent (i. e. water) [1/m^3]
		double rho = options.getReal(Option::SOLVENT_NUMBER_DENSITY) * 1e30;
		// the pressure [ Pa ]
		double P = options.getReal(Option::PRESSURE);
		// the temperature [ K ]
		double T = options.getReal(Option::ABSOLUTE_TEMPERATURE);
		// the solvent radius [ A ]
		double solvent_radius = options.getReal(Option::PROBE_RADIUS);
		
		// now compute some constant terms (names as in Pierotti, Chem. Rev.
		// 76(6):717--726, 1976)

		double sigma1 = 2 * solvent_radius * 1e-10; // [ m ]
		double sigma1_2 = sigma1 * sigma1; // [ m^2 ]
		double sigma1_3 = sigma1 * sigma1 * sigma1; // [ m^3 ]
		double y = Constants::PI * sigma1_3 * (rho/6);	// [ 1 ]
		double y_frac = y/(1-y); // [ 1 ]
		double y_frac_2 = y_frac * y_frac; // [ 1 ]
		double NkT = Constants::AVOGADRO * Constants::BOLTZMANN * T; // [ J/mol ]
		double NpiP = Constants::AVOGADRO * Constants::PI * P; // [ ? ]

		if (verbosity > 0)
		{
			Log.info() << "y = " << y << endl;
			Log.info() << "y_frac = " << y_frac << endl;
		}

		HashMap<Atom*,float> atom_areas;
		calculateSASAtomAreas(*fragment_, atom_areas, solvent_radius);
		
		// R is the sum of atom radius and probe radius [ m ]
		double R; 
		// deltaGspher is the cavitatonal energy of a spherical solute [ J/mol ]
		double deltaGspher; 
		// deltaGcav is the cavitatonal energy of the molecule [ J/mol ]
		double deltaGcav = 0; 

		// now iterate over the atoms.

		HashMap<Atom*,float>::Iterator it = atom_areas.begin();
		for (; +it; ++it)
		{
			R = it->first->getRadius() * 1e-10 + sigma1 / 2.0;
			deltaGspher =	
				  NkT * (-log(1.0 - y) + 4.5 * y_frac_2) - (NpiP * sigma1_3 / 6.0)
				- (NkT * ((6.0 * y_frac + 18 * y_frac_2) / sigma1) 
						+ (NpiP * sigma1_2)) * R
				+ (NkT * ((12.0 * y_frac + 18 * y_frac_2) / sigma1_2) 
						- (2.0 * NpiP * sigma1)) * (R * R)
				+ 4.0 / 3.0 * NpiP * (R * R * R);
			deltaGcav += it->second * 1e-20 /
				( 4 * Constants::PI * R * R ) * deltaGspher;
		}
		// return energy in units of kJ/mol
		energy_ = deltaGcav/1000;
		return 1;
	}
		
	
	void ReissCavFreeEnergyProcessor::setDefaultOptions() throw()
	{
		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		options.setDefaultReal(Option::SOLVENT_NUMBER_DENSITY, 
				Default::SOLVENT_NUMBER_DENSITY);
		options.setDefaultReal(Option::PRESSURE, Default::PRESSURE);
		options.setDefaultReal(Option::ABSOLUTE_TEMPERATURE,
				Default::ABSOLUTE_TEMPERATURE);
		options.setDefaultReal(Option::PROBE_RADIUS, Default::PROBE_RADIUS);
	}


} // namespace BALL
