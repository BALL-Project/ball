// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: microCanonicalMD.sip,v 1.3 2003/10/29 22:54:41 oliver Exp $
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
	MicroCanonicalMD(const MicroCanonicalMD& /rhs/, bool /deep/ = true); 
	~MicroCanonicalMD(); 
	// MicroCanonicalMD& operator = (const MicroCanonicalMD& /rhs/); 
	virtual bool setup(ForceField&/my_force_field/, SnapShotManager* /ssm/); 
	virtual bool setup(ForceField&/my_force_field/, SnapShotManager* /ssm/, const Options& /myoptions/); 
	virtual bool specificSetup();
	virtual void setTimeStep(double /step/); 
	virtual void simulate(bool /restart/ = false);
	virtual void simulateIterations(Size /number/, bool /restart/ = false);
	virtual void simulateTime(double /simulation_time/, bool /restart/ = false); 
};
