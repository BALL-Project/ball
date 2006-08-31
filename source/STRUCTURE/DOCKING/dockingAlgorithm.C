// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/STRUCTURE/DOCKING/dockingAlgorithm.h>

#include <stdlib.h>
#include <iostream>

using namespace std;

namespace BALL
{
	// options common for all docking algorithms
	const String DockingAlgorithm::Option::BEST_NUM = "best_num";
	const String DockingAlgorithm::Option::VERBOSITY = "verbosity";

	const int DockingAlgorithm::Default::BEST_NUM  = 100;
	const int DockingAlgorithm::Default::VERBOSITY = 0;


	DockingAlgorithm::DockingAlgorithm()
		throw()
	{
		system_changed_ = false;
		is_setup_ = false;

		options.setDefaultInteger(Option::BEST_NUM, Default::BEST_NUM);
		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
	}

	void DockingAlgorithm::setup(System& system1, System& system2, Options& new_options)
		throw()
	{
		is_setup_ = true;
		system1_ = system1;
		system2_ = system2;
		options  = new_options;
	}

	void DockingAlgorithm::setup(System& system1, System& system2)
		throw()
	{
		is_setup_ = true;
		system1_ = system1;
		system2_ = system2;
	}
		
	void DockingAlgorithm::start()
		throw()
	{
		pause_ = false;
		abort_ = false;
		is_setup_ = false;
	}

	void DockingAlgorithm::pause()
		throw()
	{
		pause_ = true;
	}

	void DockingAlgorithm::proceed()
		throw()
	{
		pause_ = false;
		abort_ = false;
	}

	void DockingAlgorithm::abort()
		throw()
	{
		abort_ = true;
	}

	bool DockingAlgorithm::hasFinished() const
		throw()
	{
		Log.error() << "This is just an interface definition. Check your dynamic binding" << std::endl;
		return true;
	}

	float DockingAlgorithm::getSetupProgress() const
		throw()
	{
		return 0;
	}

	float DockingAlgorithm::getDockingProgress() const
		throw()
	{
		Log.error() << "This is just an interface definition. Check your dynamic binding" << std::endl;
		return 0;
	}


	const System& DockingAlgorithm::getIntermediateResult()
		throw()
	{
		system_changed_ = false;
		return getIntermediateResult_();
	}

	const System& DockingAlgorithm::getIntermediateResult_()
		throw()
	{
		Log.error() << "This is just an interface definition. Check your dynamic binding" << std::endl;
		return System();
	}

	ConformationSet DockingAlgorithm::getConformationSet(Index /*total_conformations*/)
		throw()
	{
		ConformationSet dummy;
		return dummy;
	}

}
