// $Id: conjugateGradient.sip,v 1.2 2000/07/03 11:11:05 oliver Exp $

class ConjugateGradientMinimizer 
  : EnergyMinimizer
{
%HeaderCode
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
  ConjugateGradientMinimizer(const ConjugateGradientMinimizer& /rhs/, bool /deep/ = true);
  ~ConjugateGradientMinimizer();
  // ConjugateGradientMinimizer& operator = (const ConjugateGradientMinimizer& /rhs/);
  virtual bool specificSetup();
  void setStepLength(double /value/);
  double getStepLength() const; 
  virtual bool findStep();
  virtual void updateDirection();
  virtual bool minimize(Size /iterations/ = 0, bool /restart/ = false); 
  Options options;
};
