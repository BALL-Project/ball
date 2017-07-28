class AmberTorsion
	: ForceFieldComponent
{
%TypeHeaderCode
	#include <BALL/MOLMEC/AMBER/amberTorsion.h>
%End
	public:
	AmberTorsion();
	AmberTorsion(ForceField&);
	AmberTorsion(const AmberTorsion&);
	virtual ~AmberTorsion();

	struct SingleAmberTorsion {
		SingleAmberTorsion();
		SingleAmberTorsion(SingleData& t);
	};

	virtual bool setup() throw(TooManyErrors);
	virtual double updateEnergy();
	virtual void updateForces();
};
