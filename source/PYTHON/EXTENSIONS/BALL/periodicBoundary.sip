class PeriodicBoundary
{
%TypeHeaderCode
	#include <BALL/MOLMEC/COMMON/periodicBoundary.h>
%End
	public:
	PeriodicBoundary();
	PeriodicBoundary(const ForceField&);
	PeriodicBoundary(const PeriodicBoundary&);

	bool setup();
	int generateMoleculesVector();
	void enable();
	void disable();
	SimpleBox3 getBox() const;
	void setBox(const SimpleBox3&);
	int addSolvent(const String& /In/);
	int removeSolvent();
	bool isEnabled() const;
	void updateMolecules();
	Options* options;
};
