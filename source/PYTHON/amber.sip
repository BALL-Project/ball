// $Id: amber.sip,v 1.2 2000/06/30 05:58:43 oliver Exp $

class AmberFF 
 : ForceField
{
%HeaderCode
#include <BALL/MOLMEC/AMBER/amber.h>
using namespace BALL;
%End
  public:
  AmberFF();
  AmberFF(System&);
  AmberFF(System&, Options&);
  AmberFF(AmberFF&, bool);

  bool specificSetup();
  double getStretchEnergy() const;
  double getBendEnergy() const;
  double getTorsionEnergy() const;
  double getNonbondedEnergy() const;
  double getESEnergy() const;
  double getVdWEnergy() const;
  bool hasInitializedParameters() const;
};
