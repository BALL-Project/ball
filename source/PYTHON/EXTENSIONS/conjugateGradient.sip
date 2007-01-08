// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: conjugateGradient.sip,v 1.4 2003/10/29 22:54:39 oliver Exp $
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
	//    static const char* STEP_LENGTH; 
	//  };
	//  struct Default
	//  {
	//    static double STEP_LENGTH; 
	//  };
	//    BALL_CREATE(ConjugateGradientMinimizer)
  ConjugateGradientMinimizer();
  ConjugateGradientMinimizer(ForceField& /force_field/);
  ConjugateGradientMinimizer(ForceField& /force_field/, SnapShotManager* /ssm/);
  ConjugateGradientMinimizer
   (ForceField& /force_field/, SnapShotManager* /ssm/, const Options& /options/);
  ConjugateGradientMinimizer(ForceField& /force_field/, const Options& /options/);
  ~ConjugateGradientMinimizer();
  // ConjugateGradientMinimizer& operator = (const ConjugateGradientMinimizer& /rhs/);
  virtual bool specificSetup();
  void setStepLength(double /value/);
  double getStepLength() const; 
  virtual double findStep();
  virtual void updateDirection();
  virtual bool minimize(Size /iterations/ = 0, bool /restart/ = false); 
  Options options;
};
