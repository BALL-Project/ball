class ForceFieldComponent 
{
%HeaderCode
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
using namespace BALL;
%End
  public:
  ForceFieldComponent();
  ForceFieldComponent(ForceField& /force_field/);
  ForceFieldComponent(const ForceFieldComponent& /force_field/, bool /deep/ = true);
  virtual bool setup();
  ForceField* getForceField() const;
  void setForceField(ForceField& /force_field/);
  void setName(const String&);
  String getName() const;
  virtual double getEnergy() const;
  virtual double updateEnergy();
  virtual void updateForces();
};
