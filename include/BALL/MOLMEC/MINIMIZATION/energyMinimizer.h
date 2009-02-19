// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: energyMinimizer.h,v 1.44.20.3 2007/08/07 09:12:33 aleru Exp $
//

// Energy Minimizer: A class for minimizing the energy of molecular systems

#ifndef BALL_MOLMEC_MINIMIZATION_ENERGYMINIMIZER_H
#define BALL_MOLMEC_MINIMIZATION_ENERGYMINIMIZER_H

#ifndef BALL_DATATYPE_OPTIONS_H
#	include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_MOLMEC_COMMON_GRADIENT_H
#	include <BALL/MOLMEC/COMMON/gradient.h>
#endif

namespace BALL 
{
	class ForceField;
	class SnapShotManager;
	
	/** EnergyMinimizer. 
	 *  Base class for all minimizer for geometry optimization.
	 *  \ingroup  MolmecEnergyMinimizer
	 */
	class BALL_EXPORT EnergyMinimizer
	{
		public:
		
			/** @name Option definitions
			*/
			//@{
			
			/** Option names
			*/
			struct BALL_EXPORT Option
			{
				/** Max number of iterations
				*/
				static const char* MAXIMAL_NUMBER_OF_ITERATIONS;
				
				/** Energy output frequency
				*/
				static const char* ENERGY_OUTPUT_FREQUENCY;
				
				/** Trajectory ouput frequency 
				*/
				static const char* SNAPSHOT_FREQUENCY;
				
				/** Number of iteration 
				*/
				static const char* NUMBER_OF_ITERATION;
				
				/**	Energy difference bound
				*/
				static const char* ENERGY_DIFFERENCE_BOUND;
				
				/** The number of iterations without any change in energy. This
				 *  is used to detect convergence.
				 */
				static const char* MAX_SAME_ENERGY;
				
				/** The maximum RMS gradient allowed for convergence.
				 *  If the current rms gradient is below this one, we are converged.
				 */
				static const char* MAX_GRADIENT;
				
				/** Max shift of an atom per iteration 
				*/
				static const char* MAXIMUM_DISPLACEMENT;
			};
			
			struct BALL_EXPORT Default
			{
				/** Max number of iterations
				*/
				static Size MAXIMAL_NUMBER_OF_ITERATIONS;
				
				/** Energy output frequency
				*/
				static Size ENERGY_OUTPUT_FREQUENCY;
				
				/** Snapshot frequency 
				*/
				static Size SNAPSHOT_FREQUENCY;
				
				/** Number of iteration 
				*/
				static Size NUMBER_OF_ITERATION;
				
				/** Energy difference bound.
				 *  The energy difference needed for assuming 'equal energy' 
				 */
				static float ENERGY_DIFFERENCE_BOUND;
				
				/** The number of iterations without any change in energy. This
				 *  is used to detect convergence.
				 *  If this number is reached, we assume convergence.
				 */
				static Size MAX_SAME_ENERGY; 
				
				/** Default value for the maximum RMS gradient. This value is used for
				 *  deciding when to stop the minimisation.
				 */
				static float MAX_GRADIENT;
				
				/** Max shift
				*/
				static float MAXIMUM_DISPLACEMENT;
			};
			
			//@}
			/** @name Constructors and Destructors
			*/
			//@{
			
			BALL_CREATE(EnergyMinimizer)
			
			/** Default constructor.
			*/
			EnergyMinimizer();
			
			/** Constructor.
			*/
			EnergyMinimizer(ForceField& force_field);
			
			/** Constructor.
			*/
			EnergyMinimizer(ForceField& force_field, const Options& options);
			
			/** Copy constructor
			*/
			EnergyMinimizer(const EnergyMinimizer& energy_minimizer);
			
			/** Destructor.
			*/
			virtual ~EnergyMinimizer();
			
			//@}
			/** @name Assignments
			*/
			//@{
			
			/** Assignment operator
			*/
			const EnergyMinimizer&	operator=(const EnergyMinimizer& energy_minimizer);
			
			//@}
			
			/** Equality operator
			 */
			bool operator == (const EnergyMinimizer& energy_minimizer) ;
			
			/** @name Debugging and Diagnostics
			*/
			//@{
			
			/** Is the energy minimizer valid : did the setup work?
			*/
			bool isValid() const;
			
			//@}
			/** @name Setup methods
			*/
			//@{
			
			/** Sets up the energy minimizer.
			*/
			bool setup(ForceField& force_field);
			
			/** Sets up the energy minimizer.
			*/
			bool setup(ForceField& force_field, SnapShotManager *ssm);
			
			/** Sets up the energy minimizer.
			*/
			bool setup(ForceField& force_field, 
								 SnapShotManager *ssm, const Options& options);
			
			/** Sets up the energy minimizer.
			*/
			bool setup(ForceField& force_field, const Options& options);
			
			/** Specific setup
			*/
			virtual bool specificSetup();
			
			//@}
			/** @name Accessors
			*/
			//@{
			
			/** Implements the convergence criterion.
			 *  If the convergence criterion is fulfilled, this method
			 *  returns <b>true</b>. The convergence criterion is implemented as one
			 *  of two conditions:
			 *
			 *  (1) {RMS gradient} is below max_rms_gradient_
			 *  (2) \link same_energy_counter_ same_energy_counter_ \endlink  is above  \link max_same_energy_ max_same_energy_ \endlink
			 *
			 *  If any of these conditions hold  \link isConverged isConverged \endlink  returns <b>true</b>.
			 *  This method should be reimplemented in derived classes for a different
			 *  convergence criterion.
			 */
			virtual bool isConverged() const;
			
			/** Calculate the next step.
			 *  This method is implemented in each minimizer class and
			 *  tries to determine the next step to be taken.
			 *  It typically performs a line search.
			 *  The value returned is usually the step length with respect to the
			 *  current direction.
			 *  @return -1 to indicate failure
			 */
			virtual double findStep();
			
			/** Update the search direction.
			 *  This method is implemented by the derived classes to implement a
			 *  method to determine a new search direction.
			 */
			virtual void updateDirection();
			
			/** Update energy.
			 *  This method calls <tt>force_field_->updateEnergy()</tt> and stores
			 *  the result in <tt>current_energy_</tt>.
			 */
			virtual double updateEnergy();
			
			/** Update forces and store them in current_grad_.
			 *  This method calls <tt>force_field_->updateForces()</tt> and stores them in
			 *  \link current_grad_ current_grad_ \endlink .
			 */
			virtual void updateForces();
			
			/** Store the current energy and gradient.
			 *  The current gradient and current energy is copied into
			 *  initial energy and initial gradient. This is usually done at
			 *  the start of an iteration.
			 */
			void storeGradientEnergy();
			
			/** Print the energy.
			 *  This method is called by  \link finishIteration finishIteration \endlink  after every
			 *  \link energy_output_frequency_ energy_output_frequency_ \endlink  steps.
			 *  It prints the current RMS gradient and the current energy to  \link Log Log \endlink <tt>.info()</tt>.
			 *  @see setEnergyOutputFrequency
			 *  @see getEnergyOutputFrequency
			 */
			virtual void printEnergy() const;
			
			/** Take a snapshot of the system.
			 *  This method is called by  \link finishIteration finishIteration \endlink  after every
			 *  \link snapshot_frequency_ snapshot_frequency_ \endlink  steps.
			 *  It saves a <tt>SnapShot</tt> of the current atom coordinates to a  \link SnapShotManager SnapShotManager \endlink 
			 *  (if enabled).
			 *  @see setSnapShotFrequency
			 *  @see getSnapShotFrequency
			 */
			virtual void takeSnapShot() const;
			
			/** Finishing step for this iteration.
			 *  This method should be called at the end of the main iteration
			 *  loop implemented in  \link minimize minimize \endlink . It takes over some administrative stuff:
			 *
			 *  - increment the iteration counter  \link number_of_iterations_ number_of_iterations_ \endlink
			 *  - call  \link takeSnapShot takeSnapShot \endlink  if necessary
			 *  - call  \link printEnergy printEnergy \endlink  if necessary
			 *  - call  \link ForceField::update ForceField::update \endlink  if necessary (to rebuild the pair lists!)
			 *  - update the \link same_energy_counter_ same_energy_counter_ \endlink tested in \link isConverged isConverged \endlink
			 *
			 *  This method should be overwritten only in rare cases. Even then, the programmer
			 *  should make sure to call <tt>EnergyMinimizer::finishIteration</tt> or
			 *  has to take care of the above items himself.
			 *
			 *  All derived classes should call this method at the end of the minimize main loop.
			 *  Otherwise strange things might happen.
			 *  @see minimize
			 */
			virtual void finishIteration();
			
			/** Return the number of iterations performed.
			 */
			Size getNumberOfIterations() const;
			
			/** Return a reference to the current search direction
			 */
			Gradient& getDirection();
			
			/** Return a reference to the current gradient
			 */
			Gradient& getGradient();
			
			/** Return a reference to the initial gradient
			 */
			Gradient& getInitialGradient();
			
			/** Return the current energy
			 */
			double getEnergy() const;
			
			/** Return a reference to the current energy
			 */
			double& getEnergy();
			
			/** Return the initial energy
			 */
			double getInitialEnergy() const;
			
			/** Return a mutable reference to the initial energy
			 */
			double& getInitialEnergy();
			
			/** Set the number of iterations performed so far.
			 */
			void setNumberOfIterations(Size number_of_iterations);
			
			/** Get the maximum number of iterations
			 */
			Size getMaxNumberOfIterations() const;
			
			/** Set the maximum number of iterations
			 */
			void setMaxNumberOfIterations(Size number_of_iterations);
			
			/** Set the maximum number of iterations allowed with equal energy
			 *  (second convergence criterion)
			 */
			void  setMaxSameEnergy(Size number);
			
			/** Get the maximum number of iterations allowed with equal energy
			 *  (second convergence criterion)
			 */
			Size getMaxSameEnergy() const;
			
			/** Set the energy output frequency
			 */
			void setEnergyOutputFrequency(Size	energy_output_frequency);
			
			/** Get the energy ouput frequency
			 */
			Size getEnergyOutputFrequency() const;
			
			/** Set the energy difference bound for convergence
			*/
			void setEnergyDifferenceBound(float energy_difference_bound);
			
			/** Get the energy difference bound
			 */
			float getEnergyDifferenceBound() const;
			
			/** Set the maximum RMS gradient (first convergence criterion).
			 *  The gradient unit of the gradient is <b>kJ/(mol \AA)</b>.
			 */
			void setMaxGradient(float max_gradient);
			
			/** Get the maximum RMS gradient (first convergence criterion).
			 *  The gradient unit of the gradient is <b>kJ/(mol \AA)</b>.
			 */
			float getMaxGradient() const;
			
			/** Set the maximum displacement value.
			 *  This is the maximum distance an atom may be moved by the minimizer in one iteration.
			 */
			void  setMaximumDisplacement(float maximum_displacement);
			
			/** Get the maximum displacement value
			*/
			float getMaximumDisplacement() const;
			
			/** Set the snapshot frequency
			*/
			void setSnapShotFrequency(Size snapshot_frequency);
			
			/** Get the snapshot output frequency
			*/
			Size getSnapShotFrequency() const;
			
			/** Return the force field of the energy minimizer
			*/
			ForceField* getForceField();
			
			/** Return the number of force updates since the start of the minimization.
			*/
			Size getForceUpdateCounter() const ;
			
			/** Return the number of energy updates since the start of the minimization.
			*/
			Size getEnergyUpdateCounter() const ;
			
			/** Minimize the energy of the system bound to the force field.
			 *  If a number of steps is given, the minimization is aborted after
			 *  that number of steps, regardless of the number of steps given in
			 *  the options (<tt>MAX_STEPS</tt>). Together with the <tt>resume</tt> option
			 *  this feature is used to extract properties or visualize the results
			 *  in the course of the minimization. If <tt>resume</tt> is set to <b>true</b>,
			 *  the minimization continues with the former step width and settings.
			 *  @param steps maximum number of steps to be taken
			 *  @param resume <b>true</b> if the minimization is to be resumed with the previous settings
			 *  @return bool - <b>true</b> if the minimization has converged
			 */
			virtual bool	minimize(Size steps = 0, bool resume = false);
			
			/** Specify if the MDSimulation aborts if the Energy is greater than abort_energy_
			*/
			void enableEnergyAbortCondition(bool state);
			
			/// Query if the MDSimulation aborts if the Energy is greater than abort_energy_
			bool energyAbortConditionEnabled() const;
			
			/** Set the value for the energy, that will result in aborting the minization,
			 * if it will be surpassed.
			 * Default value: 10^9.
			 */
			void setEnergyToAbort(float value);
			
			///
			float getEnergyToAbort() const;
			
			/** Return true, if the minimization was aborted, e.g. because of strange
			 *  energies or gradient.
			 */
			bool wasAborted() const ;
			
			//@}
			/**	@name	Public Attributes
			*/
			//@{
			
			/**	Options
			*/
			Options	options;
			//@}
			
		protected:
			
			/** @name Protected Attributes
			*/
			//@{
			
			/** The gradient at the beginning of the current minimization step.
			*/
			Gradient initial_grad_;
			
			/** The current gradient.
			*/
			Gradient current_grad_;
			
			/** The energy at the beginning of the current minimization step.
			*/
			double initial_energy_;
			
			/** The current energy.
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
		 
			/** The boolean variable indicates if the setup of the energy minimizer was successful
			*/
			bool valid_;
			
			/** Pointer to a SnapShotManager for storing snapshots of the system
			*/
			SnapShotManager* snapshot_; 
			
			/** The force field bound to the energy minimizer.
			 *  Among other data the force field contains the molecular system
			 *  whose energy will be minimized by the energy minimizer.
			 */
			ForceField* force_field_;
			
			/** The current iteration number
			*/
			Size number_of_iterations_;
			
			/** Maximum number of iterations 
			*/
			Size maximal_number_of_iterations_;
			
			/** Frequency of energy output
			*/
			Size energy_output_frequency_;
			
			/** Frequency of atom coordinate ouput
			*/
			Size snapshot_frequency_;
			
			/** If the energy difference (before and after an iteration)
			 * is smaller than this bound, the minimization procedure stops.
			 */
			double energy_difference_bound_;
			
			/** The maximum RMS gradient tolerated (first convergence criterion)
			*/
			double max_gradient_;
			
			/** The maximum number of iterations with same energy.
			 *  When this number is reached, we assume the system to have converged
			 *  (second convergence criterion)
			 */
			Size max_same_energy_;
			
			/** A counter for the number of steps with a similar energy.
			*/
			Size same_energy_counter_;
			
			/** The maximal shift of an atom per iteration step (in Angstrom).
			*/
			float maximum_displacement_;
			
			/** Internal counter: how often is a force update done.
			 *  Measure for the speed of minimization.
			 */
			Size force_update_counter_;
			
			/** Internal counter: how often is an energy update done.
			 *  Measure for the speed of minimization.
			 */
			Size energy_update_counter_;
			
			/** Numerical lower bound: we don't want to compute the reciprocal 
			 *  of a number which is lower than 'cutlo_'.
			 */
			float cutlo_;
			
			/** The last step size (in respect of the length of the computed direction vector),
			 *  so the length of the last step was \f$step\cdot|direction|\f$.
			 */
			double step_;
			
			//_ 
			bool abort_by_energy_enabled_;
			
			//_ 
			float abort_energy_;
			
			//_
			bool aborted_;
			
		//_@}
	};
	
} // namespace Ball

#endif // BALL_MOLMEC_MINIMIZATION_ENERGYMINIMIZER_H
