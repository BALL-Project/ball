#include <BALL/STRUCTURE/DOCKING/rankedConformations.h>

#include <BALL/KERNEL/atomContainer.h>
#include <stdlib.h>
#include <iostream>
using namespace::std;


namespace BALL
{
	RankedConformations::RankedConformations(const System& system, const String& filename)
		: system_(system),
			ff_(system_),
			dcdfile_(filename, std::ios::out),
			structures_(&system_, &ff_, &dcdfile_)
	{
	}

	void RankedConformations::setup(const System& system, const String& filename)
		throw()
	{
		system_ = system;
		ff_.setup(system_);
		dcdfile_.open(filename, std::ios::out);
		structures_.setup(&system_, &ff_, &dcdfile_);
	}			
  
	void RankedConformations::add(float score, const System& conformation)
	  throw()
	{
		SnapShot sn;
		sn.takeSnapShot(conformation);
		sn.applySnapShot(system_);
			
		structures_.takeSnapShot();
		Conformation c(snapshot_order_.size(), score);
		
		snapshot_order_.insert(c);
	}

	const System& RankedConformations::getSystem() const
		throw()
	{
		return system_;
	}

	void RankedConformations::closeTrajectoryFile()
		throw()
	{
		dcdfile_.close();
	}		
}
