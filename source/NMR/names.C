	// $Id: names.C,v 1.4 2000/09/16 07:44:13 oliver Exp $

#include<BALL/NMR/names.h>


#include<BALL/NMR/johnsonBovey.h>
#include<BALL/NMR/haighMallion.h>
#include<BALL/NMR/LEF.h>
#include<BALL/NMR/anisotropy.h>
#include<BALL/NMR/randomCoil.h>


namespace BALL
{

	ShiftNames::ShiftNames()
	{
	}
	ShiftNames::~ShiftNames()
	{
	}

	ShiftModule* ShiftNames::getModule(const String& module_name)
	{
		//hier werden die Klassennamen der Module mit entsprechenden Strings identifiziert:

		if (module_name == "JB")
		{
			return (new JohnsonBoveyShift);
		}
		else if (module_name == "HM")
		{
			return (new HaighMallionShift);
		}
		else if (module_name == "LEF")
		{
			return (new LEFShiftProcessor);
		}
		else if (module_name == "ANISO")
		{
			return (new AnIsoShift);
		}
		else if (module_name == "RC")
		{
			return (new RandomCoilShift);
		}

		return 0;
	}

}	// namespace BALL
