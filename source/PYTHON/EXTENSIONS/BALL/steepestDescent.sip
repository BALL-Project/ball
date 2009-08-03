// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class SteepestDescentMinimizer 
  : EnergyMinimizer
{
%TypeHeaderCode
	#include <BALL/MOLMEC/MINIMIZATION/steepestDescent.h>
	using namespace BALL;
%End	
	public:
	//    BALL_CREATE(SteepestDescentMinimizer)
	SteepestDescentMinimizer();
	SteepestDescentMinimizer(ForceField& /force_field/);
	SteepestDescentMinimizer(ForceField& /force_field/, SnapShotManager* /ssm/);
	SteepestDescentMinimizer(ForceField& /force_field/, SnapShotManager* /ssm/, const Options& /options/);
	SteepestDescentMinimizer(ForceField& /force_field/, const Options& /options/);
	~SteepestDescentMinimizer();
	// SteepestDescentMinimizer& operator = (const SteepestDescentMinimizer& /rhs/);
	virtual bool specificSetup();
	virtual double findStep();
	virtual void updateDirection();
	virtual bool minimize(Size /iterations/ = 0, bool /resume/ = false);
};
