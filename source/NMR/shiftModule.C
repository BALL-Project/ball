// $Id: shiftModule.C,v 1.2 2000/08/28 16:08:54 oliver Exp $

#include<BALL/NMR/shiftModule.h>

using namespace std;

namespace BALL
{

	ShiftModule::ShiftModule ()
	{
		module_name_ = new String;
	}

	ShiftModule::~ShiftModule ()
	{
		delete module_name_;
	}

}	// namespace Ball
