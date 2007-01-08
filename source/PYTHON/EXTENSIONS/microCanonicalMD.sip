// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: microCanonicalMD.sip,v 1.5 2005/01/24 16:59:23 amoll Exp $
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
	MicroCanonicalMD(ForceField&/my_force_field/); 
	MicroCanonicalMD(ForceField&/my_force_field/, SnapShotManager* /ssm/); 
	MicroCanonicalMD(ForceField&/my_force_field/, SnapShotManager* /ssm/, const Options& /myoptions/);
	MicroCanonicalMD(const MicroCanonicalMD& /rhs/); 
	~MicroCanonicalMD(); 
	// MicroCanonicalMD& operator = (const MicroCanonicalMD& /rhs/); 
	virtual bool setup(ForceField&/my_force_field/, SnapShotManager* /ssm/); 
	virtual bool setup(ForceField&/my_force_field/, SnapShotManager* /ssm/, const Options& /myoptions/); 
	virtual bool specificSetup();
	virtual void setTimeStep(double /step/); 
	virtual bool simulate(bool /restart/ = false);
	virtual bool simulateIterations(Size /number/, bool /restart/ = false);
	virtual bool simulateTime(double /simulation_time/, bool /restart/ = false); 
};
