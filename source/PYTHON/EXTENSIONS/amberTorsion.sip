// $Id: amberTorsion.sip,v 1.2 2003/06/18 15:50:43 anker Exp $

class AmberTorsion 
  : ForceFieldComponent
{
%HeaderCode
	#include <BALL/MOLMEC/AMBER/amberTorsion.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE(AmberTorsion)
  AmberTorsion();
  AmberTorsion(ForceField& /force_field/);
  AmberTorsion(const AmberTorsion& /amber_stretch/);
  ~AmberTorsion();
  virtual bool setup();
  virtual double updateEnergy();
  virtual void updateForces();
};
 
