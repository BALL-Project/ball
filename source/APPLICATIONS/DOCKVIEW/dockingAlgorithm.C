// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:


#include "dockingAlgorithm.h"
#include <stdlib.h>
#include <iostream>
#include <BALL/DATATYPE/options.h>


using namespace std;


void BALL::Docking::DockingAlgorithm::setup(System &system1,System &system2 ,Options &options)
	throw()
{
	Log.error() << "This is just an interface definition. Check your dynamic binding" << std::endl;
}

void BALL::Docking::DockingAlgorithm::start()
	throw()
{
	Log.error() << "This is just an interface definition. Check your dynamic binding" << std::endl;
}

void BALL::Docking::DockingAlgorithm::pause()
	throw()
{
  Log.error() << "This is just an interface definition. Check your dynamic binding" << std::endl;
}


void BALL::Docking::DockingAlgorithm::abort()
	throw()
{
	Log.error() << "This is just an interface definition. Check your dynamic binding" << std::endl;
}

bool BALL::Docking::DockingAlgorithm::hasFinished() const
	throw()
{
  Log.error() << "This is just an interface definition. Check your dynamic binding" << std::endl;
	bool test = true;
	return test;
}

float BALL::Docking::DockingAlgorithm::getProgress() const
	throw()
{
	Log.error() << "This is just an interface definition. Check your dynamic binding" << endl;
	float test2 = 0;
	return test2;
}


