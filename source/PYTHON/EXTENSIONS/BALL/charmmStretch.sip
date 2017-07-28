class CharmmStretch
	: ForceFieldComponent
{
%TypeHeaderCode
	#include <BALL/MOLMEC/CHARMM/charmmStretch.h>
%End
	public:
	CharmmStretch();
	CharmmStretch(ForceField&);
	CharmmStretch(const CharmmStretch&);
	~CharmmStretch();
	virtual bool setup() throw(TooManyErrors);
	virtual double updateEnergy();
	virtual void updateForces();
};
