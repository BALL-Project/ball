// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: canonicalMD.sip,v 1.4 2004/04/17 16:54:27 oliver Exp $
//

class CanonicalMD 
	: MolecularDynamics
{
%TypeHeaderCode
	#include <BALL/MOLMEC/MDSIMULATION/canonicalMD.h>
	using namespace BALL;
%End
    public: 
    CanonicalMD(); 
    CanonicalMD(ForceField& /myforcefield/); 
    CanonicalMD(ForceField& /myforcefield/,SnapShotManager* /ssm/); 
    CanonicalMD(ForceField& /myforcefield/,SnapShotManager* /ssm/, const Options& /options/);
    CanonicalMD(const CanonicalMD& /rhs/); 
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
