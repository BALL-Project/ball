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
  ConjugateGradientMinimizer();
  ConjugateGradientMinimizer(ForceField&);
  ConjugateGradientMinimizer(ForceField&, SnapShotManager*);
  ConjugateGradientMinimizer(ForceField&, SnapShotManager*, Options&);
  ConjugateGradientMinimizer(ForceField&, const Options&);
  ConjugateGradientMinimizer(const ConjugateGradientMinimizer&, bool);

  // ConjugateGradientMinimizer& operator = (const ConjugateGradientMinimizer& rhs);
  virtual bool specificSetup();
  void setStepLength(double);
  double getStepLength() const; 
  virtual bool findStep();
  virtual void updateDirection();
  virtual bool minimize(int, bool); 
  Options options;
 };
