// $Id: charmmImproperTorsion.sip,v 1.1 2001/09/03 16:53:05 oliver Exp $

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
  CharmmImproperTorsion(const CharmmImproperTorsion& /charmm_stretch/, bool /clone_deep/ = true);
  ~CharmmImproperTorsion();
  virtual bool setup();
  virtual double updateEnergy();
  virtual void updateForces();
};
  
