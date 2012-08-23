// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class CharmmImproperTorsion 
  : ForceFieldComponent
{
%TypeHeaderCode
	#include <BALL/MOLMEC/CHARMM/charmmImproperTorsion.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  // BALL_CREATE(CharmmImproperTorsion)
  CharmmImproperTorsion();
  CharmmImproperTorsion(ForceField&);
  CharmmImproperTorsion(const CharmmImproperTorsion&);
  ~CharmmImproperTorsion();
  virtual bool setup() throw(TooManyErrors);
  virtual double updateEnergy();
  virtual void updateForces();
};
  
