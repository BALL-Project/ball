// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amberNonBonded.sip,v 1.5 2004/09/22 21:07:05 oliver Exp $
//

class AmberNonBonded 
  : ForceFieldComponent
{
%TypeHeaderCode
	#include <BALL/MOLMEC/AMBER/amberNonBonded.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE(AmberNonBonded)
  AmberNonBonded();
  AmberNonBonded(ForceField& /force_field/);
  AmberNonBonded(const AmberNonBonded& /amber_non_bonded/);
  ~AmberNonBonded() throw();
  virtual bool setup() throw();
  virtual double updateEnergy() throw();
  virtual void updateForces() throw();
  virtual double getElectrostaticEnergy() const throw();
  virtual double getVdwEnergy() const throw();
};
  
