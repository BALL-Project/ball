// $Id: charmmBend.sip,v 1.1 2001/09/03 16:53:05 oliver Exp $

class CharmmBend 
  : ForceFieldComponent
{
%HeaderCode
	#include <BALL/MOLMEC/CHARMM/charmmBend.h>
	using namespace BALL;
%End
  public:
  CharmmBend();
  CharmmBend(ForceField& /force_field/);
  CharmmBend(const CharmmBend& /charmm_stretch/, bool /clone_deep/ = true);
  ~CharmmBend();
  virtual bool setup();
  virtual double updateEnergy();
  virtual void updateForces();
};
