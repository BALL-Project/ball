// $Id: canonicalMD.sip,v 1.1 2001/09/03 16:53:05 oliver Exp $

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
    ~CanonicalMD() throw(); 
    // CanonicalMD &operator=(const CanonicalMD& /rhs/); 
    void setBathRelaxationTime(double /time/);
    double getBathRelaxationTime() const; 
    virtual bool setup(ForceField& /myforcefield/, SnapShotManager* /ssm/) throw(); 
    virtual bool setup(ForceField& /myforcefield/, SnapShotManager* /ssm/, const Options& /options/) throw(); 
    virtual bool specificSetup() throw();
    virtual void setTimeStep(double /time/) throw(); 
    virtual void simulate(bool /restart/ = false) throw();
    virtual void simulateIterations(Size /number/, bool /restart/ = false) throw();
    virtual void simulateTime(double /simulation_time/, bool /restart/ = false) throw(); 
};
