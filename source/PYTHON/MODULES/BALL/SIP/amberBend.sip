// $Id: amberBend.sip,v 1.1 2000/07/18 21:27:00 oliver Exp $

class AmberBend 
  : ForceFieldComponent
{
%HeaderCode	
	#include <BALL/MOLMEC/AMBER/amberBend.h>
	using namespace BALL;
%End
  public:
  AmberBend();
  AmberBend(ForceField& /force_field/);
  AmberBend(const AmberBend& /amber_stretch/, bool /clone_deep/ = true);
  ~AmberBend();
  virtual bool setup();
  virtual double updateEnergy();
  virtual void updateForces();
};
