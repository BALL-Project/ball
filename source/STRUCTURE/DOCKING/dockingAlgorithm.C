// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/STRUCTURE/DOCKING/dockingAlgorithm.h>

#include <cstdlib>
#include <iostream>

using namespace std;

namespace BALL
{
	void DockingAlgorithm::setup(System& system1, System& system2, Options& new_options)
		
	{
		system1_ = system1;
		system2_ = system2;
		options  = new_options;
	}

	void DockingAlgorithm::setup(System& system1, System& system2)
		
	{
		system1_ = system1;
		system2_ = system2;
	}
		
	void DockingAlgorithm::start()
		
	{
		pause_ = false;
		abort_ = false;
	}

	void DockingAlgorithm::pause()
		
	{
		pause_ = true;
	}

	void DockingAlgorithm::proceed()
		
	{
		pause_ = false;
		abort_ = false;
	}

	void DockingAlgorithm::abort()
		
	{
		abort_ = true;
	}

	bool DockingAlgorithm::hasFinished() const
		
	{
		Log.error() << "This is just an interface definition. Check your dynamic binding" << std::endl;
		return true;
	}

	float DockingAlgorithm::getProgress() const
		
	{
		Log.error() << "This is just an interface definition. Check your dynamic binding" << endl;
		return 0;
	}

	ConformationSet DockingAlgorithm::getConformationSet(Index /*total_conformations*/)
		
	{
		ConformationSet dummy;
		return dummy;
	}

}
