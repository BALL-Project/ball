// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: charmmStretch.sip,v 1.5 2004/12/27 17:24:56 amoll Exp $
//

class CharmmStretch 
  : ForceFieldComponent
{
%TypeHeaderCode
	#include <BALL/MOLMEC/CHARMM/charmmStretch.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  // BALL_CREATE(CharmmStretch)
  CharmmStretch();
  CharmmStretch(ForceField& /force_field/);
  CharmmStretch(const CharmmStretch& /charmm_stretch/);
  ~CharmmStretch();
  virtual bool setup() throw(TooManyErrors);
  virtual double updateEnergy();
  virtual void updateForces();
};
 
