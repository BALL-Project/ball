// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amberStretch.sip,v 1.5 2004/12/27 17:24:55 amoll Exp $
//

class AmberStretch 
  : ForceFieldComponent
{
%TypeHeaderCode
	#include <BALL/MOLMEC/AMBER/amberStretch.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  // BALL_CREATE(AmberStretch)
  AmberStretch();
  AmberStretch(ForceField& /force_field/);
  AmberStretch(const AmberStretch& /amber_stretch/);
  ~AmberStretch();
  virtual bool setup() throw(TooManyErrors);
  virtual double updateEnergy();
  virtual void updateForces();
};
  
