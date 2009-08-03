// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class StrangLBFGSMinimizer 
	: EnergyMinimizer
{
%TypeHeaderCode
	#include <BALL/MOLMEC/MINIMIZATION/strangLBFGS.h>
	using namespace BALL;
%End
	public:
	//	struct Option 
	//	{
	//		static const char* IMPROVED;
	//		static const char* NUM_OF_STORED_VECT_PAIRS;
	//	};
	// 	struct Default
	//	{
	//		static const bool IMPROVED;
	//		static const Size NUM_OF_STORED_VECT_PAIRS;
	//	};
	//    BALL_CREATE(StrangLBFGSMinimizer)
	StrangLBFGSMinimizer();	
	StrangLBFGSMinimizer(ForceField& /force_field/);
	StrangLBFGSMinimizer(ForceField& /force_field/, SnapShotManager* /ssm/);
	StrangLBFGSMinimizer(ForceField& /force_field/, SnapShotManager* /ssm/, const Options& /options/);
	StrangLBFGSMinimizer(ForceField& /force_field/, const Options& /options/);
	~StrangLBFGSMinimizer();
	// StrangLBFGSMinimizer& operator = (const StrangLBFGSMinimizer& /rhs/);
	virtual bool specificSetup();
	void setMaxNumOfStoredVectPairs(Size /num/);
	Size getMaxNumOfStoredVectPairs() const;
	void setImprovedFlag(bool /flag/);
	bool getImprovedFlag() const;
	virtual double findStep();
	virtual void updateDirection();
	virtual bool minimize(Size /iterations/ = 0, bool /restart/ = false); 
	Options options;
};
