// $Id: uhligCavFreeEnergyProcessor.h,v 1.6 2001/02/03 15:25:59 amoll Exp $

#ifndef BALL_SOLVATION_UHLIGCAVFREEENERGYPROCESSOR_H
#define BALL_SOLVATION_UHLIGCAVFREEENERGYPROCESSOR_H

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
		This processor is using the method proposed by Simonson/Bruenger, J.
		Phys. Chem. 98:4683--4694, 1994 which is based on Uhlig, J. Phys. Chem.
		41(9):1215--1225, 1937.
		{\bf Definition:} \URL{BALL/.../UhligCavFreeEnergyProcessor.h}
	 */

	class UhligCavFreeEnergyProcessor
		:	public EnergyProcessor
	{
		
		public:

		/** Symbolic names for option keys.
			This struct contains a symbolic name for each recognized key in
			UhligCavFreeEnergyProcessor::options.
		 */
		struct Option
		{
			/** The verbosity level.
				Use integer values with this option.
				@see Default::VERBOSITY
				@param verbosity integer
			 */
			static const char* VERBOSITY;

			/** The probe radius.
				This option defines the hard sphere solvent radius needed for the
				calculation of the cavitation free energy according to the scaled
				particle theory. Use float values of unit $ A $ with this option.
				@see Default::PROBE_RADIUS
				@param probe_radius float;
			 */
			static const char* PROBE_RADIUS;

			/** The surface tension.
				This option sets the surface tension needed for the calculation of
				the cavitation free energy. Use float values of unit
				$\mathrm{J}\cdot\mathrm{mol}^{-1}\cdot A^{-2}$ with this option.
				@see Default::SURFACE_TENSION
				@param surface_tension float;
			 */
			static const char* SURFACE_TENSION;

			/** The additive constant.
				This option represents an adjustable constant $C$ added to the
				$\gamma A$ term. Use float values of unit
				$\mathrm{J}\cdot\mathrm{mol}^{-1}$ with this option.
				@see Default::CONSTANT
				@param constant float;
			 */
			static const char* CONSTANT;

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

			/** Default probe radius.
				This probe radius is the one suggested by Reiss et al. in their
				paper (1.385 $ A $).
				@see Option::PROBE_RADIUS
			 */
			static const float PROBE_RADIUS;

			/** Default surface tension.
				This is a surface tension obtained by fitting calculated solvation free
				energies of several small solutes against experimantel data. The
				value ($5.41 \mathrm{cal}\cdot\mathrm{mol}^{-1}\cdot A^{-2} =
				22.635 \mathrm{J}\cdot\mathrm{mol}^{-1}\cdot A^{-2}$) slightly
				differs from the value suggested by Simonson/Br"unger in their
				paper ($6 \mathrm{cal}\cdot\mathrm{mol}^{-1}\cdot A^{-2} = 25.1
				\mathrm{J}\cdot\mathrm{mol}^{-1}\cdot A^{-2}$).
				@see Option::SURFACE_TENSION
			 */
			static const float SURFACE_TENSION;

			/** Default additive constant.
				This additive constant was also found by fitting aginst experimental
				data (@see Default::SURFACE_TENSION). In contrast to
				Simonson/Br"unger it is not zero but $0.921
				\mathrm{kcal}\cdot\mathrm{mol}^{-1} = 3.855
				\mathrm{kJ}\cdot\mathrm{mol}^{-1}$.
				@see Option::CONSTANT
			 */
			static const float CONSTANT;

		};

		/** @name Constructors and Destructors */
		//@{

		/** Default constructor */
		UhligCavFreeEnergyProcessor() throw();

		/** Copy constructor */
		UhligCavFreeEnergyProcessor(const UhligCavFreeEnergyProcessor& proc)
			throw();

		/** Destructor */
		virtual ~UhligCavFreeEnergyProcessor() throw();

		//@}


		/** @name Assignment */
		//@{

		/** Assignment operator */
		const UhligCavFreeEnergyProcessor& operator = 
			(const UhligCavFreeEnergyProcessor& proc) throw();

		/** Clear function */
		virtual void clear() throw();

		//@}


		/** @name Predicates */
		//@{

		/** Equality operator */
		bool operator == (const UhligCavFreeEnergyProcessor& proc) const
			throw();

		//@}


		/** @name Processor functions */
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

#endif // BALL_SOLVATION_UHLIGCAVFREEENERGYPROCESSOR_H
