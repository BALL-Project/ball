// $Id: canonicalMD.h,v 1.7 2000/08/30 19:58:19 oliver Exp $

#ifndef BALL_MOLMEC_MDSIMULATION_CANONICALMD_H   
#define BALL_MOLMEC_MDSIMULATION_CANONICALMD_H   


// Include all necessary BALL headers 
#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_KERNEL_ATOM_H         
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H         
#	include <BALL/MATHS/vector3.h>
#endif


#ifndef BALL_DATATYPE_OPTIONS_H
#	include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_FORCEFIELDPARAMETER_H
#	include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
#	include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

#ifndef BALL_MOLMEC_COMMON_PERIODIC_BOUNDARY_H
#	include <BALL/MOLMEC/COMMON/periodicBoundary.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
#	include <BALL/MOLMEC/COMMON/forceField.h>
#endif

#ifndef BALL_MOLMEC_MDSIMULATION_MOLECULARDYNAMICS_H
#	include <BALL/MOLMEC/MDSIMULATION/molecularDynamics.h>
#endif


// STL include commands 
#include <vector>

namespace BALL 
{

	/** a MD simulation in the canoncial ensemble.
		A class for doing molecular dynamics simulations      
		according to the principle of a canonical ensemble (NVT), i.e., 
		the temperature is kept constant. 
		This is achieved via the velocity rescaling proposed by
		Berendsen et al., J. Chem. Physics, 81:3684, 1984.
		Note that this approach does  not give all properties of a
		true canonical system. In particular, the heat capacity 
		cannot be calculated! 
		\\
		{\bf Definition:}\URL{BALL/MOLMEC/MDSIMULATION/canonicalMD.h}
		\\
	*/
  class CanonicalMD 
		: public MolecularDynamics
	{
    public: 

    /** A local auxiliary class
    */
    struct Aux_Factors
    {
      double factor1, factor2; 
    }; 
    
    /** @name Constructors and Destructors
    */
    //@{

    // BALL_CREATE_DEEP(CanonicalMD)

    /** The default constructor with no arguments
    */
    CanonicalMD(); 

    /** This constructor expects a force field 
        The force field's options are used and no saving  of snapshots is done 
    */
    CanonicalMD(ForceField& myforcefield); 

    /** This constructor expects a force field and a snapshot manager 
        The force field's options are used. 
    */
    CanonicalMD(ForceField& myforcefield, SnapShotManager* ssm); 

    /** This constructor wants a force field, a snapshot manager  and new options 
    */
    CanonicalMD(ForceField &myforcefield,SnapShotManager *ssm, const Options &myoptions);

    /** The standard copy constructor
    */
    CanonicalMD(const CanonicalMD &rhs, bool deep = true); 

    /** The destructor
    */
    virtual ~CanonicalMD(); 

    //@}


    /** @name Assignments
    */

    //@{
    /** Assignment operator
    */
    CanonicalMD &operator=(const CanonicalMD &rhs); 
    //@}


    /** Accessors
    */
    //@{
    /** This method sets a new relaxation time for the coupling to an
        external heat bath
    */
    void setBathRelaxationTime(double time);

    /** This method gets the current value for heat bath coupling
    */
    double getBathRelaxationTime() const; 
    //@}

    /** @name Setup methods
    */
    //@{

    /** This method does general setup things 
    */
    virtual bool setup(ForceField &myforcefield, SnapShotManager *ssm); 

    /** This method does general setup things 
    */
    virtual bool setup(ForceField &myforcefield, SnapShotManager *ssm, const Options &myoptions); 

    /** This method is meant for additional preparations  apart from those
       done in setup 
    */
    virtual bool specificSetup();

    //@} 

    /** @name Accessors
    */

    /** Set a new time step  for the numerical integration 
    */
    virtual void setTimeStep(double time); 

    //@{
    /**  This method does the actual simulation stuff
         It runs for getMaximalNumberIterations() iterations.
         If restart is true, the counting of iterations starts with the
         number of the last iteration in the previous run.  
    */
    virtual void simulate(bool restart = false);

    /**  This method does the actual simulation stuff. 
         It runs for the indicated number of iterations. 
         If restart is true, the counting of iterations starts with the
         number of the last iteration in the previous run.  
          
    */
    virtual void simulateIterations(Size number,bool restart = false);

    /**  This method does the actual simulation stuff. 
         It runs for the indicated time in picoseconds. 
         If restart is true, the counting of iterations starts with the
         number of the last iteration in the previous run.  
    */
    virtual void simulateTime(double simulation_time,bool restart = false); 

    //@}

    protected:
    /*_ @name Protected methods
    */
    //_@{

    /*_ A protected method for calculating some
        factors that are needed all the time
    */
    void calculateFactors();

    //_@}

    /*_  @name Protected Attributes
    */
    //@{
    /*_  The coupling parameter to the heat  bath
    */
    double bath_relaxation_time_; 

    /*_  This vector contains special precomputed factors 
    */
    vector<Aux_Factors> mass_factor_; 

    //@} 

    };  // end of class CanonicalMD  

  } // end of namespace BALL 

#endif // _BALL_MOLMEC_CANONICALMD_H  
