class CharmmBend
	: ForceFieldComponent
{
%TypeHeaderCode
	#include <BALL/MOLMEC/CHARMM/charmmBend.h>
%End
	public:
	CharmmBend();
	CharmmBend(ForceField&);
	CharmmBend(const CharmmBend&);
	~CharmmBend();
	virtual bool setup() throw(TooManyErrors);
	virtual double updateEnergy();
	virtual void updateForces();
};
