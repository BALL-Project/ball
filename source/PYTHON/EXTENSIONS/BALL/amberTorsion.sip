// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
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
  AmberTorsion(ForceField&);
  AmberTorsion(const AmberTorsion&);
  virtual ~AmberTorsion();

	struct SingleAmberTorsion {
			SingleAmberTorsion();
			SingleAmberTorsion(SingleData& t);
	};

  virtual bool setup() throw(TooManyErrors);
  virtual double updateEnergy();
  virtual void updateForces();
};
 
