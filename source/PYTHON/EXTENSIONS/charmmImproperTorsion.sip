// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: charmmImproperTorsion.sip,v 1.4 2003/10/29 22:54:38 oliver Exp $
//

class CharmmImproperTorsion 
  : ForceFieldComponent
{
%TypeHeaderCode
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
  
