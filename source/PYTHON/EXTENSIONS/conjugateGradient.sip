// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class ConjugateGradientMinimizer 
	: EnergyMinimizer
{
%TypeHeaderCode
	#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
	using namespace BALL;
%End
	public:
	enum UpdateMethod
	{
		POLAK_RIBIERE,
		FLETCHER_REEVES,
		SHANNO
	};
	//  struct Option
	//  {
	//    static const char* UPDATE_METHOD;
	//  };
	//  struct Default
	//  {
	//    static const Size UPDATE_METHOD;
	//  };
	//    BALL_CREATE(ConjugateGradientMinimizer)
	ConjugateGradientMinimizer();
	ConjugateGradientMinimizer(ForceField& /force_field/);
	ConjugateGradientMinimizer(ForceField& /force_field/, SnapShotManager* /ssm/);
	ConjugateGradientMinimizer(ForceField& /force_field/, SnapShotManager* /ssm/, const Options& /options/);
	ConjugateGradientMinimizer(ForceField& /force_field/, const Options& /options/);
	~ConjugateGradientMinimizer();
	// ConjugateGradientMinimizer& operator = (const ConjugateGradientMinimizer& /rhs/);
	virtual bool specificSetup();
	void setUpdateMethod(UpdateMethod /method/);
	UpdateMethod getUpdateMethod() const;
	virtual double findStep();
	virtual void updateDirection();
	virtual bool minimize(Size /iterations/ = 0, bool /restart/ = false);
};
