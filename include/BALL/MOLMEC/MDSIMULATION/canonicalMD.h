// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: canonicalMD.h,v 1.24 2004/04/17 14:14:52 oliver Exp $
//

#ifndef BALL_MOLMEC_MDSIMULATION_CANONICALMD_H   
#define BALL_MOLMEC_MDSIMULATION_CANONICALMD_H   

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


#include <vector>

namespace BALL 
{
	/** A MD simulation in the canoncial ensemble.
			A class for doing molecular dynamics simulations      
			according to the principle of a canonical ensemble (NVT), i.e., 
			the temperature is kept constant. 
			This is achieved via the velocity rescaling proposed by
			Berendsen et al., J. Chem. Physics, 81:3684, 1984.
			Note that this approach does  not give all properties of a
			true canonical system. In particular, the heat capacity 
			cannot be calculated!	
			\par
			The temperature is set through the \link MDSimulation::setReferenceTemperature \endlink method.
			
    	\ingroup  MDSimulation
	*/
  class CanonicalMD 
		: public MolecularDynamics
	{
    public: 

    BALL_CREATE(CanonicalMD)

    /** @name Constructors and Destructor
    */
    //@{


    ///
    CanonicalMD()	throw();

    /** This constructor expects a force field .
        The force field's options are used and no saving of snapshots is done.
				@param myforcefield the forcefield we need for the simulation
    */
    CanonicalMD(ForceField& myforcefield)	throw();

    /** This constructor expects a force field and a snapshot manager.
        The force field's options are used. 
				@param myforcefield the forcefield to use in the simulation
				@param ssm  a pointer to the SnapShotManager which will be used to 
				create trajectory files
    */
    CanonicalMD(ForceField& force_field, SnapShotManager* ssm) throw();

    /** This constructor wants a force field, a snapshot manager and new options.
				@param myforcefield the forcefield which is to be used in the	simulation
				@param ssm a pointer to the SnapShotManager used for creating 
								trajectory files
				@param myoptions arbitrary options that are to be used by this 
								instance instead of those defined therein
    */
    CanonicalMD(ForceField& myforcefield, SnapShotManager* ssm,	
								const Options& options) throw();

    /// Copy constructor
    CanonicalMD(const CanonicalMD& rhs) throw();

    /// Destructor
    virtual ~CanonicalMD() throw();
    //@}

    /** @name Assignment  */
    //@{

    /// Assignment operator
    CanonicalMD &operator = (const CanonicalMD& rhs) throw();
    //@}

    /** @name Setup methods
    */
    //@{

    /** This method does general setup things
				@param ssm if no snapshots during the simulations are required, leave this at the default (NULL pointer)
				@param force_field the forcefield, which has to be bound to a system
    */
    virtual bool setup(ForceField& force_field, SnapShotManager* ssm = 0)
			throw();

    /** This method does general setup things. 
				This method also allows the assignment of options for the setup.
    */
    virtual bool setup(ForceField& force_field, SnapShotManager* ssm,
											const Options& options)
			throw();

    /** This method is meant for additional preparations apart from those done in setup.
				@return bool, <b>true</b> if specificSetup() was successful
    */
    virtual bool specificSetup() throw();
    //@} 

    /** @name Accessors
		*/
		//@{

    /** This method sets a new relaxation time for the coupling to an external heat bath.
				The readjustment to the external bath is not done after every step in order
				to save compute time. Instead, it is coupled periodically after the
				time specified here has elapsed.
				@param time the time in <em>ps</em>
    */
    void setBathRelaxationTime(double time)	throw();

    /** This method gets the current value for heat bath coupling.
				@return the bath relaxation time in <em>ps</em>
    */
    double getBathRelaxationTime() const throw();

    /** Set a new time step for the numerical integration.
    */
    virtual void setTimeStep(double time) throw();

    /**  This method does the actual simulation stuff. 
         It runs for the indicated number of iterations. 
         If restart is true, the counting of iterations starts with the 
         number of the last iteration in the previous run.  
				 @param number the number of iterations that have to be simulated 
				 @param restart flag for restarting the simulation
    */
    virtual void simulateIterations(Size number, bool restart = false) throw();

    //@}

    protected:
		
		/// Helper class containing auxiliary factors.
    struct AuxFactors
    {
      double factor1;
			double factor2; 
    }; 
    
    /** @name Protected methods */
    //@{

    /** A protected method for calculating some factors that are needed all the time.
    */
    void calculateFactors_() throw();
    //@}

    /**  @name Protected Attributes */
    //@{

    ///  The coupling parameter to the heat bath
    double bath_relaxation_time_; 

    /// This vector contains special precomputed factors 
		std::vector<AuxFactors> mass_factor_; 
    //_@} 

	};  //class CanonicalMD  

} // namespace BALL 

#endif // BALL_MOLMEC_MDSIMULATION_CANONICALMD_H
