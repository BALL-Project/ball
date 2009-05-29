// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
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
	virtual ~AmberFF();

	virtual void clear();
  virtual bool specificSetup() throw(TooManyErrors);
  double getStretchEnergy() const;
  double getBendEnergy() const;
  double getTorsionEnergy() const;
  double getNonbondedEnergy() const;
  double getESEnergy() const;
  double getVdWEnergy() const;
  bool hasInitializedParameters() const;
	Size getUpdateFrequency() const;
	virtual String getResults() const;
};
