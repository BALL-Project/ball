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

  virtual bool specificSetup();
  double getStretchEnergy() const;
  double getBendEnergy() const;
  double getTorsionEnergy() const;
  double getNonbondedEnergy() const;
  double getESEnergy() const;
  double getVdWEnergy() const;
  bool hasInitializedParameters() const;
 };
