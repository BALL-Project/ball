// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amberNonBonded.sip,v 1.6 2004/12/27 17:24:55 amoll Exp $
//

class AmberNonBonded 
  : ForceFieldComponent
{
%TypeHeaderCode
	#include <BALL/MOLMEC/AMBER/amberNonBonded.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  // BALL_CREATE(AmberNonBonded)
  AmberNonBonded();
  AmberNonBonded(ForceField& /force_field/);
  AmberNonBonded(const AmberNonBonded& /amber_non_bonded/);
  ~AmberNonBonded() throw();
  virtual bool setup() throw(TooManyErrors);
  virtual double updateEnergy() throw();
  virtual void updateForces() throw();
  virtual double getElectrostaticEnergy() const throw();
  virtual double getVdwEnergy() const throw();
};
  
