// $Id: conjugateGradient.h,v 1.2 1999/12/19 17:18:27 oliver Exp $ 
// A conjugate gradient minimizer for geometry optimisation

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
#       include <BALL/MOLMEC/COMMON/snapShot.h>
#endif


using namespace std;

namespace BALL 
  { 
  class ConjugateGradientMinimizer : public EnergyMinimizer
    {
    public:
    /**	@name	Enums
    */
    //@{

    /**	Error codes of the ForceField class
    */
    enum	ErrorCode 
      {
      ERROR__NONE	= -1,
                          ERROR__UNKNOWN,
			  ERROR__NOT_IMPLEMENTED,
  			  NUMBER_OF_ERRORS
      };

    //@}


    /**	@name	Options names         
    */
    //@{

    /**     Options names
    */
    struct Option
      {
      /** The alpha factor used for determining the step length
          This factor controls how much the energy decreases in an
          iteration.
      */
      static const char *LINE_SEARCH_ALPHA; 

      /** The beta  factor used for determining the step length
          This factor controls how much directional gradient reduction
          will be in an iteration. 
      */
      static const char *LINE_SEARCH_BETA; 

      /** The initial step length used in the line search 
      */
      static const char *STEP_LENGTH; 
      };

    struct Default
      {
      /** The alpha factor used for determining the step length
          This factor controls how much the energy decreases in an
          iteration.
      */
      static  float LINE_SEARCH_ALPHA;


      /** The beta  factor used for determining the step length
          This factor controls how much directional gradient reduction
          will be in an iteration.
      */
      static float LINE_SEARCH_BETA; 

      /** The initial step length used in the line search 
      */
      static float STEP_LENGTH; 
      };

    //@}



    /**	@name	Constructors and Destructors	
    */
    //@{
    BALL_CREATE(ConjugateGradientMinimizer)

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
    ConjugateGradientMinimizer(ForceField& force_field, SnapShotManager *ssm,
                                                        const Options& options);

    /**	Constructor expecting a valid force field and options 
    */
    ConjugateGradientMinimizer(ForceField& force_field, const Options& options);

    /**	Copy constructor
    */
    ConjugateGradientMinimizer(const ConjugateGradientMinimizer &rhs, bool deep = true);

    /**	Destructor.
    */
    virtual ~ConjugateGradientMinimizer();

    //@}


    /**	@name	Assignments 
    */
    //@{

    /**	Assignment operator
    */
    ConjugateGradientMinimizer &operator=(const ConjugateGradientMinimizer& rhs);

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
    void setStepLength(float  value);

    /** Set explicitly the value for control parameter line_search_alpha_
    */
    void setLineSearchAlpha(float  alpha);

    /** Set explicitly the value for control parameter line_search_beta_
    */
    void setLineSearchBeta(float  beta);

    /** Get the current value of option line_search_alpha_
    */
    float getLineSearchAlpha() const; 

    /** Get the current value of option line_search_beta_
    */
    float getLineSearchBeta() const; 

    /** Get the current value of option step_length_
    */
    float getStepLength() const; 

    /**	Minimize the energy of the system using a nonlinear conjugate gradient method 
        with  line search approach 
    */

    bool  minimize(Size iterations = 0, bool restart = false); 
    //@}


    /**	@name	Public Attributes
    */
    //@{
    /**	Options Force field options
    */
    Options	options;
    //@}

    protected:

    /*_	@name	Protected Attributes */
    //_@{

    /*_ The number of movable atoms in the system
    */
    Size no_of_atoms_; 

    /*_ Vectors for gradients and search directions
    */
    vector<Vector3> new_gradient_,old_gradient_,search_direction_;

    /*_ The control factor alpha for doing the line search step
    */
    float line_search_alpha_; 

    /*_ The control factor beta for doing the line search step
    */
    float line_search_beta_; 

    /*_ The initial step length used in the line search 
    */
    float step_length_; 

    /*_ The inverse of the current length of the search direction vector 
    */
    double inv_search_dir_norm_; 

    /*_ The optimal lambda in the last line search 
    */
    double lambda_opt_; 

    /*_ The current length of the gradient
    */
    double new_gradient_norm_; 
    //_@}

    /*_ @name Protected methods. They are meant for internal use only!
    */
    //_@{

    /*_ Calculate the current RMS gradient of a given gradient 
    */
    double calculateRMSGradientNorm(vector<Vector3> &gradient); 

    /*_ Determine a new search direction 
    */
    void determineNewSearchDirection(bool return_gradient = false); 

    /*_ Determine a new solution point. True is returned when successful. 
    */
    bool determineNewSolution(); 

    /*_ Determine a new tentative lambda for the line search along
        the current search direction vector 
    */
    double determineNewLambda(double f_0, double f_1, double dir_0, double dir_1,
                              double lambda_0, double lambda_1); 
    //_@}



    };

  } // end of namespace BALL



#endif // BALL_MOLMEC_MINIMIZATION_CONJUGATEGRADIENT_H 
