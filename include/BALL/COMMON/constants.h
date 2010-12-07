// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_COMMON_CONSTANTS_H
#define BALL_COMMON_CONSTANTS_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

namespace BALL 
{

	/**	The constants namespace.
			This namespace contains definitions for some basic mathematical and physical constants.
			All constants are double precision. \par
			There are basically two ways of accessing these constants:
			
			- specify all namespaces: \par
			<tt>float my_pi = BALL::Constants::PI</tt>
			- shortcut via the <tt>using directive</tt>: \par
			<tt>using namespace BALL::Constants;  \par
			float my_pi = PI;</tt>
			
	  	\ingroup Common
	*/
	namespace Constants 
	{
		/**	@name	Mathematical constants.
		*/
		//@{

		/// PI
		BALL_EXTERN_VARIABLE const double  PI;

		/// Euler's number - base of the natural logarithm
		BALL_EXTERN_VARIABLE const double  E;

		/**	Internal theshold for equality comparisons.
				Default value is 1e-6.
		*/
		BALL_EXTERN_VARIABLE double EPSILON;
		//@}
			
		/**	@name Chemical/physical constants.
		*/
		//@{
		
		/**	Elementary charge.
			  In units of C (\f$1.60217738 \cdot 10^{-19} C\f$).
		*/
		BALL_EXTERN_VARIABLE const double	ELEMENTARY_CHARGE; // C
	
		/// Elementary charge (alias)
		BALL_EXTERN_VARIABLE const double	e0;

		/** Electron mass.
				In units of kg (\f$9.1093897 \cdot 10^{-31}\f$ kg).
		*/
		BALL_EXTERN_VARIABLE const double	ELECTRON_MASS;	 // kg

		/** Proton mass.
				In units of kg (\f$1.6726230 \cdot 10^{-27}\f$ kg).
		*/
		BALL_EXTERN_VARIABLE const double	PROTON_MASS;	 // kg

		/** Neutron mass.
				In units of kg (\f$1.6749286 \cdot 10^{-27}\f$ kg).
		*/
		BALL_EXTERN_VARIABLE const double	NEUTRON_MASS;	 // kg

		/** Avogadro constant.
				In units of \f$mol^{-1}\f$ (\f$6.0221367 \cdot 10^{23} mol^{-1}\f$).
		*/
		BALL_EXTERN_VARIABLE const double	AVOGADRO;

		/** Avogadro constant (alias)
		*/
		BALL_EXTERN_VARIABLE const double	NA;

		/** Avogadro constant (alias)
		*/
		BALL_EXTERN_VARIABLE const double	MOL;

		/** Boltzmann constant.
				In units of J/K (\f$1.380657 \cdot 10^{-23}\f$ J/K).
		*/
		BALL_EXTERN_VARIABLE const double	BOLTZMANN;

		/** Boltzmann constant (alias)
		*/
		BALL_EXTERN_VARIABLE const double	k;
		
		/** Planck constant.
				In units of Js (\f$6.6260754 \cdot 10^{-34}\f$ Js).
		*/
		BALL_EXTERN_VARIABLE const double	PLANCK;

		/** Planck constant (alias)
		*/
		BALL_EXTERN_VARIABLE const double	h;

		/** Gas constant (= NA * k)	
		*/
		BALL_EXTERN_VARIABLE const double	GAS_CONSTANT;

		/** Gas constant (alias)
		*/
		BALL_EXTERN_VARIABLE const double R;

		/** Faraday constant (= NA * e0)
		*/
		BALL_EXTERN_VARIABLE const double	FARADAY;

		/** Faraday constant (alias)
		*/
		BALL_EXTERN_VARIABLE const double	F;

		/** Bohr radius.
				In units m (\f$5.29177249 \cdot 10^{-11}\f$ m).
		*/
		BALL_EXTERN_VARIABLE const double	BOHR_RADIUS;

		/** Bohr radius (alias)
		*/
		BALL_EXTERN_VARIABLE const double	a0;

		//  the following values from: 
		//  P.W.Atkins: Physical Chemistry, 5th ed., Oxford University Press, 1995

		/** Vacuum permittivity.
				In units of \f$C^2J^{-1}m^{-1}\f$ (\f$8.85419 \cdot 10^{-12} C^2J^{-1}m^{-1}\f$).
		*/
		BALL_EXTERN_VARIABLE const double	VACUUM_PERMITTIVITY;

		/** Vacuum permeability.
				In units of \f$Js^2C^{-2}m^{-1}\f$ (\f$4\pi \cdot 10^{-7} Js^2C^{-2}m^{-1}\f$).
		*/
		BALL_EXTERN_VARIABLE const double	VACUUM_PERMEABILITY;

		/** Speed of light.
				In units of m/s (\f$2.99792458 \cdot 10^8 ms^{-1}\f$).
		*/
		BALL_EXTERN_VARIABLE const double	SPEED_OF_LIGHT;

		/** Speed of Light (alias)
		*/
		BALL_EXTERN_VARIABLE const double	c;

		/** Gravitational constant.
				In units of \f$Nm^2kg^{-2}\f$ (\f$6.67259 \cdot 10^{-11} Nm^2kg^{-2}\f$).
		*/
		BALL_EXTERN_VARIABLE const double	GRAVITATIONAL_CONSTANT;

		/** Fine structure constant.
				Without unit (\f$7.29735 \cdot 10^{-3}\f$).
		*/
		BALL_EXTERN_VARIABLE const double	FINE_STRUCTURE_CONSTANT;
		//@}

		/**	@name	Conversion factors
		*/
		//@{		
			
		/** Degree per rad.
				57.2957795130823209
		*/
		BALL_EXTERN_VARIABLE const double	DEG_PER_RAD;

		/** Rad per degree.
				0.0174532925199432957
		*/
		BALL_EXTERN_VARIABLE const double	RAD_PER_DEG;

		/** mm per inch.
				25.4
		*/
		BALL_EXTERN_VARIABLE const double	MM_PER_INCH;

		/** m per foot.
				3.048
		*/
		BALL_EXTERN_VARIABLE const double	M_PER_FOOT;

		/** Joules per calorie.
				4.184
		*/
		BALL_EXTERN_VARIABLE const double	JOULE_PER_CAL;

		/** Calories per Joule.
				1/JOULE_PER_CAL
		*/
		BALL_EXTERN_VARIABLE const double	CAL_PER_JOULE;

		//@}
	}
}

#endif // BALL_COMMON_CONSTANTS_H
