// $Id: energyMinimizer.h,v 1.2 1999/09/14 17:45:03 oliver Exp $
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

/*
#ifndef BALL_MOLMEC_COMMON_TRAJECTORY_H
#	include <BALL/MOLMEC/COMMON/snapshot.h>
#endif
*/

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
			static const char* SNAPSHOT_OUTPUT_FREQUENCY;

			/**    Number of iteration 
			*/
			static const char* NUMBER_OF_ITERATION;

			/**	Energy difference bound
			*/
			static const char* ENERGY_DIFFERENCE_BOUND;

			/**	Maximal shift
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

			/**    Snapshot ouput frequency 
			*/
			static Size SNAPSHOT_OUTPUT_FREQUENCY;

			/**    Number of iteration 
			*/
			static Size NUMBER_OF_ITERATION;

			/**	Energy difference bound
			*/
			static float ENERGY_DIFFERENCE_BOUND;

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
		bool	setup(ForceField& force_field, const Options& options);

		/**	Specific setup
		*/
		virtual bool specificSetup(void);

		//@}


		/**	@name	Accessors 
		*/
		//@{


		/**	Get the current iteration number
		*/
		Size	getNumberOfIteration(void) const;

		/**	Set the iteration number
		*/
		void	setNumberOfIteration(Size number_of_iteration);

		/**	Get the maximal number of iterations
		*/
		Size	getMaximalNumberOfIterations() const;

		/**	Set the maximal number of iterations
		*/
		void	setMaximalNumberOfIterations(Size maximal_number_of_iterations);


//		/**	Return the snapshot object of the minimization
//		*/
//		Snapshot& getSnapshot();

//		/**	Set the snapshot object of the energy minimizer
//		*/
//		void	setSnapshot(Snapshot& snapshot);


		/**	Set the energy output frequency
		*/
		void	setEnergyOutputFrequency(Size	energy_output_frequency);

		/**	Get the energy ouput frequency
		*/
		Size	getEnergyOutputFrequency(void) const;

		/** Set the energy difference bound
		*/
		void  	setEnergyDifferenceBound(float energy_difference_bound);

		/**	Get the energy difference bound
		*/
		float	getEnergyDifferenceBound(void) const;

		/**     Set the maximal shift value
		*/
		void    setMaximalShift( float maximal_shift );

		/**     Get the maximal shift value
		*/
		float   getMaximalShift( void ) const;

		/**	Set the snapshot output frequency
		*/
		void	setSnapshotOutputFrequency(Size snapshot_output_frequency);

		/**	Get the snapshot output frequency
		*/
		Size	getSnapshotOutputFrequency(void) const;

		/**	Return the force field of the energy minimizer
		*/
		ForceField*	getForceField(void);

		/**	Minimize the energy of the system bound to the force field.	
				If a number of steps is given, the minimization is aborted after
				that number of steps, regardless of the number of steps given in 
				the options ({\tt MAX_STEPS}). Together with the {\tt restart} option
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


		/*_	The force field bound to the energy minimizer.
			Among other data the force field contains the molecular system
			whose energy will be minimized by the energy minimizer.
		*/
		ForceField*	force_field_;

//		/**	Snapshot of the atoms during the minimization
//		*/
//		Snapshot	snapshot_;

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
		Size	snapshot_output_frequency_;

		/*_	If the energy difference (before and after an iteration) 
			is smaller than this bound, the minimization procedure stops.
		*/
		float	energy_difference_bound_;

		/*_	The maximal shift of an atom per iteration step (in Angstroem)
		*/
		float	maximal_shift_;

		//_@}
	};

} // namespace Ball



#endif // BALL_MOLMEC_MINIMIZATION_ENERGYMINIMIZER_H
