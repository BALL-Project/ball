// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amberTorsion.sip,v 1.4 2003/10/29 22:54:36 oliver Exp $
//

class AmberTorsion 
  : ForceFieldComponent
{
%TypeHeaderCode
	#include <BALL/MOLMEC/AMBER/amberTorsion.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE(AmberTorsion)
  AmberTorsion();
  AmberTorsion(ForceField& /force_field/);
  AmberTorsion(const AmberTorsion& /amber_stretch/);
  ~AmberTorsion();
  virtual bool setup();
  virtual double updateEnergy();
  virtual void updateForces();
};
 
