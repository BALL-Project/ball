// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amberBend.sip,v 1.5 2004/12/27 17:24:55 amoll Exp $
//

class AmberBend 
  : ForceFieldComponent
{
%TypeHeaderCode	
	#include <BALL/MOLMEC/AMBER/amberBend.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  AmberBend();
  AmberBend(ForceField& /force_field/);
  AmberBend(const AmberBend& /amber_stretch/);
  ~AmberBend();
  virtual bool setup() throw(TooManyErrors);
  virtual double updateEnergy();
  virtual void updateForces();
};
