// $Id: ShiftModule.C,v 1.1 2000/07/03 20:02:19 oliver Exp $

#include<BALL/NMR/shiftModule.h>

using namespace std;

namespace BALL{

ShiftModule::ShiftModule()
	{
	module_name_=new String;
	}
	
ShiftModule::~ShiftModule()
	{
	delete module_name_;
	}

} // namespace Ball

