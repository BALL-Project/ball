// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amber.sip,v 1.4 2004/12/27 17:24:55 amoll Exp $
//

class AmberFF 
 : ForceField
{
%TypeHeaderCode
	#include <BALL/MOLMEC/AMBER/amber.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  AmberFF();
  AmberFF(System&);
  AmberFF(System&, Options&);
  AmberFF(AmberFF&);

  bool specificSetup() throw(TooManyErrors);
  double getStretchEnergy() const;
  double getBendEnergy() const;
  double getTorsionEnergy() const;
  double getNonbondedEnergy() const;
  double getESEnergy() const;
  double getVdWEnergy() const;
  bool hasInitializedParameters() const;
};
