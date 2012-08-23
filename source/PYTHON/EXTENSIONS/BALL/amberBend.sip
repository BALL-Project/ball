// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
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
  AmberBend(ForceField&);
  AmberBend(const AmberBend&);
  virtual ~AmberBend();
  virtual bool setup() throw(TooManyErrors);
  virtual double updateEnergy();
  virtual void updateForces();
};
