// $Id: amberTorsion.sip,v 1.1 2000/06/30 05:58:45 oliver Exp $

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
  AmberTorsion(const AmberTorsion& /amber_stretch/, bool /clone_deep/ = true);
  ~AmberTorsion();
  virtual bool setup();
  virtual double updateEnergy();
  virtual void updateForces();
};
 
