// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: charmmBend.sip,v 1.3 2003/08/26 09:18:13 oliver Exp $
//

class CharmmBend 
  : ForceFieldComponent
{
%HeaderCode
	#include <BALL/MOLMEC/CHARMM/charmmBend.h>
	using namespace BALL;
%End
  public:
  CharmmBend();
  CharmmBend(ForceField& /force_field/);
  CharmmBend(const CharmmBend& /charmm_stretch/);
  ~CharmmBend();
  virtual bool setup();
  virtual double updateEnergy();
  virtual void updateForces();
};
