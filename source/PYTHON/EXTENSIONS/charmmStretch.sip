// $Id: charmmStretch.sip,v 1.2 2003/06/18 15:49:16 anker Exp $

class CharmmStretch 
  : ForceFieldComponent
{
%HeaderCode
	#include <BALL/MOLMEC/CHARMM/charmmStretch.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE(CharmmStretch)
  CharmmStretch();
  CharmmStretch(ForceField& /force_field/);
  CharmmStretch(const CharmmStretch& /charmm_stretch/);
  ~CharmmStretch();
  virtual bool setup();
  virtual double updateEnergy();
  virtual void updateForces();
};
 
