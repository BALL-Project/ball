// $Id: conjugateGradient.h,v 1.10 2001/07/11 23:59:32 amoll Exp $ 

#ifndef BALL_MOLMEC_MINIMIZATION_CONJUGATEGRADIENT_H 
#define BALL_MOLMEC_MINIMIZATION_CONJUGATEGRADIENT_H 

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifdef BALL_INCLUDE_IEEEFP
#	include <ieeefp.h> 
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#	include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_FORCEFIELDPARAMETERS_H
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

#ifndef	BALL_MOLMEC_MINIMIZATION_ENERGYMINIMIZER_H
#	include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
#endif

#ifndef BALL_MOLMEC_COMMON_SNAPSHOT_H
# include <BALL/MOLMEC/COMMON/snapShot.h>
#endif

#ifndef BALL_MOLMEC_COMMON_GRADIENT_H
# include <BALL/MOLMEC/COMMON/gradient.h>
#endif


namespace BALL 
{ 

	/**	Conjugate gradient (CG) minimizer. 
			{\bf Definition:} \URL{BALL/MOLMEC/MINIMIZATION/conjugateGradient.h} \\
			A conjugate gradient minimizer for geometry optimisation.
	*/
  class ConjugateGradientMinimizer 
		: public EnergyMinimizer
	{
    public:

		/**	@name	Enums	
		*/
		//@{

		/**	The different conjugate gradient methods implemented.
				@see	updateDirection for details on the implementation and references
		*/
		enum UpdateMethod
		{
			/**	Polak-Ribiere method
			*/
			POLAK_RIBIERE,

			/**	Fletcher-Reeves method
			*/
			FLETCHER_REEVES,

			/**	Shanno
			*/
			SHANNO
		};

		//@}	
    /**	@name	Options and Defaults
    */
    //@{

    /**	Options names
    */
    struct Option
		{
      /** The initial step length used in the line search 
      */
      static const char* STEP_LENGTH; 
    };

		/**	Defaults for all options
		*/
    struct Default
    {
      /** The initial step length used in the line search 
      */
      static double STEP_LENGTH; 
    };

    //@}

    BALL_CREATE_DEEP(ConjugateGradientMinimizer)

    /**	@name	Constructors and Destructors	
    */
    //@{

    /**	Default constructor.
    */
    ConjugateGradientMinimizer();

    /**	Constructor expecting a valid force field 
    */
    ConjugateGradientMinimizer(ForceField& force_field);

    /**	Constructor expecting a valid force field and a snapshot manager
    */
    ConjugateGradientMinimizer(ForceField& force_field,SnapShotManager *ssm);

    /**	Constructor expecting a valid force field, a snapshot manager  and options 
    */
    ConjugateGradientMinimizer
			(ForceField& force_field, SnapShotManager* ssm, const Options& options);

    /**	Constructor expecting a valid force field and options 
    */
    ConjugateGradientMinimizer(ForceField& force_field, const Options& options);

    /**	Copy constructor
    */
    ConjugateGradientMinimizer(const ConjugateGradientMinimizer& rhs, bool deep = true);

    /**	Destructor.
    */
    virtual ~ConjugateGradientMinimizer();

    //@}
    /**	@name	Assignments 
    */
    //@{

    /**	Assignment operator
    */
    ConjugateGradientMinimizer& operator = (const ConjugateGradientMinimizer& rhs);

    //@}
    /**	@name	Setup methods. They do all necessary preparations. 
    */
    //@{

    /**	Specific setup
    */
    virtual bool specificSetup();

    //@}
    /**	@name	Accessors 
    */
    //@{

    /** Set explicitly the value for control parameter step_length_
    */
    void setStepLength(double  value);

    /** Get the current value of option step_length_
    */
    double getStepLength() const; 

		/**	Calculate the next step.
				This method calculates direction and step length for the
				next optimization step. The step is not yet performed.
				This method first tries to determine a better solution 
				with a line search along the current search direction.
				If this fails, it resets the search direction to the negative 
				current gradient and performs a new line search.
				If this fails again, the step width is reduced by an order of
				magnitude and a last line search is performed along ths negative
				gradient.
				The best solution found is stored in {\tt lambda_}.
				@return	bool {\bf true} if the line search found an acceptable solution
				@see	EnergyMinimizer::findStep
		*/
		virtual bool findStep();
			
		/**	Update the search direction.
				This method updates the search direction.
				It uses the Fletcher-Reeves conjugate gradient.
		*/
		virtual void updateDirection();

    /**	Minimize the energy of the system.
				This method executes at most {\tt iterations} minimization steps.
				If the number of iterations is not given, the number specified in the
				options is taken.
				@param	iterations the maximum number of iterations
				@param	restart {\bf true} if the minimization is restarted
				@see		EnergyMinimizer::minimize
    */
    virtual bool minimize(Size iterations = 0, bool restart = false); 

    //@}
    /**	@name	Public Attributes
    */
    //@{

    /**	Force field options
    */
    Options	options;
		

    protected:

    //@}
    /*_	@name	Protected Attributes 
		*/
    //_@{

		Size number_of_atoms_;

    /*_ The step length used in the line search 
    */
    double step_; 

    /*_ The optimal lambda in the last line search 
    */
    double lambda_; 

		/*_	The update method used for the CG
		*/
		UpdateMethod method_;

    //_@}

	};

} // end of namespace BALL

#endif // BALL_MOLMEC_MINIMIZATION_CONJUGATEGRADIENT_H 
