// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: charmmImproperTorsion.sip,v 1.3 2003/08/26 09:18:14 oliver Exp $
//

class CharmmImproperTorsion 
  : ForceFieldComponent
{
%HeaderCode
	#include <BALL/MOLMEC/CHARMM/charmmImproperTorsion.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE(CharmmImproperTorsion)
  CharmmImproperTorsion();
  CharmmImproperTorsion(ForceField& /force_field/);
  CharmmImproperTorsion(const CharmmImproperTorsion& /charmm_stretch/);
  ~CharmmImproperTorsion();
  virtual bool setup();
  virtual double updateEnergy();
  virtual void updateForces();
};
  
