// $Id: shiftModule.C,v 1.1 2000/07/04 06:56:58 oliver Exp $

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

