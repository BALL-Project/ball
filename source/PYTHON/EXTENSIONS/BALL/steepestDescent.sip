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
	SteepestDescentMinimizer(ForceField&);
	SteepestDescentMinimizer(ForceField&, SnapShotManager*);
	SteepestDescentMinimizer(ForceField&, SnapShotManager*, const Options&);
	SteepestDescentMinimizer(ForceField&, const Options&);
	~SteepestDescentMinimizer();
	// SteepestDescentMinimizer& operator = (const SteepestDescentMinimizer&);
	virtual bool specificSetup();
	virtual double findStep();
	virtual void updateDirection();
	virtual bool minimize(Size iterations = 0, bool resume = false);
};
