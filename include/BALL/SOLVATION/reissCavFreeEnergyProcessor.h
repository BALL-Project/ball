// $Id: reissCavFreeEnergyProcessor.h,v 1.7 2001/02/03 15:25:59 amoll Exp $

#ifndef BALL_SOLVATION_REISSCAVFREEENERGYPROCESSOR_H
#define BALL_SOLVATION_REISSCAVFREEENERGYPROCESSOR_H

#ifndef BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_ENERGY_ENERGYPROCESSOR_H
#include <BALL/ENERGY/energyProcessor.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#include <BALL/DATATYPE/options.h>
#endif

namespace BALL
{
	/** Processor for the computation of the cavitation free energy. 
		This processor is using the SPT theory approach by Reiss et al.,
		J. Chem. Phys. 37(1):119-124, 1960 with the modification for
		non-spherical solutes by Huron/Claverie, J. Phys. Chem.
		76(15):2123-2133, 1972.
		\\ 
		Energies are returned in units of kJ/mol.
		\\
		{\bf Definition:} \URL{BALL/.../ReissCavFreeEnergyProcessor.h}
	 */

	class ReissCavFreeEnergyProcessor
		:	public EnergyProcessor
	{
		
		public:

		/** Symbolic names for option keys.
			This struct contains a symbolic name for each recognized key in
			ReissCavFreeEnergyProcessor::options.
		 */
		struct Option
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

			/** The pressure.
				With this option it is possible to define the pressure under which
				the solvation takes place. Use float values of unit Pa with this
				option.
				@see Default::PRESSURE
				@param pressure float
			 */
			static const char* PRESSURE;

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

		/** Default values for cavitation free anergy calculations.
		These values represent the default settings for the calculations of the
		cavitation free energy.
		 */
		struct Default
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

			/** Default pressure.
				This default value is the standard pressure (1.01325e5 Pa).
				@see Option::PRESSURE
			 */
			static const float PRESSURE;

			/** Default temperature.
				We use a standard temperature of 298 K (25 degrees Celsius).
				@see Option::ABSOLUTE_TEMPERATURE
			 */
			static const float ABSOLUTE_TEMPERATURE;

			/** Default probe radius.
				This probe radius is the one suggested by Reiss et al. in their
				paper (1.385 $ A $).
				@see Option::PROBE_RADIUS
			 */
			static const float PROBE_RADIUS;
		};


		/** @name Constructors and Destructors */
		//@{

		/** Default constructor */
		ReissCavFreeEnergyProcessor() throw();

		/** Copy constructor */
		ReissCavFreeEnergyProcessor(const ReissCavFreeEnergyProcessor& proc) throw();

		/** Destructor */
		virtual ~ReissCavFreeEnergyProcessor() throw();

		//@}


		/** @name Assignment */
		//@{

		/** Assignment operator */
		const ReissCavFreeEnergyProcessor& operator = 
			(const ReissCavFreeEnergyProcessor& proc) throw();

		/** Clear function */
		virtual void clear() throw();

		//@}


		/** @name Predicates */
		//@{

		/** Equality operator */
		bool operator == (const ReissCavFreeEnergyProcessor& proc) const throw();

		//@}


		/** @name processor functions */
		//@{

		/** This is where the actual computation takes place. */
		virtual bool finish() throw();

		//@}


		/** @name Options */
		//@{

		/** Options for the calculation of the caviation free energy */
		Options options;

		//@}


		private:

		void setDefaultOptions() throw();

	};
}

#endif // BALL_SOLVATION_REISSCAVFREEENERGYPROCESSOR_H
