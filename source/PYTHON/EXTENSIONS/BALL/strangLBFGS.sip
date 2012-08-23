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
	StrangLBFGSMinimizer(ForceField&);
	StrangLBFGSMinimizer(ForceField&, SnapShotManager*);
	StrangLBFGSMinimizer(ForceField&, SnapShotManager*, const Options&);
	StrangLBFGSMinimizer(ForceField&, const Options&);
	~StrangLBFGSMinimizer();
	// StrangLBFGSMinimizer& operator = (const StrangLBFGSMinimizer&);
	virtual bool specificSetup();
	void setMaxNumOfStoredVectPairs(Size);
	Size getMaxNumOfStoredVectPairs() const;
	void setImprovedFlag(bool);
	bool getImprovedFlag() const;
	virtual double findStep();
	virtual void updateDirection();
	virtual bool minimize(Size iterations = 0, bool restart = false); 
	Options options;
};
