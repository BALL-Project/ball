// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: forceFieldComponent.sip,v 1.4 2003/10/29 22:54:40 oliver Exp $
//

class ForceFieldComponent 
{
%TypeHeaderCode
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
using namespace BALL;
%End
  public:
  ForceFieldComponent();
  ForceFieldComponent(ForceField& /force_field/);
  ForceFieldComponent(const ForceFieldComponent& /force_field/);
  virtual bool setup();
  ForceField* getForceField() const;
  void setForceField(ForceField& /force_field/);
  void setName(const String&);
  String getName() const;
  virtual double getEnergy() const;
  virtual double updateEnergy();
  virtual void updateForces();
};
