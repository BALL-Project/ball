// $Id: constants.h,v 1.4 2000/02/17 00:30:36 oliver Exp $

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
							{\tt float my\_pi = BALL::Constants::PI}
					\item shortcut via the {\tt using directive}:\\
							{\tt using namespace BALL::Constants; float my\_pi = PI;}
				\end{itemize}
				{\bf Definition:} \URL{BALL/COMMON/constants.h}
		*/
		//@{

		/**	@name	Mathematical constants
				All constants are double precision.\\
				There are basically two ways of accessing these constants:
				\begin{itemize}
					\item specify all namespaces:\\
							{\tt float my\_pi = BALL::Constants::PI}
					\item shortcut via the {\tt using directive}:\\
							{\tt using namespace BALL::Constants; float my\_pi = PI;}
				\end{itemize}
				{\bf Definition:} \URL{BALL/COMMON/constants.h}
		*/
		//@{

		/// PI
		static const double  PI = 3.14159265358979323846L;

		/// Euler's number - base of the natural logarithm
		static const double  E  = 2.718281828459045235L;

		/**	BALL epsilon: used internally for comparisons
		*/
		static const double EPSILON = 1e-6;
		//@}
			
		/**	@name Chemical/physical constants
				All constants are double precision.\\
				There are basically two ways of accessing these constants:
				\begin{itemize}
					\item specify all namespaces:\\
							{\tt float my\_pi = BALL::Constants::PI}
					\item shortcut via the {\tt using directive}:\\
							{\tt using namespace BALL::Constants; float my\_pi = PI;}
				\end{itemize}
				{\bf Definition:} \URL{BALL/COMMON/constants.h}
		*/
		//@{
		
		/**	Elementary charge.
			  in units of C ($1.60217738 \cdot 10^{-19}$ C)
		*/
		static const double	ELEMENTARY_CHARGE = 1.60217738E-19L;  	 // C     
	
		/// Elementary charge (alias)
		static const double	e0								=	ELEMENTARY_CHARGE;

		/** Electron mass.
				in units of kg ($9.1093897 \cdot 10^{-31}$ kg)
		*/
		static const double	ELECTRON_MASS   	= 9.1093897E-31L;   	 // kg

		/** Proton mass.
				in units of kg ($1.6726230 \cdot 10^{-27}$ kg)
		*/
		static const double	PROTON_MASS     	= 1.6726230E-27L;   	 // kg

		/** Neutron mass.
				in units of kg ($1.6749286 \cdot 10^{-27}$ kg)
		*/
		static const double	NEUTRON_MASS    	= 1.6749286E-27L;   	 // kg

		/** Avogadro constant.
				in units of $mol^{-1}$ ($6.0221367 \cdot 10^{23} mol^{-1}$)
		*/
		static const double	AVOGADRO        	= 6.0221367E+23L;   	 // 1 / mol

		/** Avogadro constant (alias)
		*/
		static const double	NA								= AVOGADRO;

		/** Avogadro constant (alias)
		*/
		static const double	MOL             	= AVOGADRO;

		/** Boltzmann constant.
				in units of J/K ($1.380657 \cdot 10^{-23}$ J/K)
		*/
		static const double	BOLTZMANN       	= 1.380657E-23L;  	   // J / K

		/** Boltzmann constant (alias)
		*/
		static const double	k	        				= BOLTZMANN;
		
		/** Planck constant.
				in units of Js ($6.6260754 \cdot 10^{-34}$ Js)
		*/
		static const double	PLANCK 	         	= 6.6260754E-34L;      // J * sec

		/** Planck constant (alias)
		*/
		static const double	h       	   			= PLANCK;

		/** Gas constant (= NA * k)	
		*/
		static const double	GAS_CONSTANT 	  	= NA * k;

		/** Gas constant (alias)
		*/
		static const double R 								= GAS_CONSTANT;

		/** Faraday constant (= NA * e0)
		*/
		static const double	FARADAY         	= NA * e0;

		/** Faraday constant (alias)
		*/
		static const double	F    							= FARADAY;

		/** Bohr radius.
				in units m ($5.29177249 \cdot 10^{-11}$ m)
		*/
		static const double	BOHR_RADIUS     	= 5.29177249E-11L;     // m

		/** Bohr radius (alias)
		*/
		static const double	a0     						= BOHR_RADIUS;

		//  the following values from: 
		//  P.W.Atkins: Physical Chemistry, 5th ed., Oxford University Press, 1995

		/** Vacuum permittivity.
				in units of $C^2J^{-1}m^{-1}$ ($8.85419 \cdot 10^{-12} C^2J^{-1}m^{-1}$)
		*/
		static const double	VACUUM_PERMITTIVITY    	= 8.85419E-12L;     // C^2 / (J * m)

		/** Vacuum permeability.
				in units of $Js^2C^{-2}m^{-1}$ ($4\pi \cdot 10^{-7} Js^2C^{-2}m^{-1}$)
		*/
		static const double	VACUUM_PERMEABILITY     = (4 * PI * 1E-7L);	// J s^2 / (C^2 * m)

		/** Speed of light.
				in units of m/s ($2.99792458 \cdot 10^8 ms^{-1}$)
		*/
		static const double	SPEED_OF_LIGHT          = 2.99792458E+8L;	  // m / s

		/** Speed of Light (alias)
		*/
		static const double	c												= SPEED_OF_LIGHT;

		/** Gravitational constant.
				in units of $Nm^2kg^{-2}$ ($6.67259 \cdot 10^{-11} Nm^2kg^{-2}$)	
		*/
		static const double	GRAVITATIONAL_CONSTANT  = 6.67259E-11L;    	// N m^2 / kg^2

		/** Fine structure constant.
				$7.29735 \cdot 10^{-3}$
		*/
		static const double	FINE_STRUCTURE_CONSTANT = 7.29735E-3L;   		// 1      
		//@}

		/**	@name	Conversion factors
				All constants are double precision.\\
				There are basically two ways of accessing these constants:
				\begin{itemize}
					\item specify all namespaces:\\
							{\tt float my\_pi = BALL::Constants::PI}
					\item shortcut via the {\tt using directive}:\\
							{\tt using namespace BALL::Constants; float my\_pi = PI;}
				\end{itemize}
				{\bf Definition:} \URL{BALL/COMMON/constants.h}
		*/
		//@{		
			
		/** Degree per rad.
				57.2957795130823209
		*/
		static const double	DEG_PER_RAD				= 57.2957795130823209L;

		/** Rad per degree.
				0.0174532925199432957
		*/
		static const double	RAD_PER_DEG			 	= 0.0174532925199432957L;

		/** mm per inch.
				25.4
		*/
		static const double	MM_PER_INCH 			= 25.4L;

		/** m per foot.
				3.048
		*/
		static const double	M_PER_FOOT  			= 3.048L;

		/** Joules per calorie.
				4.184
		*/
		static const double	JOULE_PER_CAL     = 4.184;

		/** Calories per Joule.
				1/JOULE\_PER\_CAL
		*/
		static const double	CAL_PER_JOULE     = (1 / 4.184);

		//@}

		//@}

	}
}

#endif // BALL_COMMON_CONSTANTS_H
