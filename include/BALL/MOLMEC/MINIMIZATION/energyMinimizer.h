// $Id: energyMinimizer.h,v 1.9 1999/12/30 20:30:41 oliver Exp $
// Energy Minimizer: A class for minimizing the energy of molecular systems

#ifndef BALL_MOLMEC_MINIMIZATION_ENERGYMINIMIZER_H
#define BALL_MOLMEC_MINIMIZATION_ENERGYMINIMIZER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
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

#ifndef BALL_MOLMEC_COMMON_SNAPSHOT_H
#	include <BALL/MOLMEC/COMMON/snapShot.h>
#endif

#include <vector>

namespace BALL {

	class ForceField;

	class EnergyMinimizer
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

		/**	@name	Option definitions
		*/
		//@{

		/**     Option names
		*/
		struct Option
		{
			/**     Maximal number of iterations
			*/
			static const char* MAXIMAL_NUMBER_OF_ITERATIONS;

			/**     Energy output frequency
			*/
			static const char* ENERGY_OUTPUT_FREQUENCY;

			/**    Trajectory ouput frequency 
			*/
			static const char* SNAPSHOT_FREQUENCY;

			/**    Number of iteration 
			*/
			static const char* NUMBER_OF_ITERATION;

			/**	Energy difference bound
			*/
			static const char* ENERGY_DIFFERENCE_BOUND;

      /** The number of iterations without any change in energy. This
          is used to detect convergence.
      */
      static const char *MAX_SAME_ENERGY;

      /** The maximum RMS gradient allowed for convergence.
      */
      static const char* MAX_GRADIENT;

			/**	Maximal shift of an atom per iteration 
			*/
			static const char* MAXIMAL_SHIFT;

		}; 

		struct Default
		{
			/**     Maximal number of iterations
			*/
			static Size MAXIMAL_NUMBER_OF_ITERATIONS;

			/**     Energy output frequency
			*/
			static Size ENERGY_OUTPUT_FREQUENCY;

			/**    Snapshot frequency 
			*/
			static Size SNAPSHOT_FREQUENCY;

			/**    Number of iteration 
			*/
			static Size NUMBER_OF_ITERATION;

			/**	Energy difference bound
			*/
			static float ENERGY_DIFFERENCE_BOUND;

      /** The number of iterations without any change in energy. This
          is used to detect convergence.
      */
      static Size MAX_SAME_ENERGY; 

      /** Default value for the maximum RMS gradient. This value is used for
          deciding when to stop the minimisation.
      */
      static float MAX_GRADIENT;


			/**	Maximal shift
			*/
			static float MAXIMAL_SHIFT;
		};

		//@}



		/**	@name	Constructors and Destructors	
		*/
		//@{
		
		BALL_CREATE(EnergyMinimizer)

		/**	Default constructor.
		*/
		EnergyMinimizer();

		/**	Constructor.
		*/
		EnergyMinimizer(ForceField& force_field);

		/**	Constructor.
		*/
		EnergyMinimizer(ForceField& force_field, const Options& options);

		/**	Copy constructor
		*/
		EnergyMinimizer(const EnergyMinimizer& energy_minimizer, bool deep = true);

		/**	Destructor.
		*/
		virtual ~EnergyMinimizer();

		//@}


		/**	@name	Assignments 
		*/
		//@{

		/**	Assignment operator
		*/
		EnergyMinimizer&	operator = (const EnergyMinimizer& energy_minimizer);

		//@}

		/**	@name	Debugging and Diagnostics 
		*/
		//@{
		/**	Is the energy minimizer valid : did the setup work?
		*/
		bool	isValid() const;

		//@}

		/**	@name	Setup methods 
		*/
		//@{


		/**	Sets up the energy minimizer.
		*/
		bool	setup(ForceField& force_field);

		/**	Sets up the energy minimizer.
		*/
		bool	setup(ForceField& force_field, SnapShotManager *ssm);

		/**	Sets up the energy minimizer.
		*/
		bool	setup(ForceField& force_field, SnapShotManager *ssm,
                                                     const Options& options);

		/**	Sets up the energy minimizer.
		*/
		bool	setup(ForceField& force_field, const Options& options);

		/**	Specific setup
		*/
		virtual bool specificSetup();

		//@}


		/**	@name	Accessors 
		*/
		//@{


		/**	Get the current iteration number
		*/
		Size	getNumberOfIteration() const;

		/**	Set the iteration number
		*/
		void	setNumberOfIteration(Size number_of_iteration);

		/**	Get the maximal number of iterations
		*/
		Size	getMaximalNumberOfIterations() const;

		/**	Set the maximal number of iterations
		*/
		void	setMaximalNumberOfIterations(Size maximal_number_of_iterations);

    /** Set the maximum number of iterations allowed with equal energy
        (second convergence criterion)
    */
    void  setMaxSameEnergy(Size number);

    /** Get the maximum number of iterations allowed with equal energy
       (second convergence criterion)
    */
    Size  getMaxSameEnergy() const;

		/**	Set the energy output frequency
		*/
		void	setEnergyOutputFrequency(Size	energy_output_frequency);

		/**	Get the energy ouput frequency
		*/
		Size	getEnergyOutputFrequency() const;

		/** Set the energy difference bound for convergence 
		*/
		void  	setEnergyDifferenceBound(float energy_difference_bound);

		/**	Get the energy difference bound
		*/
		float	getEnergyDifferenceBound() const;

    /** Set the maximum RMS gradient (first convergence criterion).
      The gradient unit of the gradient is {\bf kJ/(mol \AA)}.
    */
    void  setMaxGradient(float max_gradient);

    /** Get the maximum RMS gradient (first convergence criterion).
        The gradient unit of the gradient is {\bf kJ/(mol \AA)}.
    */
    float getMaxGradient() const;


		/**     Set the maximal shift value
		*/
		void    setMaximalShift( float maximal_shift );

		/**     Get the maximal shift value
		*/
		float   getMaximalShift( void ) const;

		/**	Set the snapshot frequency
		*/
		void	setSnapShotFrequency(Size snapshot_frequency);

		/**	Get the snapshot output frequency
		*/
		Size	getSnapShotFrequency() const;

		/**	Return the force field of the energy minimizer
		*/
		ForceField*	getForceField();

		/**	Minimize the energy of the system bound to the force field.	
				If a number of steps is given, the minimization is aborted after
				that number of steps, regardless of the number of steps given in 
				the options ({\tt MAX\_STEPS}). Together with the {\tt restart} option
				this feature is used to extract properties or visualize the results
				in the course of the minimization. If restart is set to {\bf true},
				the minimization continues with the former step width.	
				@param		steps maximum number of steps to be taken
				@param		restart {\bf true} if the minimization is to be continued
				@return		{\bf true} if the convergence criterion is fulfilled
		*/
		virtual bool	minimize(Size steps = 0, bool restart = false);

		//@}


		/**	@name	Public Attributes
		*/
		//@{
		
		/**	Options Force field options
		*/
		Options	options;


		//@}

		protected:
		/*_	@ name	Protected Attributes
		*/
		//_@{

		/*_	The boolean variable indicates if the setup of the energy minimizer was successful
		*/
		bool 	valid_;

                /* Pointer to a SnapShotManager for storing snapshots of the
                   system 
                */
                SnapShotManager *snapShot_ptr_; 


		/*_	The force field bound to the energy minimizer.
			Among other data the force field contains the molecular system
			whose energy will be minimized by the energy minimizer.
		*/
		ForceField*	force_field_;

		/*_	the current iteration number
		*/
		Size	number_of_iteration_;

		/*_	Maximal number of iterations 
		*/
		Size	maximal_number_of_iterations_;

		/*_	Frequency of energy output  
		*/
		Size	energy_output_frequency_;

		/*_	Frequency of atom coordinate ouput;
		*/
		Size	snapshot_frequency_;

		/*_	If the energy difference (before and after an iteration) 
			is smaller than this bound, the minimization procedure stops.
		*/
		float	energy_difference_bound_;

    /*_ The maximum RMS gradient tolerated (first convergence criterion)
    */
    float max_gradient_;


    /*_ The maximum number of iterations with same energy.
        When this number is reached, we assume the system to have converged
        (second convergence criterion)
    */
    Size max_same_energy_;


		/*_	The maximal shift of an atom per iteration step (in Angstroem)
		*/
		float	maximal_shift_;

    /*_ Internal counters: how often is a force update and an energy 
        update done -> measures speed of minimisation 
    */
    int force_update_counter_;
    int energy_update_counter_; 


		//_@}
	};

} // namespace Ball



#endif // BALL_MOLMEC_MINIMIZATION_ENERGYMINIMIZER_H
