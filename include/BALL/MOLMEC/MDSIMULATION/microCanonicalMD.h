// $Id: microCanonicalMD.h,v 1.6 2000/12/18 13:59:03 anker Exp $
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


// STL include commands 
#include <vector>

namespace BALL 
{

	/** Microcanonical MD: A class for doing molecular dynamics simulations      
			according to the principle of a microcanonical ensemble (NVE), i.e.,
			the total energy of the system is kept constant. Numerical
			integration for new atom positions is done via the Velocity Verlet
			method. 
			\\
			{\bf Definition:} \URL{BALL/MOLMEC/MDSIMULATION/microCanonicalMD.h>
			\\
	*/

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

    BALL_CREATE_DEEP(MicroCanonicalMD)

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

		/** This constructor wants a force field, a snapshot manager and new
				options 
    */
		MicroCanonicalMD(ForceField &myforcefield, SnapShotManager *ssm, 
				const Options &myoptions);

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
    virtual bool setup(ForceField &myforcefield, SnapShotManager *ssm,
				const Options &myoptions); 

    /** This method is meant for additional preparations  apart from those
       done in setup 
    */
    virtual bool specificSetup();

    //@} 

    /** @name Accessors
    */

    /** Choose a new time step
    */
    virtual void setTimeStep(double step); 

    //@{
    /**  This method does the actual simulation stuff
         It runs for getMaximalNumberOfIterations() iterations. 
         restart = true means that the counting of iterations is
                   continued from the previous run. 
    */
    virtual void simulate(bool restart = false);

    /**  This method does the actual simulation stuff. 
         It runs for the indicated number of iterations.
         restart = true means that the counting of iterations is
                   continued from the previous run. 
    */
    virtual void simulateIterations(Size number,bool restart = false);

    /**  This method does the actual simulation stuff. 
         It runs for the indicated time in picoseconds . 
         restart = true means that the counting of iterations is
                   continued from the previous run. 
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

  } // end of namespace BALL

#endif // _BALL_MOLMEC_MICROCANONICALMD_H  
