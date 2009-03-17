// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PCMCavFreeEnergyProcessor.h,v 1.18 2005/12/23 17:01:59 amoll Exp $
//

#ifndef BALL_SOLVATION_PCMCAVFREEENERGYPROCESSOR_H
#define BALL_SOLVATION_PCMCAVFREEENERGYPROCESSOR_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_ENERGY_ENERGYPROCESSOR_H
# include <BALL/ENERGY/energyProcessor.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

namespace BALL
{
	/** Processor for the computation of the cavitation free energy. 
			This processor is using the SPT theory approach implemented in the PCM
			model. It is based on Pierotti, Chem. Rev. 76(6):717--726, 1976 and the
			modification for non-spherical solutes by Huron/Claverie, J. Phys.
			Chem. 76(15):2123-2133, 1972.	 \par
			The energy value is returned in units of kJ/mol.  \par
			\ingroup Solvation
	 */
	class BALL_EXPORT PCMCavFreeEnergyProcessor
		:	public EnergyProcessor
	{
		
		public:

		/** Symbolic names for option keys.
				This struct contains a symbolic name for each recognized key in
				PCMCavFreeEnergyProcessor::options.
		 */
		struct BALL_EXPORT Option
		{
			/** The verbosity level.
					Use integer values with this option.
					@see Default::VERBOSITY
					@param verbosity integer
			 */
			static const char* VERBOSITY;

			/** The number density of the solvent.
					This option defines the number density of the surrounding solvent. Use
					float values of unit $ A^{-3} $ with this option.
					@see Default::SOLVENT_NUMBER_DENSITY
					@param solvent_number_density float
			 */
			static const char* SOLVENT_NUMBER_DENSITY;

			/** The temperature.
					This option defines the absolute temperature at which the solvation
					takes place. Use float values of unit K with this option.
					@see Default::ABSOLUTE_TEMPERATURE
					@param absolute_temperature float;
			 */
			static const char* ABSOLUTE_TEMPERATURE;

			/** The probe radius.
					This option defines the hard sphere solvent radius needed for the
					calculation of the cavitation free energy according to the scaled
					particle theory. Use float values of unit $ A $ with this option.
					@see Default::PROBE_RADIUS
					@param probe_radius float;
			 */
			static const char* PROBE_RADIUS;
		};

		/** Default values for cavitation free energy calculations.
				These values represent the default settings for the calculations of the
				cavitation free energy.
		 */
		struct BALL_EXPORT Default
		{
			/** Default verbosity level.
					@see Option::VERBOSITY
			 */
			static const int VERBOSITY;

			/** Default number density.
					This default value is the number density of water at 300 K and
					standard pressure (3.33253e-2 $ A^{-3}$).
					@see Option::SOLVENT_NUMBER_DENSITY;
			 */
			static const float SOLVENT_NUMBER_DENSITY;

			/** Default temperature.
					We use a standard temperature of 298 K (25 degrees Celsius).
					@see Option::ABSOLUTE_TEMPERATURE
			 */
			static const float ABSOLUTE_TEMPERATURE;

			/** Default probe radius.
					This probe radius is the one suggested by PCM et al. in their
					paper (1.385 $ A $).
					@see Option::PROBE_RADIUS
			 */
			static const float PROBE_RADIUS;
		};

		/** @name Constructors and Destructors 
		*/
		//@{

		/** Default constructor 
		*/
		PCMCavFreeEnergyProcessor();

		/** Copy constructor 
		*/
		PCMCavFreeEnergyProcessor(const PCMCavFreeEnergyProcessor& proc);

		/** Destructor 
		*/
		virtual ~PCMCavFreeEnergyProcessor();

		//@}
		/** @name Assignment 
		*/
		//@{

		/** Assignment operator 
		*/
		const PCMCavFreeEnergyProcessor& operator = (const PCMCavFreeEnergyProcessor& proc);

		/** Clear function 
		*/
		virtual void clear();

		//@}
		/** @name Predicates 
		*/
		//@{

		/** Equality operator 
		*/
		bool operator == (const PCMCavFreeEnergyProcessor& proc) const;

		//@}
		/** @name processor functions 
		*/
		//@{

		/** This is where the actual computation takes place. 
		*/
		virtual bool finish();

		//@}
		/** @name Options 
		*/
		//@{

		/** Options for the calculation of the caviation free energy 
		*/
		Options options;

		//@}

	};
   
}

#endif // BALL_SOLVATION_PCMCAVFREEENERGYPROCESSOR_H
