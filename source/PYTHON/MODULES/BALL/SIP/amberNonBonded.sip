// $Id: amberNonBonded.sip,v 1.2 2001/07/25 11:32:31 oliver Exp $

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
  ~AmberNonBonded() throw();
  bool setup() throw();
  virtual double updateEnergy() throw();
  virtual void updateForces() throw();
  virtual double getElectrostaticEnergy() const throw();
  virtual double getVdwEnergy() const throw();
};
  
