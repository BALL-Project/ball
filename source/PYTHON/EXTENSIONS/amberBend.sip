// $Id: amberBend.sip,v 1.2 2003/06/18 15:50:42 anker Exp $

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
  AmberBend(const AmberBend& /amber_stretch/);
  ~AmberBend();
  virtual bool setup();
  virtual double updateEnergy();
  virtual void updateForces();
};
