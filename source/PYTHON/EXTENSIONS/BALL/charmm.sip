class CharmmFF
	: ForceField
{
%TypeHeaderCode
	#include <BALL/MOLMEC/CHARMM/charmm.h>
%End
	public:

	CharmmFF();
	CharmmFF(System&);
	CharmmFF(System&, const Options&);
	CharmmFF(const CharmmFF&);
	~CharmmFF();
	virtual bool specificSetup() throw(TooManyErrors);
	double getStretchEnergy() const;
	double getBendEnergy() const;
	double getTorsionEnergy() const;
	double getImproperTorsionEnergy() const;
	double getProperTorsionEnergy() const;
	double getNonbondedEnergy() const;
	double getVdWEnergy() const;
	double getESEnergy() const;
	double getSolvationEnergy() const;
	bool hasInitializedParameters() const;
};
