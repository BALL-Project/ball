// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
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
    CanonicalMD(ForceField&); 
    CanonicalMD(ForceField&,SnapShotManager*); 
    CanonicalMD(ForceField&,SnapShotManager*, const Options&);
    CanonicalMD(const CanonicalMD&); 
    ~CanonicalMD() throw(); 
    // CanonicalMD &operator=(const CanonicalMD&); 
    void setBathRelaxationTime(double);
    double getBathRelaxationTime() const; 
    virtual bool setup(ForceField&, SnapShotManager*) throw(); 
    virtual bool setup(ForceField&, SnapShotManager*, const Options&) throw(); 
    virtual bool specificSetup() throw();
    virtual void setTimeStep(double) throw(); 
    virtual bool simulate(bool restart = false) throw();
    virtual bool simulateIterations(Size, bool restart = false) throw();
    virtual bool simulateTime(double, bool restart = false) throw(); 
};
