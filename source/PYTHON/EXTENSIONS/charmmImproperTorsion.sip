// $Id: charmmImproperTorsion.sip,v 1.2 2003/06/18 15:49:16 anker Exp $

class CharmmImproperTorsion 
  : ForceFieldComponent
{
%HeaderCode
	#include <BALL/MOLMEC/CHARMM/charmmImproperTorsion.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE(CharmmImproperTorsion)
  CharmmImproperTorsion();
  CharmmImproperTorsion(ForceField& /force_field/);
  CharmmImproperTorsion(const CharmmImproperTorsion& /charmm_stretch/);
  ~CharmmImproperTorsion();
  virtual bool setup();
  virtual double updateEnergy();
  virtual void updateForces();
};
  
