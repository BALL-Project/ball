// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amberTorsion.sip,v 1.5 2004/12/27 17:24:56 amoll Exp $
//

class AmberTorsion 
  : ForceFieldComponent
{
%TypeHeaderCode
	#include <BALL/MOLMEC/AMBER/amberTorsion.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  // BALL_CREATE(AmberTorsion)
  AmberTorsion();
  AmberTorsion(ForceField& /force_field/);
  AmberTorsion(const AmberTorsion& /amber_stretch/);
  ~AmberTorsion();
  virtual bool setup() throw(TooManyErrors);
  virtual double updateEnergy();
  virtual void updateForces();
};
 
