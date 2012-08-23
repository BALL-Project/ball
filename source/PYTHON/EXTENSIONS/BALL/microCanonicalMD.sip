// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class MicroCanonicalMD
	: MolecularDynamics
{
%TypeHeaderCode
  #include <BALL/MOLMEC/MDSIMULATION/microCanonicalMD.h>
  using namespace BALL;
%End
	public:
	// BALL_CREATE(MicroCanonicalMD)
	MicroCanonicalMD();
	MicroCanonicalMD(ForceField&);
	MicroCanonicalMD(ForceField&, SnapShotManager*);
	MicroCanonicalMD(ForceField&, SnapShotManager*, const Options&);
	MicroCanonicalMD(const MicroCanonicalMD&);
	~MicroCanonicalMD();
	// MicroCanonicalMD& operator = (const MicroCanonicalMD&); 
	virtual bool setup(ForceField&, SnapShotManager*);
	virtual bool setup(ForceField&, SnapShotManager*, const Options&);
	virtual bool specificSetup();
	virtual void setTimeStep(double);
	virtual bool simulate(bool restart = false);
	virtual bool simulateIterations(Size, bool restart = false);
	virtual bool simulateTime(double, bool restart = false);
};
