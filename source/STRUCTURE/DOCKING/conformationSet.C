#include <BALL/STRUCTURE/DOCKING/conformationSet.h>

#include <BALL/KERNEL/atomContainer.h>
#include <BALL/FORMAT/DCDFile.h>
#include <BALL/MOLMEC/COMMON/snapShotManager.h>
#include <BALL/MOLMEC/COMMON/forceField.h>

#include <stdlib.h>
#include <iostream>

using namespace::std;


namespace BALL
{
	ConformationSet::ConformationSet(const System& system)
		: system_(system)
	{
	}

	void ConformationSet::setup(const System& system)
		throw()
	{
		system_ = system;
	}			
  
	void ConformationSet::add(const float score, const System& conformation)
	  throw()
	{
		SnapShot sn;
		sn.takeSnapShot(conformation);
		structures_.push_back(sn);

		Conformation c(snapshot_order_.size(), score);
		snapshot_order_.push_back(c);
	}

	const System& ConformationSet::getSystem() const
		throw()
	{
		return system_;
	}

	const std::vector<ConformationSet::Conformation>& ConformationSet::getScoring() const
		throw()
	{
		return snapshot_order_;
	}

	void ConformationSet::setScoring(std::vector<Conformation>& score)
		throw()
	{
		snapshot_order_ = score;
	}

	void ConformationSet::resetScoring()
		throw()
	{
		snapshot_order_.resize(structures_.size());

		for (Size i=0; i<snapshot_order_.size(); i++)
		{
			snapshot_order_[i].first = i;
			snapshot_order_[i].second = 0.0;
		}
	}

	const std::vector<SnapShot>& ConformationSet::getUnscoredConformations() const
		throw()
	{
		return structures_;
	}

	const SnapShot& ConformationSet::operator [] (const Index pos) const
	{
		return structures_[snapshot_order_[pos].first];		
	}		

	void ConformationSet::writeDCDFile(const String& filename, const Size num)
	{
		ForceField ff(system_);
		DCDFile dcd(filename, std::ios::out);
		
		SnapShotManager ssm(&system_, &ff, &dcd);
		
		Size min = (num < snapshot_order_.size()) ? num : snapshot_order_.size();
	
		for (Size i=0; i<min; i++)
		{
			structures_[snapshot_order_[i].first].applySnapShot(system_);
			ssm.takeSnapShot();
		}
	}
}
