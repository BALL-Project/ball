// $Id: microCanonicalMD.sip,v 1.1 2001/09/03 16:53:09 oliver Exp $

class MicroCanonicalMD 
	: MolecularDynamics
{
%HeaderCode
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
