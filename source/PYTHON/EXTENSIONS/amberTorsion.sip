// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amberTorsion.sip,v 1.3 2003/08/26 09:18:12 oliver Exp $
//

class AmberTorsion 
  : ForceFieldComponent
{
%HeaderCode
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
 
