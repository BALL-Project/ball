// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: charmmStretch.sip,v 1.3 2003/08/26 09:18:14 oliver Exp $
//

class CharmmStretch 
  : ForceFieldComponent
{
%HeaderCode
	#include <BALL/MOLMEC/CHARMM/charmmStretch.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE(CharmmStretch)
  CharmmStretch();
  CharmmStretch(ForceField& /force_field/);
  CharmmStretch(const CharmmStretch& /charmm_stretch/);
  ~CharmmStretch();
  virtual bool setup();
  virtual double updateEnergy();
  virtual void updateForces();
};
 
