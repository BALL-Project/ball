// $Id: charmmStretch.sip,v 1.1 2000/07/18 21:27:20 oliver Exp $

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
  CharmmStretch(const CharmmStretch& /charmm_stretch/, bool /clone_deep/ = true);
  ~CharmmStretch();
  virtual bool setup();
  virtual double updateEnergy();
  virtual void updateForces();
};
 
