// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amberBend.sip,v 1.3 2003/08/26 09:18:11 oliver Exp $
//

class AmberBend 
  : ForceFieldComponent
{
%HeaderCode	
	#include <BALL/MOLMEC/AMBER/amberBend.h>
	using namespace BALL;
%End
  public:
  AmberBend();
  AmberBend(ForceField& /force_field/);
  AmberBend(const AmberBend& /amber_stretch/);
  ~AmberBend();
  virtual bool setup();
  virtual double updateEnergy();
  virtual void updateForces();
};
