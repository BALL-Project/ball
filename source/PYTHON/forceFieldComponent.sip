class ForceFieldComponent 
{
%HeaderCode
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
using namespace BALL;
%End
  public:
  ForceFieldComponent();
  ForceFieldComponent(ForceField*);
  ForceFieldComponent(const ForceFieldComponent&, bool);

  virtual bool setup();
  ForceField* getForceField() const;
  void setForceField(ForceField*);
  void setName(const String&);
  String getName() const;
  virtual double getEnergy() const;
  virtual double updateEnergy();
  virtual void updateForces();
};
