// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amberStretch.sip,v 1.3 2003/08/26 09:18:12 oliver Exp $
//

class AmberStretch 
  : ForceFieldComponent
{
%HeaderCode
	#include <BALL/MOLMEC/AMBER/amberStretch.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE(AmberStretch)
  AmberStretch();
  AmberStretch(ForceField& /force_field/);
  AmberStretch(const AmberStretch& /amber_stretch/);
  ~AmberStretch();
  virtual bool setup();
  virtual double updateEnergy();
  virtual void updateForces();
};
  
