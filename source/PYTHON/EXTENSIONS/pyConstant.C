// $Id: pyConstant.C,v 1.1 2001/09/03 16:53:13 oliver Exp $

#include <BALL/COMMON/constants.h>
#include <BALL/PYTHON/pyConstant.h>

namespace BALL
{
	// EPSILON (used fr comparisons)
	double Constant::EPSILON = 1e-6;

	// PI
	double Constant::PI = 3.14159265358979323846L;

	// Euler's number - base of the natural logarithm
	double Constant::E  = 2.718281828459045235L;
	
	//	Elementary charge.
	double Constant::ELEMENTARY_CHARGE = 1.60217738E-19L;  	 // C     

	/// Elementary charge (alias)
	double Constant::e0								=	ELEMENTARY_CHARGE;

	// Electron mass.
	double Constant::ELECTRON_MASS   	= 9.1093897E-31L;   	 // kg

	// Proton mass.
	double Constant::PROTON_MASS     	= 1.6726230E-27L;   	 // kg

	// Neutron mass.
	double Constant::NEUTRON_MASS    	= 1.6749286E-27L;   	 // kg

	// Avogadro constant.
	double Constant::AVOGADRO        	= 6.0221367E+23L;   	 // 1 / mol

	// Avogadro constant (alias)
	double Constant::NA								= AVOGADRO;

	// Avogadro constant (alias)
	double Constant::MOL             	= AVOGADRO;

	// Boltzmann constant.
	double Constant::BOLTZMANN       	= 1.380657E-23L;  	   // J / K

	// Boltzmann constant (alias)
	double Constant::k	        				= BOLTZMANN;
	
	// Planck constant.
	double Constant::PLANCK 	         	= 6.6260754E-34L;      // J * sec

	// Planck constant (alias)
	double Constant::h       	   			= PLANCK;

	// Gas constant (= NA * k)	
	double Constant::GAS_CONSTANT 	  	= NA * k;

	// Gas constant (alias)
	double Constant::R 								= GAS_CONSTANT;

	// Faraday constant (= NA * e0)
	double Constant::FARADAY         	= NA * e0;

	// Faraday constant (alias)
	double Constant::F    							= FARADAY;

	// Bohr radius.
	double Constant::BOHR_RADIUS     	= 5.29177249E-11L;     // m

	// Bohr radius (alias)
	double Constant::a0     						= BOHR_RADIUS;

	//  the following values from: 
	//  P.W.Atkins: Physical Chemistry, 5th ed., Oxford University Press, 1995

	// Vacuum permittivity.
	double Constant::VACUUM_PERMITTIVITY    	= 8.85419E-12L;     // C^2 / (J * m)

	// Vacuum permeability.
	double Constant::VACUUM_PERMEABILITY     = (4 * PI * 1E-7L);	// J s^2 / (C^2 * m)

	// Speed of light.
	double Constant::SPEED_OF_LIGHT          = 2.99792458E+8L;	  // m / s

	// Speed of Light (alias)
	double Constant::c												= SPEED_OF_LIGHT;

	// Gravitational constant.
	double Constant::GRAVITATIONAL_CONSTANT  = 6.67259E-11L;    	// N m^2 / kg^2

	// Fine structure constant.
	double Constant::FINE_STRUCTURE_CONSTANT = 7.29735E-3L;   		// 1      
		
	// Degree per rad.
	double Constant::DEG_PER_RAD				= 57.2957795130823209L;

	// Rad per degree.
	double Constant::RAD_PER_DEG			 	= 0.0174532925199432957L;

	// mm per inch.
	double Constant::MM_PER_INCH 			= 25.4L;

	// m per foot.
	double Constant::M_PER_FOOT  			= 3.048L;

	// Joule per calorie
	double Constant::JOULE_PER_CAL     = 4.184;

	// Calories per Joule.
	double Constant::CAL_PER_JOULE     = (1 / 4.184);
}
