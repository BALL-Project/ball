// $Id: amberStretch.sip,v 1.1 2001/09/03 16:53:03 oliver Exp $

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
  
