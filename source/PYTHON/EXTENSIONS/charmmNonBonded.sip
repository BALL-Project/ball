// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: charmmNonBonded.sip,v 1.3 2003/08/26 09:18:14 oliver Exp $
//

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
  CharmmNonBonded(const CharmmNonBonded& /charmm_non_bonded/);
  ~CharmmNonBonded() throw();
  virtual bool setup() throw();
  virtual double updateEnergy() throw();
  virtual void updateForces() throw();
  virtual double getElectrostaticEnergy() const throw();
  virtual double getVdwEnergy() const throw();
  virtual double getSolvationEnergy() const throw();
};
