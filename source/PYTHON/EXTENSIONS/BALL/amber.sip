class AmberFF
	: ForceField
{
%TypeHeaderCode
	#include <BALL/MOLMEC/AMBER/amber.h>
%End
	public:
	AmberFF();
	AmberFF(System&);
	AmberFF(System&, Options&);
	AmberFF(AmberFF&);
	virtual ~AmberFF();

	virtual void clear();
	virtual bool specificSetup() throw(TooManyErrors);
	double getStretchEnergy() const;
	double getBendEnergy() const;
	double getTorsionEnergy() const;
	double getNonbondedEnergy() const;
	double getESEnergy() const;
	double getVdWEnergy() const;
	bool hasInitializedParameters() const;
	Size getUpdateFrequency() const;
	virtual String getResults() const;
};
