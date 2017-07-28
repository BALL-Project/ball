class CharmmNonBonded
	: ForceFieldComponent
{
%TypeHeaderCode
	#include <BALL/MOLMEC/CHARMM/charmmNonBonded.h>
%End
	public:
	CharmmNonBonded();
	CharmmNonBonded(ForceField&);
	CharmmNonBonded(const CharmmNonBonded&);
	~CharmmNonBonded() throw();
	virtual bool setup() throw(TooManyErrors);
	virtual double updateEnergy() throw();
	virtual void updateForces() throw();
	virtual double getElectrostaticEnergy() const throw();
	virtual double getVdwEnergy() const throw();
	virtual double getSolvationEnergy() const throw();
};
