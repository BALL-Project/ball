// $Id: energyMinimizer.sip,v 1.2 2003/03/25 17:08:57 anhi Exp $
class EnergyMinimizer
{
%HeaderCode
	#include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
	using namespace BALL;
%End
  public:

  EnergyMinimizer();
  EnergyMinimizer(ForceField&);
  EnergyMinimizer(ForceField&, Options&);

  // EnergyMinimizer& operator = (const EnergyMinimizer&);
  bool isValid() const;
  bool setup(ForceField&);
  bool setup(ForceField&, SnapShotManager*);
  bool setup(ForceField&, SnapShotManager*,  Options&);
  bool setup(ForceField&, Options&);
  virtual bool specificSetup();
  virtual bool isConverged() const;
  virtual double findStep();
  virtual void updateDirection();
  virtual double updateEnergy();
  virtual void updateForces();
  virtual void printEnergy() const;
  virtual void takeSnapShot() const;
  virtual void finishIteration();
  int getNumberOfIterations() const;
  Gradient& getDirection();
  Gradient& getGradient();
  Gradient& getInitialGradient();
  double getEnergy() const;
  double getInitialEnergy() const;
  void setNumberOfIterations(int);
  int getMaxNumberOfIterations() const;
  void setMaxNumberOfIterations(int);
  void setMaxSameEnergy(int);
  int getMaxSameEnergy() const;
  void setEnergyOutputFrequency(int);
  int getEnergyOutputFrequency() const;
  void setEnergyDifferenceBound(float);
  float getEnergyDifferenceBound() const;
 	void  setMaxGradient(float);
  float getMaxGradient() const;
  void setMaximumDisplacement(float);
  float getMaximumDisplacement() const;
  void setSnapShotFrequency(int);
  int getSnapShotFrequency() const;
  ForceField* getForceField();
  virtual bool minimize(int, bool);
  Options options;
};
