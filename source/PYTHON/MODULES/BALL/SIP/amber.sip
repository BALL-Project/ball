// $Id: amber.sip,v 1.2 2000/10/24 20:29:16 oliver Exp $

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
  AmberFF(AmberFF&);

  bool specificSetup();
  double getStretchEnergy() const;
  double getBendEnergy() const;
  double getTorsionEnergy() const;
  double getNonbondedEnergy() const;
  double getESEnergy() const;
  double getVdWEnergy() const;
  bool hasInitializedParameters() const;
};
