// $Id: constants.h,v 1.6 2000/05/15 19:11:50 oliver Exp $

#ifndef BALL_COMMON_CONSTANTS_H
#define BALL_COMMON_CONSTANTS_H

namespace BALL 
{

	namespace Constants 
	{

		/**	@name The constants namespace.
				This namespace contains definitions for some basic mathematical and physical constants.
				All constants are double precision.\\
				There are basically two ways of accessing these constants:
				\begin{itemize}
					\item specify all namespaces:\\
							{\tt float my_pi = BALL::Constants::PI}
					\item shortcut via the {\tt using directive}:\\
							{\tt using namespace BALL::Constants; float my_pi = PI;}
				\end{itemize}
				{\bf Definition:} \URL{BALL/COMMON/constants.h}
		*/
		//@{

		/**	@name	Mathematical constants
				All constants are double precision.\\
				There are basically two ways of accessing these constants:
				\begin{itemize}
					\item specify all namespaces:\\
							{\tt float my_pi = BALL::Constants::PI}
					\item shortcut via the {\tt using directive}:\\
							{\tt using namespace BALL::Constants; float my_pi = PI;}
				\end{itemize}
				{\bf Definition:} \URL{BALL/COMMON/constants.h}
		*/
		//@{

		/// PI
		extern const double  PI;

		/// Euler's number - base of the natural logarithm
		extern const double  E;

		/**	Internal theshold for equality comparisons.
				Default value is 1e-6.
		*/
		extern double EPSILON;
		//@}
			
		/**	@name Chemical/physical constants
				All constants are double precision.\\
				There are basically two ways of accessing these constants:
				\begin{itemize}
					\item specify all namespaces:\\
							{\tt float my_pi = BALL::Constants::PI}
					\item shortcut via the {\tt using directive}:\\
							{\tt using namespace BALL::Constants; float my_pi = PI;}
				\end{itemize}
				{\bf Definition:} \URL{BALL/COMMON/constants.h}
		*/
		//@{
		
		/**	Elementary charge.
			  In units of C ($1.60217738 \cdot 10^{-19} C$).
		*/
		extern const double	ELEMENTARY_CHARGE;  	 // C     
	
		/// Elementary charge (alias)
		extern const double	e0;

		/** Electron mass.
				In units of kg ($9.1093897 \cdot 10^{-31}$ kg).
		*/
		extern const double	ELECTRON_MASS   	;   	 // kg

		/** Proton mass.
				In units of kg ($1.6726230 \cdot 10^{-27}$ kg).
		*/
		extern const double	PROTON_MASS     	;   	 // kg

		/** Neutron mass.
				In units of kg ($1.6749286 \cdot 10^{-27}$ kg).
		*/
		extern const double	NEUTRON_MASS    	;   	 // kg

		/** Avogadro constant.
				In units of $mol^{-1}$ ($6.0221367 \cdot 10^{23} mol^{-1}$).
		*/
		extern const double	AVOGADRO;

		/** Avogadro constant (alias)
		*/
		extern const double	NA;

		/** Avogadro constant (alias)
		*/
		extern const double	MOL;

		/** Boltzmann constant.
				In units of J/K ($1.380657 \cdot 10^{-23}$ J/K).
		*/
		extern const double	BOLTZMANN;

		/** Boltzmann constant (alias)
		*/
		extern const double	k;
		
		/** Planck constant.
				In units of Js ($6.6260754 \cdot 10^{-34}$ Js).
		*/
		extern const double	PLANCK;

		/** Planck constant (alias)
		*/
		extern const double	h;

		/** Gas constant (= NA * k)	
		*/
		extern const double	GAS_CONSTANT;

		/** Gas constant (alias)
		*/
		extern const double R;

		/** Faraday constant (= NA * e0)
		*/
		extern const double	FARADAY;

		/** Faraday constant (alias)
		*/
		extern const double	F;

		/** Bohr radius.
				In units m ($5.29177249 \cdot 10^{-11}$ m).
		*/
		extern const double	BOHR_RADIUS;

		/** Bohr radius (alias)
		*/
		extern const double	a0;

		//  the following values from: 
		//  P.W.Atkins: Physical Chemistry, 5th ed., Oxford University Press, 1995

		/** Vacuum permittivity.
				In units of $C^2J^{-1}m^{-1}$ ($8.85419 \cdot 10^{-12} C^2J^{-1}m^{-1}$).
		*/
		extern const double	VACUUM_PERMITTIVITY;

		/** Vacuum permeability.
				In units of $Js^2C^{-2}m^{-1}$ ($4\pi \cdot 10^{-7} Js^2C^{-2}m^{-1}$).
		*/
		extern const double	VACUUM_PERMEABILITY;

		/** Speed of light.
				In units of m/s ($2.99792458 \cdot 10^8 ms^{-1}$).
		*/
		extern const double	SPEED_OF_LIGHT;

		/** Speed of Light (alias)
		*/
		extern const double	c;

		/** Gravitational constant.
				In units of $Nm^2kg^{-2}$ ($6.67259 \cdot 10^{-11} Nm^2kg^{-2}$).
		*/
		extern const double	GRAVITATIONAL_CONSTANT;

		/** Fine structure constant.
				Without unit ($7.29735 \cdot 10^{-3}$).
		*/
		extern const double	FINE_STRUCTURE_CONSTANT;
		//@}

		/**	@name	Conversion factors
				All constants are double precision.\\
				There are basically two ways of accessing these constants:
				\begin{itemize}
					\item specify all namespaces:\\
							{\tt float my_pi = BALL::Constants::PI}
					\item shortcut via the {\tt using directive}:\\
							{\tt using namespace BALL::Constants; float my_pi = PI;}
				\end{itemize}
				{\bf Definition:} \URL{BALL/COMMON/constants.h}
		*/
		//@{		
			
		/** Degree per rad.
				57.2957795130823209
		*/
		extern const double	DEG_PER_RAD;

		/** Rad per degree.
				0.0174532925199432957
		*/
		extern const double	RAD_PER_DEG;

		/** mm per inch.
				25.4
		*/
		extern const double	MM_PER_INCH 			;

		/** m per foot.
				3.048
		*/
		extern const double	M_PER_FOOT  			;

		/** Joules per calorie.
				4.184
		*/
		extern const double	JOULE_PER_CAL;

		/** Calories per Joule.
				1/JOULE_PER_CAL
		*/
		extern const double	CAL_PER_JOULE;

		//@}

		//@}

	}
}

#endif // BALL_COMMON_CONSTANTS_H
