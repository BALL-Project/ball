// $Id: energyMinimizer.h,v 1.15 2000/03/28 15:32:10 oliver Exp $
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

#ifndef BALL_MOLMEC_COMMON_GRADIENT_H
#	include <BALL/MOLMEC/COMMON/gradient.h>
#endif

namespace BALL 
{

	class ForceField;

	class EnergyMinimizer
	{
		public:

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
      static const char* MAX_SAME_ENERGY;

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


    /** Implements the convergence criterion.
				If the convergence criterion is fulfilled, this method
				returns {\bf true}. The convergence criterion is implemented as one
				of three conditions:
				\begin{itemize}
					\item {RMS gradient} is below max_rms_gradient_
					\item \Ref{same_energy_counter_} is above \Ref{max_same_energy_}
					\item {the energy difference} between two successive steps is below \Ref{energy_difference_bound_}
				\end{itemize}
				If any of these conditions hold \Ref{isConverged} returns {\bf true}.
				This method should be reimplemented in derived classes for a different
				convergence criterion.
    */
    virtual bool isConverged() const;

    /** Calculate the next step.
				This method is implemented in each minimizer class and 
				tries to determine the next step to be taken.
				It typically performs a line search.
        @return bool {\bf true} if an acceptable solution was found
    */
    virtual bool findStep();


    /** Update the search direction.
				This method is implemented by the derived classes to implement a 
				method to determine a new search direction.
    */
    virtual void updateDirection();

    /** Update energy.
				This method calls {\tt force_field_->updateEnergy()} and stores
				the result in {\tt current_energy_}.
    */
    virtual double updateEnergy();

    /** Update forces and store them in current_grad_.
				This method calls {\tt force_field_->updateForces()} and stores them in 
				\Ref{current_grad_}.
    */
    virtual void updateForces();

    /** Print the energy.
        This method is called by \Ref{finishIteration} after every
        \Ref{energy_output_frequency_} steps.
        It prints the current RMS gradient and the current energy to \Ref{Log}{\tt .info()}.
				@see setEnergyOutputFrequency
				@see getEnergyOutputFrequency
    */
    virtual void printEnergy() const;

		/**	Take a snapshot of the system.
				This method is called by \Ref{finishIteration} after every
				\Ref{snapshot_frequency_} steps.
				It saves a {\tt SnapShot} of the current atom coordinates to a \Ref{SnapShotManager}
				(if enabled).
				@see setSnapShotFrequency
				@see getSnapShotFrequency
		*/
		virtual void takeSnapShot() const;

		/**	Finishing step for this iteration.
				This method should be called at the end of the main iteration 
				loop implemented in \Ref{minimize}. It takes over some administrative stuff:
				\begin{itemize}
					\item increment the iteration counter \Ref{number_of_iteration_}
					\item call \Ref{takeSnapShot} if necessary
					\item call \Ref{printEnergy} if necessary
					\item call \Ref{ForceField::update} if necessary (to rebuild the pair lists!)
				\end{itemize}

				This method should be overwritten only in rare cases. Even then, the programmer
				should make sure to call {\tt EnergyMinimizer::finishIteration} or 
				has to take care of the above items himself.

				All derved classes should call this method at the end of the minimize main loop.
				Otherwise strange things might happen.
				@see	minimize
		*/
		virtual void finishIteration();
		
		/**	Get the current iteration number
		*/
		Size	getNumberOfIteration() const;

		/**	Return a reference to the current search direction
		*/
		Gradient& getDirection();

		/**	Return a reference to the current gradient
		*/
		Gradient& getGradient();

		/**	Return a reference to the initial gradient
		*/
		Gradient& getInitialGradient();

		/**	Return the current energy
		*/
		double getEnergy() const;

		/**	Return the initial energy
		*/
		double getInitialEnergy() const;

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
				the options ({\tt MAX_STEPS}). Together with the {\tt restart} option
				this feature is used to extract properties or visualize the results
				in the course of the minimization. If restart is set to {\bf true},
				the minimization continues with the former step width.	
				@param		steps maximum number of steps to be taken
				@param		restart {\bf true} if the minimization is to be continued
				@return		bool - {\bf true} if the minimization is terminated
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
		/**	@name	Protected Attributes
		*/
		//@{

		/**	The gradient at the beginning of the current minimization step.
		*/
		Gradient initial_grad_;

		/**	The current gradient.
		*/
		Gradient current_grad_;

		/**	The energy at the beginning of the current minimization step.
		*/
		double initial_energy_;

		/**	The current energy.
		*/
		double current_energy_;

    /** The gradient from the last step
    */
    Gradient old_grad_;

    /** The energy from the last step
    */
    double old_energy_;
 
    /** The current search direction
    */
    Gradient direction_;
 
		/**	The boolean variable indicates if the setup of the energy minimizer was successful
		*/
		bool 	valid_;

		/** Pointer to a SnapShotManager for storing snapshots of the
					system 
		*/
		SnapShotManager* snapshot_ptr_; 

		/**	The force field bound to the energy minimizer.
				Among other data the force field contains the molecular system
				whose energy will be minimized by the energy minimizer.
		*/
		ForceField*	force_field_;

		/**	the current iteration number
		*/
		Size	number_of_iteration_;

		/**	Maximal number of iterations 
		*/
		Size	maximal_number_of_iterations_;

		/**	Frequency of energy output  
		*/
		Size	energy_output_frequency_;

		/**	Frequency of atom coordinate ouput;
		*/
		Size	snapshot_frequency_;

		/**	If the energy difference (before and after an iteration) 
				is smaller than this bound, the minimization procedure stops.
		*/
		double	energy_difference_bound_;

    /** The maximum RMS gradient tolerated (first convergence criterion)
    */
    float max_gradient_;

    /** The maximum number of iterations with same energy.
        When this number is reached, we assume the system to have converged
        (second convergence criterion)
    */
    Size max_same_energy_;

		/**	A counter for the number of steps with a similar energy.
		*/
		Size same_energy_counter_;

		/**	The maximal shift of an atom per iteration step (in Angstrom).
		*/
		float	maximal_shift_;

    /** Internal counter: how often is a force update done.
       Measure for the speed of minimization 
    */
    int force_update_counter_;

    /** Internal counter: how often is an energy update done.
       Measure for the speed of minimization 
    */
    int energy_update_counter_; 
		//_@}
	};

} // namespace Ball



#endif // BALL_MOLMEC_MINIMIZATION_ENERGYMINIMIZER_H
