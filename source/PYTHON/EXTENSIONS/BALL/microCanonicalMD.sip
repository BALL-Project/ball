class MicroCanonicalMD
	: MolecularDynamics
{
%TypeHeaderCode
	#include <BALL/MOLMEC/MDSIMULATION/microCanonicalMD.h>
%End
	public:
	MicroCanonicalMD();
	MicroCanonicalMD(ForceField&);
	MicroCanonicalMD(ForceField&, SnapShotManager*);
	MicroCanonicalMD(ForceField&, SnapShotManager*, const Options&);
	MicroCanonicalMD(const MicroCanonicalMD&);
	~MicroCanonicalMD();
	virtual bool setup(ForceField&, SnapShotManager*);
	virtual bool setup(ForceField&, SnapShotManager*, const Options&);
	virtual bool specificSetup();
	virtual void setTimeStep(double);
	virtual bool simulate(bool restart = false);
	virtual bool simulateIterations(Size, bool restart = false);
	virtual bool simulateTime(double, bool restart = false);
};
