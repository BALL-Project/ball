// $Id: charmmNonBonded.sip,v 1.1 2000/07/18 17:02:16 oliver Exp $

class CharmmNonBonded 
  : ForceFieldComponent
{
%HeaderCode
	#include <BALL/MOLMEC/CHARMM/charmmNonBonded.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE(CharmmNonBonded)
  CharmmNonBonded();
  CharmmNonBonded(ForceField& /force_field/);
  CharmmNonBonded(const CharmmNonBonded& /charmm_non_bonded/, bool /clone_deep/ = true);
  ~CharmmNonBonded();
  virtual bool setup();
  virtual double updateEnergy();
  virtual void updateForces();
  virtual double getElectrostaticEnergy() const;
  virtual double getVdwEnergy() const;
  virtual double getSolvationEnergy() const;
};
