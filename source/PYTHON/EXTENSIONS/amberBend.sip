// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amberBend.sip,v 1.4 2003/10/29 22:54:35 oliver Exp $
//

class AmberBend 
  : ForceFieldComponent
{
%TypeHeaderCode	
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
