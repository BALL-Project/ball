class MMFF94
	: ForceField
{
%TypeHeaderCode
	#include <BALL/MOLMEC/MMFF94/MMFF94.h>
%End
	public:
	MMFF94();
	MMFF94(System&);
	MMFF94(System&, Options&);
	MMFF94(MMFF94&);

	bool specificSetup() throw(TooManyErrors);

	double getStretchEnergy() const;
	double getBendEnergy() const;
	double getStretchBendEnergy() const;
	double getTorsionEnergy() const;
	double getNonbondedEnergy() const;
	double getESEnergy() const;
	double getVdWEnergy() const;
	double getPlaneEnergy() const;
	bool hasInitializedParameters() const;
	Size getUpdateFrequency() const;
	virtual String getResults() const throw();
	bool isInOneAromaticRing(const Bond& bond) const;
	bool checkAtomType(Atom& atom);
};
