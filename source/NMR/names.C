// $Id: names.C,v 1.3 2000/09/07 19:38:31 oliver Exp $

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
			return (new LEFShift);
		}
		else if (module_name == "ANISO")
		{
			return (new AnIsoShift);
		}
		else if (module_name == "RC")
		{
			return (new RandomCoilShift);
		}

		return NULL;
	}

}	// namespace BALL
