// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/STRUCTURE/DOCKING/dockingAlgorithm.h>

#include <stdlib.h>
#include <iostream>

using namespace std;

namespace BALL
{
	void DockingAlgorithm::setup(System& system1, System& system2, Options& new_options)
		throw()
	{
		system1_ = system1;
		system2_ = system2;
		options  = new_options;
	}

	void DockingAlgorithm::setup(System& system1, System& system2)
		throw()
	{
		system1_ = system1;
		system2_ = system2;
	}
		
	void DockingAlgorithm::start()
		throw()
	{
		Log.error() << "This is just an interface definition. Check your dynamic binding" << std::endl;
	}

	void DockingAlgorithm::pause()
		throw()
	{
		Log.error() << "This is just an interface definition. Check your dynamic binding" << std::endl;
	}

	void DockingAlgorithm::proceed()
		throw()
	{
		Log.error() << "This is just an interface definition. Check your dynamic binding" << std::endl;
	}

	void DockingAlgorithm::abort()
		throw()
	{
		Log.error() << "This is just an interface definition. Check your dynamic binding" << std::endl;
	}

	bool DockingAlgorithm::hasFinished() const
		throw()
	{
		Log.error() << "This is just an interface definition. Check your dynamic binding" << std::endl;
		bool test = true;
		return test;
	}

	float DockingAlgorithm::getProgress() const
		throw()
	{
		Log.error() << "This is just an interface definition. Check your dynamic binding" << endl;
		float test2 = 0;
		return test2;
	}

	ConformationSet DockingAlgorithm::getConformationSet(Index /*total_conformations*/)
		throw()
	{
		ConformationSet dummy;
		return dummy;
	}

}
