// $Id: amberNonBonded.sip,v 1.1 2000/07/18 17:02:05 oliver Exp $

class AmberNonBonded 
  : ForceFieldComponent
{
%HeaderCode
	#include <BALL/MOLMEC/AMBER/amberNonBonded.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE(AmberNonBonded)
  AmberNonBonded();
  AmberNonBonded(ForceField& /force_field/);
  AmberNonBonded(const AmberNonBonded& /amber_non_bonded/, bool /clone_deep/ = true);
  ~AmberNonBonded();
  bool setup();
  virtual double updateEnergy();
  virtual void updateForces();
  virtual double getElectrostaticEnergy() const;
  virtual double getVdwEnergy() const;
};
  
