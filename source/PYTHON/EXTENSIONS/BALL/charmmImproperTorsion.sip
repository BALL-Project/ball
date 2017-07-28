class CharmmImproperTorsion
	: ForceFieldComponent
{
%TypeHeaderCode
	#include <BALL/MOLMEC/CHARMM/charmmImproperTorsion.h>
%End
	public:
	CharmmImproperTorsion();
	CharmmImproperTorsion(ForceField&);
	CharmmImproperTorsion(const CharmmImproperTorsion&);
	~CharmmImproperTorsion();
	virtual bool setup() throw(TooManyErrors);
	virtual double updateEnergy();
	virtual void updateForces();
};
