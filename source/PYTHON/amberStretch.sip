// $Id: amberStretch.sip,v 1.1 2000/06/30 05:58:44 oliver Exp $

class AmberStretch 
  : ForceFieldComponent
{
%HeaderCode
	#include <BALL/MOLMEC/AMBER/amberStretch.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE(AmberStretch)
  AmberStretch();
  AmberStretch(ForceField& /force_field/);
  AmberStretch(const AmberStretch& /amber_stretch/, bool /clone_deep/ = true);
  ~AmberStretch();
  virtual bool setup();
  virtual double updateEnergy();
  virtual void updateForces();
};
  
