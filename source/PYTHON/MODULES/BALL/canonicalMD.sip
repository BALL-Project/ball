// $Id: canonicalMD.sip,v 1.1 2000/07/18 17:02:13 oliver Exp $

class CanonicalMD 
	: MolecularDynamics
{
%HeaderCode
	#include <BALL/MOLMEC/MDSIMULATION/canonicalMD.h>
	using namespace BALL;
%End
    public: 
    CanonicalMD(); 
    CanonicalMD(ForceField& /myforcefield/); 
    CanonicalMD(ForceField& /myforcefield/,SnapShotManager* /ssm/); 
    CanonicalMD(ForceField& /myforcefield/,SnapShotManager* /ssm/, const Options& /options/);
    CanonicalMD(const CanonicalMD& /rhs/, bool /deep/ = true); 
    ~CanonicalMD(); 
    // CanonicalMD &operator=(const CanonicalMD& /rhs/); 
    void setBathRelaxationTime(double /time/);
    double getBathRelaxationTime() const; 
    virtual bool setup(ForceField& /myforcefield/, SnapShotManager* /ssm/); 
    virtual bool setup(ForceField& /myforcefield/, SnapShotManager* /ssm/, const Options& /options/); 
    virtual bool specificSetup();
    virtual void setTimeStep(double /time/); 
    virtual void simulate(bool /restart/ = false);
    virtual void simulateIterations(Size /number/, bool /restart/ = false);
    virtual void simulateTime(double /simulation_time/, bool /restart/ = false); 
};
