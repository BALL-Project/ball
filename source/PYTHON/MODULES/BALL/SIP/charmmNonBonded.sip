// $Id: charmmNonBonded.sip,v 1.2 2001/07/25 11:32:32 oliver Exp $

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
  ~CharmmNonBonded() throw();
  virtual bool setup() throw();
  virtual double updateEnergy() throw();
  virtual void updateForces() throw();
  virtual double getElectrostaticEnergy() const throw();
  virtual double getVdwEnergy() const throw();
  virtual double getSolvationEnergy() const throw();
};
