// $Id: names.C,v 1.1 2000/07/03 20:02:23 oliver Exp $

#include<BALL/NMR/names.h>


// hier muessen alle shiftmodule geladen werden :

#include<BALL/NMR/johnsonBovey.h>
#include<BALL/NMR/haighMallion.h>
#include<BALL/NMR/LEF.h>
#include<BALL/NMR/anisotropy.h>
#include<BALL/NMR/randomCoil.h>
///////////////////////////////////////////////
using namespace std;


namespace BALL {

ShiftNames::ShiftNames()
	{}
ShiftNames::~ShiftNames()
	{}
	
ShiftModule* ShiftNames::get_module(String module_name)
		{
		//hier werden die Klassennamen der Module mit entsprechenden Strings identifiziert:
		
		if (module_name=="JB") return (new JohnsonBoveyShift);
		else
		if (module_name=="HM") return (new HaighMallionShift);
		else
		if (module_name=="LEF") return (new LEFShift);
		else
		if (module_name=="ANISO") return (new AnIsoShift);
		else
		if (module_name=="RC") return (new RandomCoilShift);
		else 
			return NULL;
		}

} // namespace Ball
 	
