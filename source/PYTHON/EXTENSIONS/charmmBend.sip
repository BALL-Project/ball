// $Id: charmmBend.sip,v 1.2 2003/06/18 15:49:15 anker Exp $

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
  CharmmBend(const CharmmBend& /charmm_stretch/);
  ~CharmmBend();
  virtual bool setup();
  virtual double updateEnergy();
  virtual void updateForces();
};
