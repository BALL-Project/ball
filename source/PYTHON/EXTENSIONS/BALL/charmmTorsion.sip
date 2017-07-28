class CharmmTorsion
	: ForceFieldComponent
{
%TypeHeaderCode
	#include <BALL/MOLMEC/CHARMM/charmmTorsion.h>
%End

	public:
	CharmmTorsion();
	CharmmTorsion(ForceField&);
	CharmmTorsion(const CharmmTorsion&);
	~CharmmTorsion();
	virtual bool setup() throw(TooManyErrors);
	virtual double updateEnergy();
	virtual void updateForces();
};
