// $Id: microCanonicalMD.h,v 1.3 2000/03/26 12:49:30 oliver Exp $
// Microcanonical MD: A class for doing molecular dynamics simulations      
// according to the principle of a microcanonical ensemble (NVE), i.e., 
// the total energy of the system is kept constant.
// Numerical integration for new atom positions is done via the
// Velocity Verlet method. 


#ifndef BALL_MOLMEC_MDSIMULATION_MICROCANONICALMD_H   
#define BALL_MOLMEC_MDSIMULATION_MICROCANONICALMD_H   


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

#ifndef BALL_MOLMEC_COMMON_SNAPSHOT_H
#	include <BALL/MOLMEC/COMMON/snapShot.h>
#endif


// STL include commands 
#include <vector>

namespace BALL 
  {
  class MicroCanonicalMD : public MolecularDynamics
    {
    public: 

    /** A local auxiliary class
    */
    struct Aux_Factors
      {
      double factor1,factor2; 
      }; 
    
    /** @name Constructors and Destructors
    */
    //@{

    BALL_CREATE(MicroCanonicalMD)

    /** The default constructor with no arguments
    */
    MicroCanonicalMD(); 

    /** This constructor expects a force field.
        The force field's options are used and no snapshots are taken  
    */
    MicroCanonicalMD(ForceField &myforcefield); 

    /** This constructor expects a force field  and a snapshot-manager 
        The force field's options are used 
    */
    MicroCanonicalMD(ForceField &myforcefield, SnapShotManager *ssm); 

    /** This constructor wants a force field, a snapshot manager  and new options 
    */
    MicroCanonicalMD(ForceField &myforcefield, SnapShotManager *ssm, const Options &myoptions);

    /** The standard copy constructor
    */
    MicroCanonicalMD(const MicroCanonicalMD &rhs, bool deep = true); 

    /** The destructor
    */
    virtual ~MicroCanonicalMD(); 

    //@}


    /** @name Assignments
    */

    //@{
    /** Assignment operator
    */
    MicroCanonicalMD &operator=(const MicroCanonicalMD &rhs); 
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

    //@{
    /**  This method does the actual simulation stuff
         It runs for the number of iterations currently stored in
         'number_of_iterations_'
    */
    virtual void simulate(bool restart = false);

    /**  This method does the actual simulation stuff
         It runs for the indicated number of iterations
    */
    virtual void simulateIterations(Size number,bool restart = false);

    /**  This method does the actual simulation stuff
         It runs for the indicated time in picoseconds  
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

    /**  This vector contains special, precomputed factors 
    */
    vector<Aux_Factors> mass_factor_; 

    //@} 

    };  // end of class MicroCanonicalMD  

  } // end of namespace Ball

#endif // _BALL_MOLMEC_MICROCANONICALMD_H  
