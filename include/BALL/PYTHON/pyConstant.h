// $Id: pyConstant.h,v 1.1 2000/06/27 22:07:27 oliver Exp $

#ifndef BALL_PYTHON_PYCONSTANT_H
#define BALL_PYTHON_PYCONSTANT_H

namespace BALL 
{
	/**	Python constants namespace wrapper.
			This class is a wrapper for the constants namespace.	
			\\
			{\bf Definition:}\URL{BALL/PYTHON/pyConstant.h}
	*/
	class Constant
	{
		public:
		
			// PI
			static double  PI;

			/// Euler's number - base of the natural logarithm
			static double  E;

			/**	Internal theshold for equality comparisons.
					Default value is 1e-6.
			*/
			static double EPSILON;
			/**	Elementary charge.
					In units of C ($1.60217738 \cdot 10^{-19} C$).
			*/
			static double	ELEMENTARY_CHARGE;  	 // C     
		
			/// Elementary charge (alias)
			static double	e0;

			/** Electron mass.
					In units of kg ($9.1093897 \cdot 10^{-31}$ kg).
			*/
			static double	ELECTRON_MASS   	;   	 // kg

			/** Proton mass.
					In units of kg ($1.6726230 \cdot 10^{-27}$ kg).
			*/
			static double	PROTON_MASS     	;   	 // kg

			/** Neutron mass.
					In units of kg ($1.6749286 \cdot 10^{-27}$ kg).
			*/
			static double	NEUTRON_MASS    	;   	 // kg

			/** Avogadro constant.
					In units of $mol^{-1}$ ($6.0221367 \cdot 10^{23} mol^{-1}$).
			*/
			static double	AVOGADRO;

			/** Avogadro constant (alias)
			*/
			static double	NA;

			/** Avogadro constant (alias)
			*/
			static double	MOL;

			/** Boltzmann constant.
					In units of J/K ($1.380657 \cdot 10^{-23}$ J/K).
			*/
			static double	BOLTZMANN;

			/** Boltzmann constant (alias)
			*/
			static double	k;
			
			/** Planck constant.
					In units of Js ($6.6260754 \cdot 10^{-34}$ Js).
			*/
			static double	PLANCK;

			/** Planck constant (alias)
			*/
			static double	h;

			/** Gas constant (= NA * k)	
			*/
			static double	GAS_CONSTANT;

			/** Gas constant (alias)
			*/
			static double R;

			/** Faraday constant (= NA * e0)
			*/
			static double	FARADAY;

			/** Faraday constant (alias)
			*/
			static double	F;

			/** Bohr radius.
					In units m ($5.29177249 \cdot 10^{-11}$ m).
			*/
			static double	BOHR_RADIUS;

			/** Bohr radius (alias)
			*/
			static double	a0;

			//  the following values from: 
			//  P.W.Atkins: Physical Chemistry, 5th ed., Oxford University Press, 1995

			/** Vacuum permittivity.
					In units of $C^2J^{-1}m^{-1}$ ($8.85419 \cdot 10^{-12} C^2J^{-1}m^{-1}$).
			*/
			static double	VACUUM_PERMITTIVITY;

			/** Vacuum permeability.
					In units of $Js^2C^{-2}m^{-1}$ ($4\pi \cdot 10^{-7} Js^2C^{-2}m^{-1}$).
			*/
			static double	VACUUM_PERMEABILITY;

			/** Speed of light.
					In units of m/s ($2.99792458 \cdot 10^8 ms^{-1}$).
			*/
			static double	SPEED_OF_LIGHT;

			/** Speed of Light (alias)
			*/
			static double	c;

			/** Gravitational constant.
					In units of $Nm^2kg^{-2}$ ($6.67259 \cdot 10^{-11} Nm^2kg^{-2}$).
			*/
			static double	GRAVITATIONAL_CONSTANT;

			/** Fine structure constant.
					Without unit ($7.29735 \cdot 10^{-3}$).
			*/
			static double	FINE_STRUCTURE_CONSTANT;
				
			/** Degree per rad.
					57.2957795130823209
			*/
			static double	DEG_PER_RAD;

			/** Rad per degree.
					0.0174532925199432957
			*/
			static double	RAD_PER_DEG;

			/** mm per inch.
					25.4
			*/
			static double	MM_PER_INCH 			;

			/** m per foot.
					3.048
			*/
			static double	M_PER_FOOT  			;

			/** Joules per calorie.
					4.184
			*/
			static double	JOULE_PER_CAL;

			/** Calories per Joule.
					1/JOULE_PER_CAL
			*/
			static double	CAL_PER_JOULE;
	};
} // namespace BALL

#endif // BALL_PYTHON_PYCONSTANT_H
