// $Id: PCMCavFreeEnergyProcessor.C,v 1.9 2001/09/24 07:37:34 aubertin Exp $

#include <BALL/SOLVATION/PCMCavFreeEnergyProcessor.h>
#include <BALL/STRUCTURE/numericalSAS.h>
#include <BALL/STRUCTURE/analyticalSES.h>

using namespace std;

namespace BALL
{

	const char* PCMCavFreeEnergyProcessor::Option::VERBOSITY = "verbosity";
	const char* PCMCavFreeEnergyProcessor::Option::SOLVENT_NUMBER_DENSITY 
		= "solvent_number_density";
	const char* PCMCavFreeEnergyProcessor::Option::ABSOLUTE_TEMPERATURE 
		= "absolute_temperature";
	const char* PCMCavFreeEnergyProcessor::Option::PROBE_RADIUS 
		= "probe_radius";
	
	const int PCMCavFreeEnergyProcessor::Default::VERBOSITY = 0;
	const float PCMCavFreeEnergyProcessor::Default::SOLVENT_NUMBER_DENSITY 
		= 3.33253e-2;
	const float PCMCavFreeEnergyProcessor::Default::ABSOLUTE_TEMPERATURE 
		= 298.0;
	const float PCMCavFreeEnergyProcessor::Default::PROBE_RADIUS = 1.385;

	PCMCavFreeEnergyProcessor::PCMCavFreeEnergyProcessor() throw()
		:	EnergyProcessor()
	{
		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		options.setDefaultReal(Option::SOLVENT_NUMBER_DENSITY, 
				Default::SOLVENT_NUMBER_DENSITY);
		options.setDefaultReal(Option::ABSOLUTE_TEMPERATURE,
				Default::ABSOLUTE_TEMPERATURE);
		options.setDefaultReal(Option::PROBE_RADIUS, Default::PROBE_RADIUS);
		valid_ = true;
	}

	PCMCavFreeEnergyProcessor::PCMCavFreeEnergyProcessor
	(const PCMCavFreeEnergyProcessor& proc) throw()
		: EnergyProcessor(proc)
	{
	}

	
	PCMCavFreeEnergyProcessor::~PCMCavFreeEnergyProcessor() throw()
	{
		clear();

		valid_ = false;
	}


	void PCMCavFreeEnergyProcessor::clear() throw()
	{
		options.clear();

		valid_ = true;
	}

        const PCMCavFreeEnergyProcessor& PCMCavFreeEnergyProcessor::operator = (const PCMCavFreeEnergyProcessor& proc) throw()     
        {
	         valid_=proc.valid_;
                 energy_=proc.energy_;
                 fragment_=proc.fragment_;  
                 return *this;
        }

        bool PCMCavFreeEnergyProcessor::operator == (const PCMCavFreeEnergyProcessor& proc) const throw()
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
				result = ((*fragment_ == *proc.fragment_) 
						&& (energy_ 	 == proc.energy_)
						&& (valid_ 	 == proc.valid_));
			}
		}
		return result;
	}
	
	bool PCMCavFreeEnergyProcessor::finish() throw()
	{
		// first check for user settings

		int verbosity = (int) options.getInteger(Option::VERBOSITY);
		// rho is the number density of the solvent (i. e. water) [1/m^3]
		double rho = options.getReal(Option::SOLVENT_NUMBER_DENSITY) * 1e30;
		// the temperature [ K ]
		double T = options.getReal(Option::ABSOLUTE_TEMPERATURE);
		// the solvent radius [ A ]
		double solvent_radius = options.getReal(Option::PROBE_RADIUS);
		if (verbosity > 0)
		{
			Log.info() << "Using a probe radius of " << solvent_radius << " A" <<
				endl;
		}
		
		// now compute some constant terms (names as in Pierotti, Chem. Rev.
		// 76(6):717--726, 1976)

		double sigma1 = 2 * solvent_radius * 1e-10; // [ m ]
		double sigma1_3 = sigma1 * sigma1 * sigma1; // [ m^3 ]
		double y = Constants::PI * sigma1_3 * (rho/6);	// [ 1 ]
		double y_frac = y/(1-y); // [ 1 ]
		double y_frac_2 = y_frac * y_frac; // [ 1 ]
		double NkT = Constants::AVOGADRO * Constants::BOLTZMANN * T; // [ J/mol ]
		if (verbosity > 0)
		{
			Log.info() << "y = " << y << endl;
			Log.info() << "y_frac = " << y_frac << endl;
		}
		
		HashMap<const Atom*,float> atom_areas;
		calculateSASAtomAreas(*fragment_, atom_areas, solvent_radius);
		HashMap<const Atom*,float> atom_areas_reduced;
		// BAUSTELLE: Hier stand vorher SES? Warum?
		calculateSASAtomAreas(*fragment_, atom_areas_reduced, 0.0);
		
		// R is two times ( atom radius + probe radius ) [ m ]
		double R; 
		// S is atom radius + probe radius;
		double S; // [ 1 ]
		// deltaGspher is the cavitatonal energy of a spherical solute [ J/mol ]
		double deltaGspher; 
		// deltaGcav is the cavitatonal energy of the molecule [ J/mol ]
		double deltaGcav = 0; 

		HashMap<const Atom*,float>::Iterator it = atom_areas.begin();
		HashMap<const Atom*,float>::Iterator it_red = atom_areas_reduced.begin();

		for (; +it; ++it, ++it_red)
		{
			// R = 2 * it->first->getRadius() * 1e-10 / sigma1;
			S = it->first->getRadius() * 1e-10 + sigma1 / 2.0;
			R = 2 * S;

			deltaGspher =	
					- log(1.0 - y) + 4.5 * y_frac_2
					- ( ( 6.0 * y_frac + 18 * y_frac_2 ) / sigma1 ) * S
					+ ( ( 12.0 * y_frac + 18 * y_frac_2 ) / (sigma1 * sigma1) ) * S * S;
			deltaGspher *= NkT;

			R = it->first->getRadius() * 1e-10;
			deltaGcav += it_red->second * 1e-20 / 
				( 4 * Constants::PI * R * R ) * deltaGspher;
		}

		// return energy in junits of kJ/mol
		energy_ = deltaGcav/1000;
		return 1;
	}
} // namespace BALL
